#include "Deck.h"
#include <QDebug>

Deck::Deck()
{
	initializeDeck();
}


Deck::~Deck()
{

}

void Deck::initializeDeck()
{
	char suit;
	char figure;
	for (int i = 0; i < numOfCards; i++)
	{
		if (i == 8 || i == 21 || i == 34 || i == 47)
			figure = 'X';
		if (i == 9 || i == 22 || i == 35 || i == 48)
			figure = 'J';
		if (i == 10 || i == 23 || i == 36 || i == 49)
			figure = 'Q';
		if (i == 11 || i == 24 || i == 37 || i == 50)
			figure = 'K';
		if (i == 12 || i == 25 || i == 38 || i == 51)
			figure = 'A';

		if (i < 13)
		{
			suit = 'H';
			if (i < 8)
				figure = (i+2) + '0';		
		}
		if (i >= 13 && i < 26)
		{
			suit = 'C';
			if (i < 21)
				figure = (i-11) + '0';
		}

		if (i >= 26 && i < 39)
		{
			suit = 'D';
			if (i < 34)
				figure = (i - 24) + '0';
		}

		if (i >= 39 && i < 52)
		{
			suit = 'S';
			if (i < 47)
				figure = (i - 37) + '0';
		}
		//qDebug() << suit << figure;
		Card* theCard = new Card(suit, figure);
		theDeck.append(theCard);
	}
}

void Deck::displayDeckTest()
{
	for (int i = 0; i < numOfCards; i++)
	{
		qDebug() << theDeck.at(i)->getFigure() << " "<< theDeck.at(i)->getSuit();
	}
}

Card* Deck::getCardFromDeck(int i)
{
	return theDeck.at(i);
}