#include <util.h>

#include <mohrs/player.h>

MoHRS::Player::Player()
{
    
}

MoHRS::Player::~Player()
{
	
}

bool MoHRS::Player::SetName(const std::string& name)
{
    this->_name = Util::removeQuote(name);
	return true;
}

bool MoHRS::Player::SetTicket(const std::string& ticket)
{
    this->_ticket = Util::removeQuote(ticket);
	return true;
}

