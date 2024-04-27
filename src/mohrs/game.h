#ifndef MOHRS_GAME_H
#define MOHRS_GAME_H

#include <vector>
#include <string>
#include <mohrs/region.h>
#include <mohrs/player.h>

/**
    Medal of Honor - Rising Sun
*/
namespace MoHRS 
{
	class Game;

	typedef std::vector<Game> Games;

	class Game
	{
		private:
			int            _id          = -1;
			std::string    _ip          = "";
			std::string    _name        = "";
			MoHRS::Regions _region      = MoHRS::Regions::Unknown;
			uint8_t        _num_players = 0;
			uint8_t        _max_players = 8;
			std::string    _host_player = "";
			Players        _players;
			std::string    _theater_session = "";

		public:
			Game();
			~Game();

			int            GetId() const             { return this->_id;              }
			std::string    GetIp() const             { return this->_ip;              }
			std::string    GetName() const           { return this->_name;            }
			MoHRS::Regions GetRegion() const         { return this->_region;          }
			uint8_t        GetNumPlayers() const     { return this->_num_players;     }
			uint8_t        GetMaxPlayers() const     { return this->_max_players;     }
			std::string    GetHostPlayer() const     { return this->_host_player;     }
			std::string    GetTheaterSession() const { return this->_theater_session; }
			Players        GetPlayers() const        { return this->_players;         }

			bool SetId(int id);
			bool SetId(const std::string& str_id);
			bool SetIp(const std::string& ip);
			bool SetName(const std::string& name);
			bool SetRegion(const MoHRS::Regions region);
			bool SetRegion(int8_t int_region);
			bool SetRegion(const std::string& str_region);
			bool SetNumPlayers(uint8_t num_players);
			bool SetNumPlayers(const std::string& str_num_players);
			bool SetMaxPlayers(uint8_t max_players);
			bool SetMaxPlayers(const std::string& str_max_players);
			bool SetHostPlayer(const std::string& host_player);
			bool SetTheaterSession(const std::string& theater_session);
			bool SetPlayers(const MoHRS::Players& players);
			
			bool AddPlayer(const Player& player);
	};
}

#endif // MOHRS_GAME_H
