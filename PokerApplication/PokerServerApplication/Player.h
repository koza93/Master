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

		int getSocketDescriptor();
		int getTotalChips();
		int getCurrentBet();
		

		
	signals:
		public slots :
		private slots:

	private:
		int socketDescriptor = 0;
		int totalChips = 0;
		int currentBet = 0;
};

#endif // PLAYER_H