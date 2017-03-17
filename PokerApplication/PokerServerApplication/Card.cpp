#include "Card.h"
#include <QDebug>

Card::Card()
{

}

Card::Card(char s, char f)
{
	setFigure(f);
	setSuit(s);
}

Card::~Card()
{

}

void Card::setFigure(char f)
{
	figure = f;
}

void Card::setSuit(char s)
{
	suit = s;
}

char Card::getSuit()
{
	return suit;
}

char Card::getFigure()
{
	return figure;
}