#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
//#include "GameThread.h"
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>


class ServerThread : public QThread
{
	Q_OBJECT

public:
	explicit ServerThread(int ID, QObject * parent = 0);
	void run();
	void delay(int);
	//GameThread *gThread;
signals:
	void error(QTcpSocket::SocketError error);
	//void sendMessageSig();
	public slots :
	void readyRead();
	void disconnected();
	void sendMessage();
	void updateNumberClients(int num);

	private slots :

private:
	QTcpSocket *socket;
	int socketDescriptor;
	int numberOfClients = 0;
};

#endif // SERVERTHREAD_H