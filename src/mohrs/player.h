#ifndef MOHRS_PLAYER_H
#define MOHRS_PLAYER_H

#include <vector>
#include <string>

/**
    Medal of Honor - Rising Sun
*/
namespace MoHRS 
{
	class Player;

	typedef std::vector<Player> Players;

	/**
     * @brief Represents a player in the game.
     */
	class Player
	{
		private:
			std::string _name;    /**< The name of the player. */
			std::string _ticket;  /**< The ticket associated with the player. */

		public:
			Player();
			~Player();

			std::string GetName() const   { return this->_name;   }
			std::string GetTicket() const { return this->_ticket; }

			bool SetName(const std::string& name);
			bool SetTicket(const std::string& ticket);
	};
}

#endif // MOHRS_PLAYER_H
