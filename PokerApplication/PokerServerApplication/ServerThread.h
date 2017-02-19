#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
//#include "GameThread.h"
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QFuture>
#include <QtConcurrent>

class ServerThread : public QThread
{
	Q_OBJECT

public:
	explicit ServerThread(int ID, QObject * parent = 0);
	void run();
	void delay(int);
	//void aFunction();
	//GameThread *gThread;
signals:
	void doSignal();								//semi-dummy signal to invoke messages (hopegully will solve problem with calling functions)	
	void notifyOnBet();								//notifies pokers server that bet was made so it increments current player and emits it back
	void notifyOnCheck(int);						//notifies pokers server that a check was made (int socket descriptor of the player)
	void notifyOnRaise(int,int);					//notifies pokers server that raise was made (int socketdescriptor of the player, int amount raised)
	void notifyOnCall(int);							//notifies pokers server that call wa made (int socket descriptor of the player)
	void error(QTcpSocket::SocketError error);
	//void sendMessageSig();
	public slots :
	void getSignal();								//semi-dummy slot to invoke messages (hopegully will solve problem with calling functions)	
	void readyRead();
	void disconnected();
	void sendMessage(QString msg);
	void updateBetMade();
	void updateRaiseMade(int, int);
	void updateCallMade(int);
	void updateCheckMade(int);
	void updateNumberClients(int num);
	void updateCurrentPlayer(int num);
	void updateNoOfPlayersToStartGame(int num);				//updates number of players needed to start a game
	void changeGameStage(int gameStage);

	private slots :

private:
	QTcpSocket *socket;
	int socketDescriptor;
	int numberOfClients = 0;
	int currentPlayer = 0;
	int previousPlayer = 0;
	int betAmount = 0;
	int numberOfPlayersToStart = 10;				//some reasonable number that will be changed by a lower value
	bool betMade = false;
	bool betRaised = false;
	bool betCalled = false;
	bool betFolded = false;
	bool betChecked = false;
	bool isGameFinished = false;
	bool isPreFlopFinished = false;
	bool isFlopFinished = false;
	bool isTurnFinished = false;
	bool isRiverFinished = false;
	
};

#endif // SERVERTHREAD_H