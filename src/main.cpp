#include <iostream>
#include <signal.h>
#include <thread>
#include <fstream>
#include <filesystem>

#include <version.h>
#include <logger.h>
#include <settings.h>
#include <globals.h>
#include <server.h>
#include <mohrs/matchmaker.h>
#include <theater/client.h>
#include <webserver/client.h>
#include <service/file_system.h>
#include <service/discord.h>

// Globals
MoHRS::Matchmaker*           g_matchmaker;

Server*                      g_theater_server;
Server*                      g_webserver_server;

class Service::File_System*  g_file_system;
class Service::Discord*      g_discord;

// Settings
Json::Value                  g_settings;
std::shared_mutex            g_settings_mutex;

void load_settings()
{
	std::unique_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read/write)
	Json::CharReaderBuilder builder;
	
	std::ifstream ifs;
	ifs.open("../data/settings.json");
	
	if(ifs.is_open())
	{
		if(!parseFromStream(builder, ifs, &g_settings, nullptr))
		{
			Logger::error("Cant parse settings.json.");
			Logger::error("load_settings() at parseFromStream");
			exit(EXIT_FAILURE);
		}
		
		ifs.close();
	}
}

void start_theater_server()
{
	g_matchmaker = new MoHRS::Matchmaker();
	g_theater_server = new Server(Server::Type::Theater);

	// Wait till discord has a chance to start
	std::this_thread::sleep_for(std::chrono::seconds(1));

	g_theater_server->Listen();
}

void start_webserver_server()
{
	g_webserver_server = new Server(Server::Type::Webserver);	
	g_webserver_server->Listen();
}

void start_file_system()
{
	g_file_system = new class Service::File_System();

	// Load "data/gamescripts" files in memory
	for (const auto& entry : std::filesystem::recursive_directory_iterator("../data"))
	{
		if (entry.is_regular_file())
		{
			// load file in memory
			g_file_system->Load(entry.path());
		}
	}

	Logger::info("Load all files in memory.", Service::Type::File_System);

	// Example code how to use
	/*
	std::string data;

	if(!g_file_system->GetFile("../data/gamescripts/bfmc/ps2/en/PS2news_en_US.txt", data))
	{
		Logger::error("Oepsie doepsie");
	}

	Logger::debug("data = " + data);

	g_file_system->UnLoadAll();
	*/
}

void start_discord()
{
	g_discord = new class Service::Discord();
	g_discord->Start();
}

void signal_callback(int signum)
{
	Logger::info("Caught signal " + std::to_string(signum));
	
	//Disconnect all clients
	g_webserver_server->DisconnectAllClients();
	
	// Close services
	g_webserver_server->Close();
	
	g_file_system->UnLoadAll();

	// Exit application
	exit(signum);
}

int main(int argc, char const* argv[])
{
	Logger::Initialize();
	load_settings();
	
	Logger::info("--- PROJECT INFO ---");
	Logger::info("Project name     = " + std::string(PROJECT_GIT_NAME));
	Logger::info("Project toplevel = " + std::string(PROJECT_GIT_TOPLEVEL));
	Logger::info("Branch name      = " + std::string(PROJECT_GIT_BRANCH_NAME));
	Logger::info("Branch hash      = " + std::string(PROJECT_GIT_BRANCH_HASH));
	Logger::info("Version          = " + std::string(PROJECT_VERSION_STRING));
	
	// Register signal callbacks
	signal(SIGINT, signal_callback);
	signal(SIGTERM, signal_callback);
	signal(SIGQUIT, signal_callback);
	signal(SIGTSTP, signal_callback);
	signal(SIGKILL, signal_callback);
	
	// Start servers
	std::thread t_theater(&start_theater_server);
	std::thread t_theater_heartbeat(&Theater::Client::Heartbeat);
	std::thread t_webserver(&start_webserver_server);
	std::thread t_webserver_heartbeat(&Webserver::Client::Heartbeat);
	std::thread t_file_system(&start_file_system);
	std::thread t_discord(&start_discord);

	t_theater.detach();
	t_theater_heartbeat.detach();
	t_webserver.detach();
	t_webserver_heartbeat.detach();
	t_file_system.detach();
	t_discord.detach();

	// Sleep ZZZZZZzzzzzZZZZZ
	while(true)
	{
		sleep(1);
	}
	
	return EXIT_SUCCESS;
}

