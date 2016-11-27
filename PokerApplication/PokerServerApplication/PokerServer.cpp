#include "PokerServer.h"
#include <QTime>
#include <QCoreApplication>

PokerServer::PokerServer(QObject * parent) : QTcpServer(parent)
{

	
}



void PokerServer::StartServer()
{

	if (!this->listen(QHostAddress::Any,1234))
	{
		qDebug() << "Failed to start server";
	}
	else
	{
		qDebug() << "Server started, waiting for connection...";
		PokerServer s;
		
	}
	/*
	gameThread = new GameThread(numberOfClients, this);
	connect(gameThread, SIGNAL(finished()), gameThread, SLOT(deleteLater()));
	gameThread->start();*/
}

void PokerServer::incomingConnection(qintptr  socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	ServerThread *thread = new ServerThread(socketDescriptor, this);
	//clientThreads.append(thread);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this, SIGNAL(updateNoClients(int)), thread, SLOT(updateNumberClients(int)), Qt::QueuedConnection);
	//connect(this, SIGNAL(clientsConnected()), thread, SLOT(sendMessage()));
	//connect(gameThread, SIGNAL(clientsConnected()), thread, SLOT(sendMessage()), Qt::QueuedConnection);
	thread->start();
	numberOfClients++;
	if (numberOfClients > 0)
	{
		qDebug() << "Emmiting signal from poker server";
	
		emit updateNoClients(numberOfClients);
	}
	
	//emit updateNoClients(numberOfClients);
	qDebug() << "Number of clients: " << numberOfClients;
}


void PokerServer::delay(int millisecondsToWait)
{
	QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
	while (QTime::currentTime() < dieTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
