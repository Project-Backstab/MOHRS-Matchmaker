#ifndef THEATER_CLIENT_H
#define THEATER_CLIENT_H

#include <net/socket.h>
#include <util.h>

/**
 * @namespace Theater
 * @brief Namespace containing classes related to theater management.
 */
namespace Theater
{
	/**
	 * @typedef Parameter
	 * @brief Typedef for a map of string parameters.
	 */
	typedef std::map<std::string, std::string> Parameter;

	/**
	 * @brief Size of the header for network communication.
	 */
	const int HEADER_SIZE = 12;

	/**
	 * @class Client
	 * @brief Represents a client for network communication in the theater system.
	 * @details Inherits from Net::Socket for network functionality.
	 */
	class Client : public Net::Socket
	{
		public:
			/**
			 * @brief Constructor for Webserver Client.
			 * 
			 * @param socket The socket identifier.
			 * @param address The client's socket address.
			 */
			Client(int socket, struct sockaddr_in address);

			/**
			 * @brief Destructor for Webserver Client.
			 */
			~Client();
			
			/**
			 * @brief Start listening for incoming requests.
			 */
			void Listen();

			/**
			 * @brief Disconnect the client.
			 */
			void Disconnect();
			
			/**
			 * @brief Sends a message with parameters to the client.
			 * @param action The action to be performed.
			 * @param parameter The parameters associated with the action.
			 * @details Sends a message to the client with the specified action and parameters.
			 */
			void Send(const std::string& action, const Theater::Parameter& parameter) const;

			/**
			 * @brief Sends a message with data to the client.
			 * @param action The action to be performed.
			 * @param data The data associated with the action.
			 * @details Sends a message to the client with the specified action and data.
			 */
			void Send(const std::string& action, const std::string& data) const;

			// Events
			
			/**
			 * @brief Handles an incoming request.
			 * @param request The incoming request to be handled.
			 * @details Handles an incoming request received as a vector of unsigned characters.
			 */
			void onRequest(const std::vector<unsigned char>& request);
			
			// Requests
			
			/**
			 * @brief Processes a connection request with the specified parameters.
			 * @param parameter The parameters associated with the connection request.
			 * @details Processes a connection request on the client side with the specified parameters.
			 * The protocol for connection request is as follows:
			 * - Client sends:    CONN.......0 PROT=1 PROD=MOH3-PS2 VERS=2003.1.0.
			 * - Server responds: CONN.......dNUM-CHALLENGES=0.
			 */
            void requestCONN(const Theater::Parameter& parameter);
			
			/**
			 * @brief Processes a user request with the specified parameters.
			 * @param parameter The parameters associated with the user request.
			 * @details Processes a user request on the client side with the specified parameters.
			 * The protocol for the user request is as follows:
			 * - Client sends:    USER.......ZCRC=10dc UID=b9ae75b4a39dbd70acab076760aec09d72257f3ec059517b9fa03a8aebf6187a.
			 * - Server responds: USER........TICKET="1111".
			 */
			void requestUSER(const Theater::Parameter& parameter);
			
			/**
			 * @brief Processes a profile request with the specified parameters.
			 * @param parameter The parameters associated with the profile request.
			 * @details Processes a profile request on the client side with the specified parameters.
			 * The protocol for the profile request is as follows:
			 * - Client sends:    PROF........TEXT="PLAYER1".
			 * - Server responds: PROF.......!CLEAN-TEXT="PLAYER1".
			 */
			void requestPROF(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a list of live streams request with the specified parameters.
			 * @param parameter The parameters associated with the list of live streams request.
			 * @details Processes a request for a list of live streams on the client side with the specified parameters.
			 * The protocol for the list of live streams request is as follows:
			 * - Client sends:    LLST........TID=1.
			 * - Server responds: LLST....... NUM-LOBBIES=1 TID=1.
			 *                    LDAT.......dFAVORITE-GAMES=0 FAVORITE-PLAYERS=0 LOBBY-ID=1 LOCALE=1 NAME="Europe" NUM-GAMES=0 TID=1.
			 */
			void requestLLST(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a global list of streams request with the specified parameters.
			 * @param parameter The parameters associated with the global list of streams request.
			 * @details Processes a request for a global list of streams on the client side with the specified parameters.
			 * The protocol for the global list of streams request is as follows:
			 * - Client sends:    GLST........TID=2 LOBBY-ID=1.
			 * - Server responds: GLST.......)LOBBY-ID=1 NUM-GAMES=1 TID=2.
			 *                    GDAT.......{FAVORITE=0 GAME-ID=1 IP="1.2.3.4" MAX-PLAYERS=8 NAME="Server" NUM-FAV-PLAYERS=0 NUM-PLAYERS=1 PORT=28500 TID=2.
			 */
			void requestGLST(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a recommended list of streams request with the specified parameters.
			 * @param parameter The parameters associated with the recommended list of streams request.
			 * @details Processes a request for a recommended list of streams on the client side with the specified parameters.
			 * The protocol for the recommended list of streams request is as follows:
			 * - Client sends:    RLST........TID=1.
			 * - Server responds: RLST....... NUM-REGIONS=8 TID=1.
			 *                    RDAT.......OLOCALE=0 NAME="Europe" NUM-GAMES=0 NUM-PLAYERS=0 REGION-ID=1 TID=1.
			 */
			void requestRLST(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a create game request with the specified parameters.
			 * @param parameter The parameters associated with the create game request.
			 * @details Processes a request to create a game on the client side with the specified parameters.
			 * The protocol for the create game request is as follows:
			 * - Client sends:    CGAM.......JREGION-ID=1 NAME="Server" MAX-PLAYERS=8 HOST-PLAYER="PLAYER1".
			 * - Server responds: CGAM.......!GAME-ID=1 LOBBY-ID=1.
			 */
			void requestCGAM(const Theater::Parameter& parameter);

			/**
			 * @brief Processes an update game request with the specified parameters.
			 * @param parameter The parameters associated with the update game request.
			 * @details Processes a request to update a game on the client side with the specified parameters.
			 * The protocol for the update game request is as follows:
			 * - Client sends: UGAM.......sLOBBY-ID=1 GAME-ID=1 NAME="Server" NUM-PLAYERS=1 MAX-PLAYERS=8 PLAYER-NAME.1="PLAYER1" TICKET.1="1111".
			 */
			void requestUGAM(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a remove game request with the specified parameters.
			 * @param parameter The parameters associated with the remove game request.
			 * @details Processes a request to remove a game on the client side with the specified parameters.
			 * The protocol for the remove game request is as follows:
			 * - Client sends: RGAM.......!LOBBY-ID=1 GAME-ID=1.
			 */
			void requestRGAM(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a file request with the specified parameters.
			 * @param parameter The parameters associated with the file request.
			 * @details Processes a request for a file on the client side with the specified parameters.
			 * The protocol for the file request is as follows:
			 * - Client sends:    FILE.......ATID=1 TYPE=moh3/tos/0_20753 ENCODING=TEXT CHUNK=1920.
			 * - Server responds: FILE........NUM-CHUNKS=1 TID=1 TYPE=moh3/tos/.
			 *                    FCHU........DATA="<your data>" (Data can be max 2047 bytes).
			 */
			void requestFILE(const Theater::Parameter& parameter);

			/**
			 * @brief Processes a ping request with the specified parameters.
			 * @param parameter The parameters associated with the ping request.
			 * @details Processes a ping request on the client side with the specified parameters.
			 * The protocol for the ping request is as follows:
			 * - Client sends: PING........
			 * - Server responds: PONG........
			 */
			void requestPING(const Theater::Parameter& parameter);

		private:
			/**
			 * @brief Log a transaction.
			 * 
			 * This function logs a transaction with the specified direction and response.
			 * 
			 * @param direction The direction of the transaction ("<--" or "-->").
			 * @param response The response to log.
			 */
			void _LogTransaction(const std::string& direction, const std::string& response) const;
		
		public:
			/**
			 * @brief Heartbeat function to manage client connections.
			 *
			 * This function periodically checks for client connections and closes
			 * connections that have not sent any data in the last minute.
			 */
			static void Heartbeat();
			
			/**
			 * @brief Parses the provided data into a Parameter map.
			 * @param data The data to be parsed into parameters.
			 * @return A Parameter map parsed from the provided data.
			 * @details Parses the provided data into a map of parameters.
			 */
			static Parameter GetParameter(const std::string& data);
			
			/**
			 * @brief Converts the provided Parameter map into a string.
			 * @param parameter The Parameter map to be converted into a string.
			 * @return A string representation of the Parameter map.
			 * @details Converts the provided Parameter map into a string format.
			 */
			static std::string GetData(const Theater::Parameter& parameter);
	};
}

#endif // THEATER_CLIENT_H
