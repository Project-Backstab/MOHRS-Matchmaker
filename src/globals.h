#ifndef GLOBALS_H
#define GLOBALS_H

// Forward declare
class Server;

namespace Service
{
	class File_System;
	class Discord;
}

namespace MoHRS
{
	class Matchmaker;
}

extern MoHRS::Matchmaker*           g_matchmaker;

/**
 * @brief Pointer to the global Theater Server instance.
 */
extern Server*                      g_theater_server;

/**
 * @brief Pointer to the global Webserver Server instance.
 */
extern Server*                      g_webserver_server;

/**
 * @brief Pointer to the global File_System instance.
 */
extern class Service::File_System*  g_file_system;

/**
 * @brief Pointer to the global Discord instance.
 */
extern class Service::Discord*      g_discord;

#endif // GLOBALS_H
