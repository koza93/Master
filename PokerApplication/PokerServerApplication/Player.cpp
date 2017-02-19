#include "Player.h"
#include <QDebug>

Player::Player()
{

}

Player::Player(int socketDesc, int chips)
{
	setSocketDescriptor(socketDesc);
	setTotalChips(chips);
}

Player::~Player()
{

}

void Player::setSocketDescriptor(int socketDesc)
{
	socketDescriptor = socketDesc;
}

void Player::setTotalChips(int chips)
{
	totalChips = chips;
}

void Player::setCurrentBet(int bet)
{
	currentBet = bet;
}

void Player::setAsDealer(bool d)
{
	dealer = d;
}
void Player::setAsBigBlind(bool b)
{
	bigBlind = b;
}

int Player::getSocketDescriptor()
{
	return socketDescriptor;
}

int Player::getTotalChips()
{
	return totalChips;
}

int Player::getCurrentBet()
{
	return currentBet;
}

bool Player::isDealer()
{
	return dealer;
}

bool Player::isBigBlind()
{
	return bigBlind;
}