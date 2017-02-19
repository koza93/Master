#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QMainWindow>
#include <QDebug>

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

		int getSocketDescriptor();
		int getTotalChips();
		int getCurrentBet();	
		bool isDealer();
		bool isBigBlind();

		
		
	signals:
		public slots :
		private slots:

	private:
		int socketDescriptor = 0;
		int totalChips = 0;
		int currentBet = 0;
		bool dealer = false;
		bool bigBlind = false;
};

#endif // PLAYER_H