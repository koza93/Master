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
void Player::setAsBigBlind(bool bb)
{
	bigBlind = bb;
}

void Player::setAsSmallBlind(bool sb)
{
	smallBlind = sb;
}

void Player::setAsFolded(bool f)
{
	hasFolded = f;
}

void Player::setAsBet(bool b)
{
	hasBet = b;
}

void Player::setBestCards(QString s)
{
	myBestCards = s;
}

void Player::setInGame(bool s)
{
	inGame = s;
}

QString Player::getBestCards()
{
	return myBestCards;
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

bool Player::isInGame()
{
	return inGame;
}

bool Player::isBigBlind()
{
	return bigBlind;
}

bool Player::isSmallBlind()
{
	return smallBlind;
}

bool Player::isFolded()
{
	return hasFolded;
}

bool Player::isBet()
{
	return hasBet;
}

Card* Player::getMyCards(int n)
{
	return myCurrentHand[n];
}

void Player::setMyCurrentHand(Card *c, int num)
{
	if(num < 2)
		myCurrentHand[num] = c;
	
}