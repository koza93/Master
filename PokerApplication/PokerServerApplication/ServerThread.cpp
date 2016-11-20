#include "ServerThread.h"

ServerThread::ServerThread(int ID, QObject * parent) :QThread(parent)	
{
	this->socketDescriptor = ID;
}

void ServerThread::run()
{
	qDebug() << "Starting new thread:" << this->socketDescriptor;
	socket = new QTcpSocket();
	if (!socket->setSocketDescriptor(this->socketDescriptor))
	{
		emit error(socket->error());
		return;
	}

	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

	qDebug() << "Client connected:" << this->socketDescriptor;

	exec();
}

void ServerThread::readyRead()
{
	QByteArray Data = socket->readAll();
	qDebug() << "I am sending:" << Data << "  ME="<<this->socketDescriptor;
	socket->write(Data);

}

void ServerThread::disconnected()
{
	qDebug() << "Client disconnected:" << this->socketDescriptor;
	socket->deleteLater();
	exit(0);
}