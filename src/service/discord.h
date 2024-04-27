#ifndef SERVICE_DISCORD_H
#define SERVICE_DISCORD_H

#include <vector>
#include <dpp/dpp.h>

namespace Service
{
	/**
	 * @brief The Discord class represents a Discord service.
	 * 
	 * This class provides functionality for interacting with the Discord API.
	 */
	class Discord
	{
		private:
			dpp::cluster                _bot;     /**< The Discord bot instance. */
			std::vector<dpp::snowflake> _guilds;  /**< List of guild snowflake IDs. */

		public:
			/**
			 * @brief Default constructor for the Discord class.
			 * 
			 * Initializes a new Discord bot instance.
			 */
			Discord();
			
			/**
			 * @brief Destructor for the Discord class.
			 * 
			 * Cleans up any resources associated with the Discord bot instance.
			 */
			~Discord();

			/**
			 * @brief Starts the Discord service.
			 * 
			 * This function initializes the Discord bot and connects it to the Discord API.
			 * It must be called before using other functionality of the Discord service.
			 */
			void Start();

			/**
			 * @brief Sends a message through the Discord service.
			 * 
			 * @param msg The message to be sent.
			 * 
			 * This function sends a message to the Discord server using the initialized bot instance.
			 * The message is sent to the default channel or recipient.
			 */
			void Send(const std::string& msg);
	};
}

#endif // SERVICE_DISCORD_H
