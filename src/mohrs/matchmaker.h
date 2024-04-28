#ifndef MOHRS_MATCHMAKER_H
#define MOHRS_MATCHMAKER_H

#include <mohrs/game.h>
#include <theater/client.h>
#include <shared_mutex>

/**
    Medal of Honor - Rising Sun
*/
namespace MoHRS
{
	class Game;

	class Matchmaker
	{
		private:
			MoHRS::Games               _games;
			mutable std::shared_mutex  _mutex;

		public:
			Matchmaker();
			~Matchmaker();

			Games GetGames() const { return this->_games; }

			bool createGame(const Theater::Client& client, const Theater::Parameter& parameter, MoHRS::Game& game);
			bool updateGame(const Theater::Client& client, const Theater::Parameter& parameter);
			bool removeGame(const std::string& address);

			bool findGamesByRegion(MoHRS::Regions region, MoHRS::Games& games) const;
			
			bool findFavoritesByGame(const Theater::Parameter& parameter, const MoHRS::Game& game, int& num_fav_game, int& num_fav_players) const;
			bool findFavoritesByGames(const Theater::Parameter& parameter, const MoHRS::Games& games, int& num_fav_game, int& num_fav_players) const;

		private:
			void _checkFavoriteGame(const MoHRS::Game& game, const std::vector<std::string>& fav_games, int& num_fav_games) const;
			void _checkFavoritePlayers(const MoHRS::Game& game, const std::vector<std::string>& fav_players, int& num_fav_players) const;
	};
}

#endif // MOHRS_MATCHMAKER_H
