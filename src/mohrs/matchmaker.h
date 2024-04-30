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

	/**
	 * @brief Represents a matchmaker for managing games.
	 */
	class Matchmaker
	{
		private:
			MoHRS::Games               _games;    /**< The list of games managed by the matchmaker. */
			mutable std::shared_mutex  _mutex;    /**< The mutex for thread-safe access to the games list. */

		public:
			Matchmaker();
			~Matchmaker();

			Games GetGames() const { return this->_games; }

			/**
			 * @brief Creates a new game.
			 * @param client The client associated with the game creation.
			 * @param parameter The parameters for creating the game.
			 * @param game[out] Reference to the created game.
			 * @return True if the game was created successfully, false otherwise.
			 */
			bool createGame(const Theater::Client& client, const Theater::Parameter& parameter, MoHRS::Game& game);

			/**
			 * @brief Updates an existing game.
			 * @param client The client associated with the game update.
			 * @param parameter The parameters for updating the game.
			 * @return True if the game was updated successfully, false otherwise.
			 */
			bool updateGame(const Theater::Client& client, const Theater::Parameter& parameter);

			/**
			 * @brief Removes a game.
			 * @param address The address of the game to be removed.
			 * @return True if the game was removed successfully, false otherwise.
			 */
			bool removeGame(const std::string& address);

			/**
			 * @brief Finds games by region.
			 * @param region The region to search for games.
			 * @param games[out] Reference to the vector to store found games.
			 * @return True if games were found, false otherwise.
			 */
			bool findGamesByRegion(MoHRS::Regions region, MoHRS::Games& games) const;
			
			/**
			 * @brief Finds favorite games and players by a single game.
			 * @param parameter The parameters associated with the search.
			 * @param game The game to search for favorites.
			 * @param num_fav_game[out] Number of favorite games found.
			 * @param num_fav_players[out] Number of favorite players found.
			 * @return True if favorites were found, false otherwise.
			 */
			bool findFavoritesByGame(const Theater::Parameter& parameter, const MoHRS::Game& game, int& num_fav_game, int& num_fav_players) const;

			/**
			 * @brief Finds favorite games and players by multiple games.
			 * @param parameter The parameters associated with the search.
			 * @param games The games to search for favorites.
			 * @param num_fav_game[out] Number of favorite games found.
			 * @param num_fav_players[out] Number of favorite players found.
			 * @return True if favorites were found, false otherwise.
			 */
			bool findFavoritesByGames(const Theater::Parameter& parameter, const MoHRS::Games& games, int& num_fav_game, int& num_fav_players) const;

		private:
			/**
			 * @brief Checks if a game is a favorite.
			 * @param game The game to check.
			 * @param fav_games The list of favorite games.
			 * @param num_fav_games[out] Number of favorite games found.
			 */
			void _checkFavoriteGame(const MoHRS::Game& game, const std::vector<std::string>& fav_games, int& num_fav_games) const;

			/**
			 * @brief Checks if players in a game are favorites.
			 * @param game The game to check.
			 * @param fav_players The list of favorite players.
			 * @param num_fav_players[out] Number of favorite players found.
			 */
			void _checkFavoritePlayers(const MoHRS::Game& game, const std::vector<std::string>& fav_players, int& num_fav_players) const;
	};
}

#endif // MOHRS_MATCHMAKER_H
