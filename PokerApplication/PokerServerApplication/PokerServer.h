#ifndef POKERSERVER_H
#define POKERSERVER_H
#include "ServerThread.h"
#include <QThread>
#include <QDebug>
#include <QTcpServer>
#include <QFuture>
#include <QTConcurrent/QtConcurrentRun>
#include <QList>
class PokerServer : public QTcpServer
{
	Q_OBJECT

public:
	ServerThread *thread;
	//QList<ServerThread*> clientThreads;
	explicit PokerServer(QObject * parent = 0);
	void StartServer();
	void delay(int);
	int currentPlayer = 0;									//thread id  of current player
	QList<int> listOfPlayers;
signals:
	void updateBetMade();									//notify socket threads bet was made
	void updateRaiseMade(int, int);							//notify socket threads a raise was made (who_made_the_raise,amount_raised)
	void updateCurrentPlayer(int num);						//notify socket threads of current player
	void updateNoClients(int num);							//notify socket threads of number of clients
	void changeGameStage(int num);							//notify socket that game stage changed ie pre flop to flop. 0 for pre flop, 1 for flop ....to change from pre flop to flop send 1
	//void mSignal();
public slots :
	void incrementCurrentPlayer();							//go to next player on the list
	void detectBetWasMade();								//if a socket detects a bet it notifies the pokerserver and then poker server notifies all threads(sockets)
	void detectRaiseWasMade(int,int);						//if asocket detects a raise i notifies who and how much raised
	//void mySlot();
	private slots :

private:
	int numberOfClients = 0;							
	int globalI = 0;										//used to increment through Currentplayer list
	int globalGameStage = 0;								//determines stage of game {0 pre flop, 1 flop ....} to change from pre flop to flop send 1
	int bigBlind = 0;
	int totalChips = 0;										//perperson
	int totalPot = 0;										//totalPot for a game

protected:
	void incomingConnection(qintptr  socketDescriptor);

};

#endif // POKERSERVER_H