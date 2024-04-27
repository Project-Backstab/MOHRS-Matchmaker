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

	class Player
	{
		private:
			std::string _name;
			std::string _ticket;

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
