#include <mohrs/matchmaker.h>
#include <mohrs/game.h>

MoHRS::Matchmaker::Matchmaker()
{

}

MoHRS::Matchmaker::~Matchmaker()
{
	
}

bool MoHRS::Matchmaker::createGame(const Theater::Client& client, const Theater::Parameter& parameter, MoHRS::Game& game)
{
	try
	{
		MoHRS::Player player;
		std::string host_player = parameter.at("HOST-PLAYER");

		// Set game server information
		game.SetName(parameter.at("NAME"));
		game.SetRegion(parameter.at("REGION-ID"));
		game.SetMaxPlayers(parameter.at("MAX-PLAYERS"));
		game.SetHostPlayer(host_player);
		game.SetNumPlayers(1);
		
		// Set server information
		game.SetIp(client.GetIP());
		game.SetTheaterSession(client.GetAddress());

		// Set Player
		player.SetName(host_player);
		game.AddPlayer(player);

		// Generate new game id
		game.SetId(this->_genNewGameId());
		
		{
			std::unique_lock<std::shared_mutex> guard(this->_mutex); // matchmaker lock (read/write)

			// Save new game
			this->_games.push_back(game);
		}
	}
	catch(const std::exception& e)
	{
		return false;
	}

	return true;
}

bool MoHRS::Matchmaker::updateGame(const Theater::Client& client, const Theater::Parameter& parameter)
{
	MoHRS::Players players;
	std::string num_players = "1";

	try
	{
		num_players = parameter.at("NUM-PLAYERS");

		for(int i = 1; i <= 8; i++)
		{
			MoHRS::Player player;

			player.SetName(parameter.at("PLAYER-NAME." + std::to_string(i)));
			player.SetTicket(parameter.at("TICKET." + std::to_string(i)));

			players.push_back(player);
		}
	}
	catch(const std::exception& e) {}

	std::unique_lock<std::shared_mutex> guard(this->_mutex); // matchmaker lock (read/write)

	MoHRS::Games::iterator game_it;
	std::string address = client.GetAddress();

	for(game_it = this->_games.begin(); game_it != this->_games.end(); ++game_it)
	{
		if(game_it->GetTheaterSession() == address)
		{
			game_it->SetNumPlayers(num_players);
			game_it->SetPlayers(players);

			return true;
		}
	}

	return false;
}

bool MoHRS::Matchmaker::removeGame(const std::string& address)
{
	std::unique_lock<std::shared_mutex> guard(this->_mutex); // matchmaker lock (read/write)

	MoHRS::Games::const_iterator game_it;

	for(game_it = this->_games.begin(); game_it != this->_games.end(); ++game_it)
	{
		if(game_it->GetTheaterSession() == address)
		{
			// Remove the game server out of the list
			this->_games.erase(game_it);

			// Go back one item value
			game_it--;
		}
	}

	return true;
}

bool MoHRS::Matchmaker::findGamesByRegion(MoHRS::Regions region, MoHRS::Games& games) const
{
	std::shared_lock<std::shared_mutex> guard(this->_mutex); // matchmaker lock (read)

	for(const MoHRS::Game& game : this->_games)
	{
		if(game.GetRegion() == region)
			games.push_back(game);
	}

	return true;
}

bool MoHRS::Matchmaker::findFavoritesByGame(const Theater::Parameter& parameter, const MoHRS::Game& game, int& num_fav_games, int& num_fav_players)
{
	if(parameter.find("FAV-GAME") != parameter.end() && parameter.find("FAV-PLAYER") != parameter.end())
	{
		// Collect favorites
		std::vector<std::string> fav_games = Util::splitFavorite(parameter.at("FAV-GAME"));
		std::vector<std::string> fav_players = Util::splitFavorite(parameter.at("FAV-PLAYER"));

		this->_checkFavoriteGame(game, fav_games, num_fav_games);
		this->_checkFavoritePlayers(game, fav_players, num_fav_players);
	}

	return true;
}

bool MoHRS::Matchmaker::findFavoritesByGames(const Theater::Parameter& parameter, const MoHRS::Games& games, int& num_fav_games, int& num_fav_players)
{
	if(parameter.find("FAV-GAME") != parameter.end() && parameter.find("FAV-PLAYER") != parameter.end())
	{
		// Collect favorites
		std::vector<std::string> fav_games = Util::splitFavorite(parameter.at("FAV-GAME"));
		std::vector<std::string> fav_players = Util::splitFavorite(parameter.at("FAV-PLAYER"));
		
		for(const MoHRS::Game& game : games)
		{
			this->_checkFavoriteGame(game, fav_games, num_fav_games);
			this->_checkFavoritePlayers(game, fav_players, num_fav_players);
		}
	}

	return true;
}

//	Private functions

int MoHRS::Matchmaker::_genNewGameId() const
{
	int id = 1;

	for(const Game game : this->_games)
	{
		if(id >= game.GetId())
			id = game.GetId() + 1;
	}

	return id;
}

void MoHRS::Matchmaker::_checkFavoriteGame(const MoHRS::Game& game, const std::vector<std::string>& fav_games, int& num_fav_games) const
{
	std::string game_name = game.GetName();

	for(const std::string& fav_game : fav_games)
	{
		if(game_name.find(fav_game) != std::string::npos)
		{
			num_fav_games++;
			break;
		}
	}
}

void MoHRS::Matchmaker::_checkFavoritePlayers(const MoHRS::Game& game, const std::vector<std::string>& fav_players, int& num_fav_players) const
{
	for(const MoHRS::Player& player : game.GetPlayers())
	{
		std::string player_name = player.GetName();

		for(const std::string& fav_player : fav_players)
		{
			if(player_name.find(fav_player) != std::string::npos)
			{
				num_fav_players++;
				break;
			}
		}
	}
}