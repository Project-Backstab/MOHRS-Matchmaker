#ifndef THEATER_CLIENT_H
#define THEATER_CLIENT_H

#include <net/socket.h>
#include <util.h>

namespace Theater
{
	typedef std::map<std::string, std::string> Parameter;

	const int HEADER_SIZE = 12;

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
			
			void Send(const std::string& action, const Theater::Parameter& parameter) const;
			void Send(const std::string& action, const std::string& data) const;

			/*
				Events
			*/
			
			void onRequest(const std::vector<unsigned char>& request);
			
			/*
				Requests
			*/
			
            void requestCONN(const Theater::Parameter& parameter);
			void requestUSER(const Theater::Parameter& parameter);
			void requestPROF(const Theater::Parameter& parameter);
			void requestLLST(const Theater::Parameter& parameter);
			void requestGLST(const Theater::Parameter& parameter);
			void requestRLST(const Theater::Parameter& parameter);
			void requestCGAM(const Theater::Parameter& parameter);
			void requestUGAM(const Theater::Parameter& parameter);
			void requestRGAM(const Theater::Parameter& parameter);
			void requestFILE(const Theater::Parameter& parameter);
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
			
			static Parameter GetParameter(const std::string& data);
			static std::string GetData(const Theater::Parameter& parameter);
	};
}

#endif // THEATER_CLIENT_H
