#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
 
#include <logger.h>
#include <settings.h>
#include <theater/client.h>
#include <webserver/client.h>

#include <server.h>

Server::Server(Server::Type type)
{
	std::shared_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read)
	
	int port = -1;
	int socket_type = SOCK_STREAM;

	// socket options
	int opt_reuse = 1;     // Enable reuse option
	
	this->_type = type;
	
	// setting
	switch(type)
	{
		case Server::Type::Theater:
			port = g_settings["theater"]["port"].asInt();
		break;
		case Server::Type::Webserver:
			port = g_settings["webserver"]["port"].asInt();
		break;
	}
	
	if ((this->_socket = socket(AF_INET, socket_type, 0)) < 0)
	{
		Logger::error("Server::Server() at socket", this->_type);
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_reuse, sizeof(opt_reuse)))
	{
		Logger::error("Server::Server() at setsockopt with opt_reuse", this->_type);
		exit(EXIT_FAILURE);
	}

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(port);

	if (bind(this->_socket, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0)
	{
		Logger::error("Server::Server() at bind", this->_type);
		exit(EXIT_FAILURE);
	}
}

std::vector<std::shared_ptr<Net::Socket>> Server::GetClients()
{
	std::lock_guard<std::mutex> guard(this->_mutex); // server lock
	
	return this->_clients;
}

void Server::Listen()
{
	int client_socket;
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	
	if (listen(this->_socket, 3) < 0)
	{
		Logger::error("Server::Listen() on listen", this->_type);
		return;
	}
	
	this->onServerListen();

	while(true)
	{
		if ((client_socket = accept(this->_socket, (struct sockaddr*)&client_address, &client_address_len)) < 0)
		{	
			Logger::error("Server::Listen() on accept", this->_type);
			return;
		}
		
		switch(this->_type)
		{
			case Server::Type::Theater:
			{
				std::lock_guard<std::mutex> guard(this->_mutex); // server lock
				
				this->_clients.push_back(std::make_shared<Theater::Client>(client_socket, client_address));
				
				std::shared_ptr<Net::Socket> client = this->_clients.back();
				
				this->onClientConnect(client);
				
				std::thread t([client]() {
					static_cast<Theater::Client*>(client.get())->Listen();
				});
				t.detach();
			}
			break;

			case Server::Type::Webserver:
			{
				std::lock_guard<std::mutex> guard(this->_mutex); // server lock
				
				this->_clients.push_back(std::make_shared<Webserver::Client>(client_socket, client_address));
				
				std::shared_ptr<Net::Socket> client = this->_clients.back();
				
				this->onClientConnect(client);
				
				std::thread t([client]() {
					static_cast<Webserver::Client*>(client.get())->Listen();
				});
				t.detach();
			}
			break;
		}
	}
}

void Server::DisconnectAllClients()
{
	for(std::shared_ptr<Net::Socket> client : this->_clients)
	{
		switch(this->_type)
		{
			case Server::Type::Webserver:
				dynamic_cast<Webserver::Client*>(client.get())->Disconnect();
			break;
		}
	}
}

void Server::Close()
{
	shutdown(this->_socket, SHUT_RDWR);
	
	onServerShutdown();
}

/*
	Events
*/

void Server::onServerListen() const
{
	Logger::info("Server is now listening on " + this->GetAddress() + " " + this->GetSocketType(), this->_type);
}

void Server::onServerShutdown() const
{
	Logger::info("Server shutdown", this->_type);
}

void Server::onClientConnect(const Net::Socket& client) const
{
	std::shared_lock<std::shared_mutex> guard2(g_settings_mutex); // settings lock (read)
	
	if ((g_logger_mode & Logger::Mode::Development) != 0)
	{
		Logger::info("Client " + client.GetAddress() + " connected",
			this->_type, g_settings["show_client_connect"].asBool());
	}
}

void Server::onClientConnect(const std::shared_ptr<Net::Socket>& client) const
{
	std::shared_lock<std::shared_mutex> guard2(g_settings_mutex); // settings lock (read)
	
	if ((g_logger_mode & Logger::Mode::Development) != 0)
	{
		Logger::info("Client " + client->GetAddress() + " connected",
			this->_type, g_settings["show_client_connect"].asBool());
	}
}

void Server::onClientDisconnect(const Net::Socket& client)
{
	std::shared_lock<std::shared_mutex> guard2(g_settings_mutex); // settings lock (read)
	
	if(this->GetSocketType() == "tcp")
	{
		std::lock_guard<std::mutex> guard(this->_mutex); // server lock
		
		// Find shared pointer in clients list
		auto it = std::find_if(this->_clients.begin(), this->_clients.end(),
			[rawPtrToSearch = const_cast<Net::Socket*>(&client)](const std::shared_ptr<Net::Socket>& ptr)
			{
				return ptr.get() == rawPtrToSearch;
			}
		);
		
		// When found remove client
		if (it != this->_clients.end())
		{
			if ((g_logger_mode & Logger::Mode::Development) != 0)
			{
				Logger::info("Client " + client.GetAddress() + " disconnected",
					this->_type, g_settings["show_client_disconnect"].asBool());
			}

			this->_clients.erase(it);
		}
	}
	else
	{
		if ((g_logger_mode & Logger::Mode::Development) != 0)
		{
			Logger::info("Client " + client.GetAddress() + " disconnected",
				this->_type, g_settings["show_client_disconnect"].asBool());
		}
	}
}

