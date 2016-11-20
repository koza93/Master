#include "PokerServer.h"

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
	}
}

void PokerServer::incomingConnection(qintptr  socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	ServerThread *thread = new ServerThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();
}