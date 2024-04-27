#include <util.h>

#include <mohrs/game.h>

MoHRS::Game::Game()
{

}

MoHRS::Game::~Game()
{
	
}

bool MoHRS::Game::SetId(int id)
{
	this->_id = id;
	return true;
}

bool MoHRS::Game::SetId(const std::string& str_id)
{
	try
	{
		int id = std::stoi(str_id);
		
		return this->SetId(id);
	}
	catch(...) {};
	
	return false;
}

bool MoHRS::Game::SetIp(const std::string& ip)
{
	if(ip.find("10.10.10.") == 0)
		this->_ip = "86.87.139.235";
	else
		this->_ip = ip;
	
	return true;
}

bool MoHRS::Game::SetName(const std::string& name)
{
	this->_name = Util::removeQuote(name);
	return true;
}

bool MoHRS::Game::SetRegion(const MoHRS::Regions region)
{
	this->_region = region;
	return true;
}

bool MoHRS::Game::SetRegion(int8_t int_region)
{
	if(int_region >= static_cast<uint8_t>(MoHRS::Regions::Europe) && int_region <= static_cast<uint8_t>(MoHRS::Regions::Mars))
	{
		MoHRS::Regions region = static_cast<MoHRS::Regions>(int_region);
		
		return this->SetRegion(region);
	}
}

bool MoHRS::Game::SetRegion(const std::string& str_region)
{
	try
	{
		uint8_t int_region = static_cast<uint8_t>(std::stoul(str_region));
		
		return this->SetRegion(int_region);
	}
	catch(...) {};
	
	return false;
}

bool MoHRS::Game::SetNumPlayers(uint8_t num_players)
{
	this->_num_players = num_players;
	return true;
}

bool MoHRS::Game::SetNumPlayers(const std::string& str_num_players)
{
	try
	{
		uint8_t num_players = static_cast<uint8_t>(std::stoul(str_num_players));
		
		return this->SetNumPlayers(num_players);
	}
	catch(...) {};
	
	return false;
}

bool MoHRS::Game::SetMaxPlayers(uint8_t max_players)
{
	this->_max_players = max_players;
	return true;
}

bool MoHRS::Game::SetMaxPlayers(const std::string& str_max_players)
{
	try
	{
		uint8_t max_players = static_cast<uint8_t>(std::stoul(str_max_players));
		
		return this->SetMaxPlayers(max_players);
	}
	catch(...) {};
	
	return false;
}

bool MoHRS::Game::SetHostPlayer(const std::string& host_player)
{
	this->_host_player = Util::removeQuote(host_player);
	return true;
}

bool MoHRS::Game::SetTheaterSession(const std::string& theater_session)
{
	this->_theater_session = theater_session;
	return true;
}

bool MoHRS::Game::SetPlayers(const MoHRS::Players& players)
{
	this->_players = players;
	return true;
}


bool MoHRS::Game::AddPlayer(const MoHRS::Player& player)
{
	this->_players.push_back(player);

	return true;
}

