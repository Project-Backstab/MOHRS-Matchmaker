#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <thread>

#include <settings.h>
#include <logger.h>
#include <server.h>
#include <globals.h>
#include <util.h>
#include <atomizes.hpp>
#include <service/file_system.h>

#include <webserver/client.h>

using namespace atomizes;

typedef void (Webserver::Client::*RequestActionFunc)(const atomizes::HTTPMessage&, const std::string&, const Util::Url::Variables&);

static std::map<std::string, RequestActionFunc> mRequestActions = 
{
	// I like memes :D
	{ "/",                                                    &Webserver::Client::requestMeme               },
	{ "/favicon.ico",                                         &Webserver::Client::requestEmpty              },
	
	// API
	{ "/API/admin/clients",                                   &Webserver::Client::requestAPIAdminClients    },
};

Webserver::Client::Client(int socket, struct sockaddr_in address)
{
	this->_socket = socket;
	this->_address = address;
	this->UpdateLastRecievedTime();
}

Webserver::Client::~Client()
{
	this->Disconnect();
}

void Webserver::Client::Listen()
{
	std::vector<char> buffer(4096, 0);
	HTTPMessageParser http_parser;
	HTTPMessage http_request;
	
	// Read socket
	int recv_size = read(this->_socket, &(buffer[0]), 4096);
	
	// If error or no data is recieved we end the connection
	if(recv_size <= 0)
	{
		this->Disconnect();

		return;
	}
	
	// Resize buffer
	buffer.resize(recv_size);
	
	this->UpdateLastRecievedTime();

	// Parse buffer to http header
	http_parser.Parse(&http_request, &(buffer[0]));
	
	// Trigger onRequest event
	this->onRequest(http_request);
	
	this->Disconnect();
}

void Webserver::Client::Disconnect()
{
	this->Close();
	g_webserver_server->onClientDisconnect(*this);
}

void Webserver::Client::Send(const atomizes::HTTPMessage &http_response) const
{
	this->Net::Socket::Send(http_response.ToString());
}

// Events

void Webserver::Client::onRequest(const atomizes::HTTPMessage &http_request)
{
	if(http_request.GetMethod() == MessageMethod::GET)
	{
		std::string url_base;
		Util::Url::Variables url_variables;
		
		this->_LogTransaction("-->", http_request.GetPath());
		
		// Split url into url base and variables
		Util::Url::GetElements(http_request.GetPath(), url_base, url_variables);
		
		auto it = mRequestActions.find(url_base);
		if (it != mRequestActions.end())
		{
			// Get Function address
			RequestActionFunc func = it->second;
		
			// Execute action function with class object.
			(this->*(func))(http_request, url_base, url_variables);
		}
		else
		{		
			Logger::warning("action \"" + url_base + "\" not implemented!", Server::Type::Webserver);
		}
	}
}

void Webserver::Client::requestFile(const atomizes::HTTPMessage& http_request, const std::string& url_base,
		const Util::Url::Variables& url_variables)
{
	this->_SendFile("../data" + url_base);
}

void Webserver::Client::requestEmpty(const atomizes::HTTPMessage& http_request, const std::string& url_base,
		const Util::Url::Variables& url_variables)
{
	HTTPMessage http_response = this->_defaultResponseHeader();
	
	http_response.SetStatusCode(200);
	http_response.SetMessageBody("\r\n");
	
	this->Send(http_response);
	
	this->_LogTransaction("<--", "HTTP/1.1 200 OK");
}

void Webserver::Client::requestMeme(const atomizes::HTTPMessage& http_request, const std::string& url_base,
		const Util::Url::Variables& url_variables)
{
	this->_SendFile("../data/meme/index.html");
}

// Private functions

void Webserver::Client::_LogTransaction(const std::string& direction, const std::string& response) const
{
	std::shared_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read)
	
	//if ((g_logger_mode & Logger::Mode::Development) == 0)
	//{
	//	return;
	//}
	
	bool show_console = (g_settings["webserver"]["show_requests"].asBool() && direction == "-->") ||
						(g_settings["webserver"]["show_responses"].asBool() && direction == "<--");
	
	Logger::info(this->GetAddress() + " " + direction + " " + response,
			Server::Type::Webserver, show_console);
}

atomizes::HTTPMessage Webserver::Client::_defaultResponseHeader(bool isPlainText) const
{
	HTTPMessage http_response;
	
	http_response.SetHeader("Server", "MOHRS-Matchmaker");
	http_response.SetHeader("Accept-Ranges", "bytes");

	if(isPlainText)
		http_response.SetHeader("Content-Type", "text/plain");
	
	return http_response;
}

bool Webserver::Client::_readFile(const std::string &file_name, std::string& data) const
{
	bool finished = false;

	// Load file from memory
	if(g_file_system->GetFile(file_name, data))
	{
		// Debug
		//Logger::debug("file_name = " + file_name);
		//Logger::debug("file size = " + std::to_string(data.size()));
		
		return true;
	}
	
	std::ifstream input;

	try
	{
		input.open(file_name, std::ifstream::in | std::ifstream::binary);

		if(input.is_open())
		{
			data.append((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
			
			input.close();
			
			return true;
		}
	}
	catch(...)
	{
		Logger::error(file_name);
	}
	
	//Logger::debug(std::to_string(data.size()));
	
	return false;
}

void Webserver::Client::_SendFile(const std::string& file_name) const
{
	std::string data;
	atomizes::HTTPMessage http_response = this->_defaultResponseHeader(false);
	
	if(this->_readFile(file_name, data) && data.size() != 0)
	{
		http_response.SetStatusCode(200);
		http_response.SetMessageBody(data);
	}
	else
	{ // fix: Prevent to hang the http connection
		http_response.SetStatusCode(404);
		http_response.SetMessageBody("\r\n");
	}
	
	this->Send(http_response);
	
	this->_LogTransaction("<--", "HTTP/1.1 200 OK");
}

// Static functions

void Webserver::Client::Heartbeat()
{
	Logger::info("Heartbeat started", Server::Webserver);

	while(true)
	{
		std::this_thread::sleep_for (std::chrono::seconds(60));

		auto target_time = std::chrono::system_clock::now() - std::chrono::minutes(1);

		for(std::shared_ptr<Net::Socket> client : g_webserver_server->GetClients())
		{
			auto last_recieved = client.get()->GetLastRecievedTime();

			if (last_recieved <= target_time)
			{
				client.get()->Close();
			}
		}
	}
}

