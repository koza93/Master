#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "Card.h"

class Player : public QObject
{
	Q_OBJECT
	Q_INVOKABLE

	public:
		Player();
		Player(int, int);
		~Player();

		void setSocketDescriptor(int);
		void setTotalChips(int);
		void setCurrentBet(int);
		void setAsDealer(bool);					//set the attribute dealer to true or false - only one dealer at a time
		void setAsBigBlind(bool);					//set the attribute dealer to true or false - only one bigblind at a time
		void setAsSmallBlind(bool);					//set the attribute dealer to true or false - only one sblind at a time
		void setAsFolded(bool);					//set the attribute hasFolded to true or false 
		void setAsBet(bool);					//set the attribute hasBet to true or false 
		void setBestCards(QString);				//sets the best cards of the player for current hand
		void setInGame(bool);

		int getSocketDescriptor();
		int getTotalChips();
		int getCurrentBet();	
		QString getBestCards();					// return best cards of player for current game
		bool isDealer();
		bool isBigBlind();
		bool isSmallBlind();
		bool isFolded();
		bool isBet();
		bool isInGame();

		void setMyCurrentHand(Card*, int);		//(theCard, the position in array) //position must be less than 2
		Card* getMyCards(int);					//(position in array) //must be less than 2

		
		
	signals:
		public slots :
		private slots:

	private:
		int socketDescriptor = 0;
		int totalChips = 500;					//TODO set the total number of chips from the server
		int currentBet = 0;
		bool dealer = false;
		bool bigBlind = false;
		bool smallBlind = false;
		bool hasFolded = false;
		bool hasBet = false;
		bool inGame = true;

		Card* myCurrentHand[2];
		QString myBestCards;
};

#endif // PLAYER_H

