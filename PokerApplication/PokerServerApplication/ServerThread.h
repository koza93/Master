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
	void notifyOnRaise(int,int);
	void error(QTcpSocket::SocketError error);
	//void sendMessageSig();
	public slots :
	void getSignal();								//semi-dummy slot to invoke messages (hopegully will solve problem with calling functions)	
	void readyRead();
	void disconnected();
	void sendMessage(QString msg);
	void updateBetMade();
	void updateRaiseMade(int, int);
	void updateNumberClients(int num);
	void updateCurrentPlayer(int num);
	void changeGameStage(int gameStage);

	private slots :

private:
	QTcpSocket *socket;
	int socketDescriptor;
	int numberOfClients = 0;
	int currentPlayer = 0;
	int previousPlayer = 0;
	int betAmount = 0;
	bool betMade = false;
	bool betRaised = false;
	bool betFolded = false;
	bool isGameFinished = false;
	bool isPreFlopFinished = false;
	bool isFlopFinished = false;
	bool isTurnFinished = false;
	bool isRiverFinished = false;
	
};

#endif // SERVERTHREAD_H