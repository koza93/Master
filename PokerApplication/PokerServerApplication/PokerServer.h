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
	void updateCurrentPlayer(int num);						//notify socket threads of current player
	void updateNoClients(int num);							//notify socket threads of number of clients
	//void mSignal();
public slots :
	void incrementCurrentPlayer();							//go to next player on the list
	void detectBetWasMade();								//if a socket detects a bet it notifies the pokerserver and then poker server notifies all threads(sockets)
	//void mySlot();
	private slots :

private:
	int numberOfClients = 0;							
	int globalI = 0;										//used to increment through Currentplayer list
protected:
	void incomingConnection(qintptr  socketDescriptor);

};

#endif // POKERSERVER_H