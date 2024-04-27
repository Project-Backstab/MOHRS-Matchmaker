#ifndef WEBSERVER_CLIENT_H
#define WEBSERVER_CLIENT_H

#include <net/socket.h>
#include <util.h>

// Forward declair
namespace atomizes
{
	class HTTPMessage;
};

namespace Json
{
	class Value;
};

namespace Battlefield
{
	class Clan;
	class Player;
};

namespace Webserver
{
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
			 * @brief Send an HTTP response.
			 * 
			 * @param http_response The HTTP response message.
			 */
			void Send(const atomizes::HTTPMessage& http_response) const;

			/**
			 * @brief Send a JSON value.
			 * 
			 * @param value The JSON value to send.
			 */
			void Send(const Json::Value &value) const;
			
			/*
				Events
			*/
			/**
			 * @brief Process incoming HTTP request.
			 * 
			 * @param http_request The HTTP request message.
			 */
			void onRequest(const atomizes::HTTPMessage& http_request);
			
			/*
				Requests
			*/
			/**
			 * @brief Handle a request for a specific file.
			 * 
			 * @param http_request   The HTTP request message.
			 * @param url_base       The base URL for file requests.
			 * @param url_variables  Additional URL variables.
			 */
			void requestFile(const atomizes::HTTPMessage& http_request, const std::string& url_base, const Util::Url::Variables& url_variables);

			/**
			 * @brief Handle a request for a meme.
			 * 
			 * @param http_request   The HTTP request message.
			 * @param url_base       The base URL for meme requests.
			 * @param url_variables  Additional URL variables.
			 */
			void requestMeme(const atomizes::HTTPMessage& http_request, const std::string& url_base, const Util::Url::Variables& url_variables);
			
			// Empty
			/**
			 * @brief Handle a request for a empty response.
			 * 
			 * @param http_request   The HTTP request message.
			 * @param url_base       The base URL for meme requests.
			 * @param url_variables  Additional URL variables.
			 */
			void requestEmpty(const atomizes::HTTPMessage& http_request, const std::string& url_base, const Util::Url::Variables& url_variables);
			
			// API
			/**
			 * @brief Handle a request for admin clients through the API.
			 * 
			 * @param http_request   The HTTP request message.
			 * @param url_base       The base URL for admin clients API requests.
			 * @param url_variables  Additional URL variables.
			 */
			void requestAPIAdminClients(const atomizes::HTTPMessage& http_request, const std::string& url_base, const Util::Url::Variables& url_variables);
		
		private:
			/**
			 * @brief Generate the default HTTP response header.
			 * 
			 * @param isPlainText Flag indicating whether the content type is plain text.
			 * @return The generated HTTP response message.
			 */
			atomizes::HTTPMessage _defaultResponseHeader(bool isPlainText = true) const;

			/**
			 * @brief Log a transaction.
			 * 
			 * This function logs a transaction with the specified direction and response.
			 * 
			 * @param direction The direction of the transaction ("<--" or "-->").
			 * @param response The response to log.
			 */
			void _LogTransaction(const std::string& direction, const std::string& response) const;

			/**
			 * @brief Read a file and store its contents in a string.
			 * 
			 * @param file_name The name of the file to read.
			 * @param data Reference to store the file contents.
			 * @return True if the file was read successfully, false otherwise.
			 */
			bool _readFile(const std::string& file_name, std::string& data) const;

			/**
			 * @brief Send a file as an HTTP response.
			 * 
			 * @param file_name The name of the file to send.
			 */
			void _SendFile(const std::string& file_name) const;
		
		public:
			/**
			 * @brief Heartbeat function to manage client connections.
			 *
			 * This function periodically checks for client connections and closes
			 * connections that have not sent any data in the last minute.
			 */
			static void Heartbeat();
	};
}

#endif // WEBSERVER_CLIENT_H
