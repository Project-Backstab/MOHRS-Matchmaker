#ifndef GLOBALS_H
#define GLOBALS_H

// Forward declare
class Server;

namespace Service
{
	class File_System;
}

namespace MoHRS
{
	class Matchmaker;
}

extern MoHRS::Matchmaker*       g_matchmaker;

/**
 * @brief Pointer to the global Theater Server instance.
 */
extern Server*      g_theater_server;

/**
 * @brief Pointer to the global Webserver Server instance.
 */
extern Server*      g_webserver_server;

/**
 * @brief Pointer to the global File_System instance.
 */
extern Service::File_System*    g_file_system;

#endif // GLOBALS_H
