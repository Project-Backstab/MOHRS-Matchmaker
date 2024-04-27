#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <thread>
#include <sstream>

#include <settings.h>
#include <logger.h>
#include <server.h>
#include <globals.h>
#include <database.h>
#include <util.h>
#include <mohrs/region.h>
#include <mohrs/game.h>
#include <mohrs/matchmaker.h>

#include <theater/client.h>

typedef void (Theater::Client::*RequestActionFunc)(const Theater::Parameter&);

static std::map<std::string, RequestActionFunc> mRequestActions = 
{
	{ "CONN",                           &Theater::Client::requestCONN               },
	{ "USER",                           &Theater::Client::requestUSER               },
	{ "PROF",                           &Theater::Client::requestPROF               },
	{ "LLST",                           &Theater::Client::requestLLST               },
	{ "GLST",                           &Theater::Client::requestGLST               },
	{ "RLST",                           &Theater::Client::requestRLST               },
	{ "CGAM",                           &Theater::Client::requestCGAM               },
	{ "UGAM",                           &Theater::Client::requestUGAM               },
	{ "RGAM",                           &Theater::Client::requestRGAM               },
	{ "FILE",                           &Theater::Client::requestFILE               },
};

Theater::Client::Client(int socket, struct sockaddr_in address)
{
	this->_socket = socket;
	this->_address = address;
	this->UpdateLastRecievedTime();
}

Theater::Client::~Client()
{
	this->Disconnect();
}

void Theater::Client::Listen()
{
	while(true)
	{
		std::vector<unsigned char> buffer(4096, 0);
		
		int recv_size = read(this->_socket, &(buffer[0]), 4096);
		
		// If error or no data is recieved we end the connection
		if(recv_size <= 0)
		{
			break;
		}
		
		// Resize buffer
		buffer.resize(recv_size);
		
		this->UpdateLastRecievedTime();

		this->_LogTransaction("-->", Util::Buffer::ToString(buffer));
		
		this->onRequest(buffer);
	}
	
	this->Disconnect();
}

void Theater::Client::Disconnect()
{
	g_matchmaker->removeGame(this->GetAddress());

	this->Close();
	g_theater_server->onClientDisconnect(*this);
}

void Theater::Client::Send(const std::string& action, const Theater::Parameter& parameter) const
{
	// Generate data from parameter
	std::string data = Theater::Client::GetData(parameter);
	
	this->Send(action, data);	
}

void Theater::Client::Send(const std::string& action, const std::string& data) const
{
	std::vector<unsigned char> response(Theater::HEADER_SIZE, 0);
	
	// Set action
	std::copy(action.begin(), action.begin() + 4, response.begin());

	// Set data size
	uint32_t calc_size = Theater::HEADER_SIZE + data.size() + 1;

	response[8] = (calc_size >> 24) & 0xFF;
	response[9] = (calc_size >> 16) & 0xFF;
	response[10] = (calc_size >> 8) & 0xFF;
	response[11] = calc_size & 0xFF;

	// Set data
	response.insert(response.end(), data.begin(), data.end());

	// End
	response.push_back(0x00);

	this->Net::Socket::Send(response);

	this->_LogTransaction("<--", Util::Buffer::ToString(response));
}

// Events

void Theater::Client::onRequest(const std::vector<unsigned char>& request)
{
	if(request.size() <= Theater::HEADER_SIZE)
		return;

	std::string action(request.begin(), request.begin() + 4);
	std::string data(request.begin() + Theater::HEADER_SIZE, request.end() - 1);
	Parameter parameter = Theater::Client::GetParameter(data);

	//uint32_t request_length = 0;
	//request_length |= static_cast<uint32_t>(request[8]) << 24;
	//request_length |= static_cast<uint32_t>(request[9]) << 16;
	//request_length |= static_cast<uint32_t>(request[10]) << 8;
	//request_length |= static_cast<uint32_t>(request[11]);

	// Debug
	//Logger::debug("request_length = " + std::to_string(request_length));
	//Logger::debug("request.size() = " + std::to_string(request.size()));
	//Logger::debug("parameter = " + data);
	//for (const auto& pair : parameter) {
	//	Logger::debug("Key: " + pair.first + ", Value: " + pair.second);
	//}

	auto it = mRequestActions.find(action);
	if (it != mRequestActions.end())
	{
		// Get Function address
		RequestActionFunc func = it->second;
	
		// Execute action function with class object.
		(this->*(func))(parameter);
	}
	else
	{		
		Logger::warning("action \"" + action + "\" not implemented!", Server::Type::Theater);
	}
}

void Theater::Client::requestCONN(const Theater::Parameter& parameter)
{
	this->Send("CONN", {
		{"NUM-CHALLENGES", "0"},
		{"IMPORTANT", "Elon_Musk_Wants_to_Embed_AI-on-a-Chip_Into_Every_Human_Brain"}
	});

	this->Send("CONN", "NUM-CHALLENGES=0");
}

void Theater::Client::requestUSER(const Theater::Parameter& parameter)
{
	this->Send("USER", {
		{ "TICKET", Util::addQuote("1111") }
	});
}

void Theater::Client::requestPROF(const Theater::Parameter& parameter)
{
	if(parameter.find("TEXT") == parameter.end())
	{
		return;
	}

	std::string text = parameter.at("TEXT");

	this->Send("PROF", {
		{ "CLEAN-TEXT", text }
	});
}

void Theater::Client::requestLLST(const Theater::Parameter& parameter)
{
	if(parameter.find("TID") == parameter.end())
	{
		return;
	}

	std::string tid = parameter.at("TID");

	this->Send("LLST", {
		{ "TID", tid},
		{ "NUM-LOBBIES", std::to_string(MoHRS::RegionNames.size()) }
	});

	for(const auto& region : MoHRS::RegionNames)
	{
		MoHRS::Games games;
		int num_fav_games = 0,
			num_fav_players = 0;

		g_matchmaker->findGamesByRegion(region.first, games);
		g_matchmaker->findFavoritesByGames(parameter, games, num_fav_games, num_fav_players);

		this->Send("LDAT", {
			{ "TID", tid },
			{ "LOBBY-ID", std::to_string(static_cast<int8_t>(region.first)) },
			{ "NAME", Util::addQuote(region.second) },
			{ "LOCALE", "1" },
			{ "NUM-GAMES", std::to_string(games.size()) },
			{ "FAVORITE-GAMES", std::to_string(num_fav_games) },
			{ "FAVORITE-PLAYERS", std::to_string(num_fav_players) }
		});
	}
}

void Theater::Client::requestGLST(const Theater::Parameter& parameter)
{
	if(parameter.find("TID") == parameter.end() || parameter.find("LOBBY-ID") == parameter.end())
	{
		return;
	}

	std::string tid = parameter.at("TID");
	std::string lobby_id = parameter.at("LOBBY-ID");

	MoHRS::Game game;
	MoHRS::Games games;
	

	// Convert region
	game.SetRegion(lobby_id);

	// Find games
	g_matchmaker->findGamesByRegion(game.GetRegion(), games);

	this->Send("GLST", {
		{ "TID", tid },
		{ "LOBBY-ID", lobby_id },
		{ "NUM-GAMES", std::to_string(games.size()) },
	});

	for(const MoHRS::Game& game : games)
	{
		int num_fav_games = 0,
			num_fav_players = 0;

		g_matchmaker->findFavoritesByGame(parameter, game, num_fav_games, num_fav_players);

		this->Send("GDAT", {
			{ "TID", tid },
			{ "IP", Util::addQuote(game.GetIp()) },
			{ "PORT", "28500" },
			{ "GAME-ID", std::to_string(game.GetId()) },
			{ "FAVORITE", std::to_string(num_fav_games) },
			{ "NUM-FAV-PLAYERS", std::to_string(num_fav_players) },
			{ "NUM-PLAYERS", std::to_string(game.GetNumPlayers()) },
			{ "MAX-PLAYERS", std::to_string(game.GetMaxPlayers()) },
			{ "NAME", Util::addQuote(game.GetName()) },
		});
	}
}

void Theater::Client::requestRLST(const Theater::Parameter& parameter)
{
	if(parameter.find("TID") == parameter.end())
	{
		return;
	}

	std::string tid = parameter.at("TID");

	this->Send("RLST", {
		{ "TID", tid },
		{ "NUM-REGIONS", std::to_string(MoHRS::RegionNames.size()) }
	});

	for(const auto& region : MoHRS::RegionNames)
	{
		MoHRS::Games games;

		g_matchmaker->findGamesByRegion(region.first, games);

		this->Send("RDAT", {
			{ "TID", tid },
			{ "REGION-ID", std::to_string(static_cast<int8_t>(region.first)) },
			{ "NAME", Util::addQuote(region.second) },
			{ "LOCALE", "0" },
			{ "NUM-GAMES", std::to_string(games.size()) },
			{ "NUM-PLAYERS", "0" }, // Don't see a reason to process
		});
	}
}

void Theater::Client::requestCGAM(const Theater::Parameter& parameter)
{
	MoHRS::Game game;

	if(!g_matchmaker->createGame(*this, parameter, game))
		return;
	
	// Send create game successfull
	this->Send("CGAM", {
		{ "LOBBY-ID", std::to_string(static_cast<uint8_t>(game.GetRegion())) },
		{ "GAME-ID", std::to_string(game.GetId()) }
	});

	// Send update time in seconds
	this->Send("UGAM", {
		{"QUENCH", "20"}
	});
}

void Theater::Client::requestUGAM(const Theater::Parameter& parameter)
{
	g_matchmaker->updateGame(*this, parameter);
}

void Theater::Client::requestRGAM(const Theater::Parameter& parameter)
{
	g_matchmaker->removeGame(this->GetAddress());
}

void Theater::Client::requestFILE(const Theater::Parameter& parameter)
{
	this->Disconnect();
}

// Private functions

void Theater::Client::_LogTransaction(const std::string& direction, const std::string& response) const
{
	std::shared_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read)
	
	//if ((g_logger_mode & Logger::Mode::Development) == 0)
	//{
	//	return;
	//}
	
	bool show_console = (g_settings["theater"]["show_requests"].asBool() && direction == "-->") ||
						(g_settings["theater"]["show_responses"].asBool() && direction == "<--");
	
	Logger::info(this->GetAddress() + " " + direction + " " + response,
			Server::Type::Theater, show_console);
}

Theater::Parameter Theater::Client::GetParameter(const std::string& data)
{
	std::istringstream iss(data);
	Theater::Parameter parameter;

	std::string token;
	while (std::getline(iss, token, ' '))
	{
		// Skip empty tokens
		if (token.empty()) {
			continue;
		}

		// Find the position of '=' in the token
		size_t equalPos = token.find('=');
		if (equalPos != std::string::npos) {
			// Split the token into key and value
			std::string key = token.substr(0, equalPos);
			std::string value = token.substr(equalPos + 1);

			// Add key-value pair to the map
			parameter[key] = value;
		}
	}

	return parameter;
}

std::string Theater::Client::GetData(const Theater::Parameter& parameter)
{
	std::string data;

	for (const auto& pair : parameter)
	{
		data += pair.first + "=" + pair.second + " ";
	}

	// Remove the trailing space if there are any key-value pairs
	if (!parameter.empty())
	{
		data.pop_back();
	}

	return data;
}

// Static functions

void Theater::Client::Heartbeat()
{
	Logger::info("Heartbeat started", Server::Theater);

	while(true)
	{
		std::this_thread::sleep_for (std::chrono::seconds(60));

		auto target_time = std::chrono::system_clock::now() - std::chrono::minutes(1);

		for(std::shared_ptr<Net::Socket> client : g_theater_server->GetClients())
		{
			auto last_recieved = client.get()->GetLastRecievedTime();

			if (last_recieved <= target_time)
			{
				client.get()->Close();
			}
		}
	}
}

