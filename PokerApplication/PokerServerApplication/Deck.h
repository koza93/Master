#ifndef DECK_H
#define DECK_H

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "Card.h"
class Deck : public QObject
{
	Q_OBJECT
		Q_INVOKABLE

public:
	Deck();
	~Deck();

	void initializeDeck();
	void displayDeckTest();
	void shuffleDeck();

	Card* getCardFromDeck(int i);


signals:
	public slots :
		private slots :

private:
	const int numOfCards = 52;
	QList<Card*> theDeck;
};
#endif //DECK_H