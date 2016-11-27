#ifndef POKERSERVER_H
#define POKERSERVER_H
#include "ServerThread.h"
#include <QThread>
#include <QDebug>
#include <QTcpServer>
#include <QFuture>
#include <QTConcurrent/QtConcurrentRun>
class PokerServer : public QTcpServer
{
	Q_OBJECT

public:
	ServerThread *thread;
	//QList<ServerThread*> clientThreads;
	explicit PokerServer(QObject * parent = 0);
	void StartServer();
	void delay(int);
signals:
	void updateNoClients(int num);
	//void someSignal();
	//void mSignal();
	public slots :
	//void mySlot();
	private slots :

private:
	int numberOfClients = 0;
protected:
	void incomingConnection(qintptr  socketDescriptor);

};

#endif // POKERSERVER_H