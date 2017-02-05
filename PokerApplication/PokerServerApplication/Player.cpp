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