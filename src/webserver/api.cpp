#include <atomizes.hpp>
#include <json/json.h>
#include <random>
#include <chrono>

#include <logger.h>
#include <globals.h>
#include <settings.h>
#include <mohrs/game.h>
#include <mohrs/matchmaker.h>

#include <webserver/client.h>

void Webserver::Client::Send(const Json::Value &value) const
{
	// Create a JSON writer
	Json::StreamWriterBuilder writer;
	std::string jsonString = Json::writeString(writer, value);

	// Create http response
	atomizes::HTTPMessage http_response = this->_defaultResponseHeader();
	http_response.SetStatusCode(200);
	http_response.SetMessageBody(jsonString);

	//Logger::debug("json = " + jsonString);

	// Send http response
	this->Send(http_response);
}

void Webserver::Client::requestAPIAdminClients(const atomizes::HTTPMessage& http_request, const std::string& url_base,
		const Util::Url::Variables& url_variables)
{
	// Check password
	auto it = url_variables.find("password");
	if (it == url_variables.end() || it->second != g_settings["webserver"]["password"].asString())
	{
		return;
	}

	Json::Value json_results;

	// Matchmaker
	Json::Value json_matchmaker(Json::arrayValue);
	for(const MoHRS::Game& game : g_matchmaker->GetGames())
	{
		Json::Value json_game;
		
		json_game["id"] = game.GetId();
		json_game["ip"] = game.GetIp();
		json_game["name"] = game.GetName();
		json_game["region"] = static_cast<int8_t>(game.GetRegion());
		json_game["num_players"] = game.GetNumPlayers();
		json_game["max_players"] = game.GetMaxPlayers();
		json_game["host_player"] = game.GetHostPlayer();
		json_game["theater_session"] = game.GetTheaterSession();

		// Players
		Json::Value json_players(Json::arrayValue);
		for(const MoHRS::Player& player : game.GetPlayers())
		{
			Json::Value json_player;

			json_player["name"] = player.GetName();
			json_player["ticket"] = player.GetTicket();

			json_players.append(json_player);
		}
		json_game["players"] = json_players;
		
		json_matchmaker.append(json_game);
	}
	json_results["matchmaker"] = json_matchmaker;

	// Theater
	Json::Value json_theater(Json::arrayValue);
	for(std::shared_ptr<Net::Socket> client : g_theater_server->GetClients())
	{
		Json::Value json_client;

		json_client["ip"] = client.get()->GetIP();
		json_client["port"] = client.get()->GetPort();

		time_t last_recieved_time = std::chrono::system_clock::to_time_t(client.get()->GetLastRecievedTime());
		json_client["last_recieved_time"] = std::string(std::ctime(&last_recieved_time));

		// Debug
		//json_client["ref_count"] = client.use_count();

		json_theater.append(json_client);
	}
	json_results["theater"] = json_theater;

	// Webserver
	Json::Value json_webserver(Json::arrayValue);
	for(std::shared_ptr<Net::Socket> client : g_webserver_server->GetClients())
	{
		Json::Value json_client;

		json_client["ip"] = client.get()->GetIP();
		json_client["port"] = client.get()->GetPort();

		time_t last_recieved_time = std::chrono::system_clock::to_time_t(client.get()->GetLastRecievedTime());
		json_client["last_recieved_time"] = std::string(std::ctime(&last_recieved_time));

		// Debug
		//json_client["ref_count"] = client.use_count();

		json_webserver.append(json_client);
	}
	json_results["webserver"] = json_webserver;

	this->Send(json_results);

	this->_LogTransaction("<--", "HTTP/1.1 200 OK");
}

