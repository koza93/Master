#ifndef CARD_H
#define CARD_H

#include <QtWidgets/QMainWindow>
#include <QDebug>

class Card : public QObject
{
	Q_OBJECT
		Q_INVOKABLE

public:
	Card();
	Card(char, char);
	~Card();

	void setSuit(char);
	void setFigure(char);

	char getSuit();
	char getFigure();

signals:
	public slots :
		private slots :

private:
	char figure;
	char suit;
	
};
#endif //CARD_H