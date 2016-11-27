#include "ServerThread.h"


ServerThread::ServerThread(int ID, QObject * parent) :QThread(parent)
{

	this->socketDescriptor = ID;
	this->setParent(parent);
	

	//create thread instead of concurrent thread?
	//gThread = gameThread;
	
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
	connect(this, SIGNAL(sendMessage()), this, SLOT(sendMessageSig()), Qt::DirectConnection);
		
	qDebug() << "Client connected:" << this->socketDescriptor;
	
	exec();
}

void ServerThread::readyRead()
{
	QByteArray Data = socket->readAll();
	qDebug() << "I am receiving:" << Data << "  ME="<<this->socketDescriptor;
	
	QByteArray threadNumber = QByteArray::number(this->socketDescriptor, 10);
	if (Data == "WhoAmI") {
		socket->write("Thread:" + threadNumber);
	}
	else
	socket->write("Recived:" + Data);

}

void ServerThread::disconnected()
{
	qDebug() << "Client disconnected:" << this->socketDescriptor;
	socket->deleteLater();
	exit(0);
}

void ServerThread::sendMessage() {
	qDebug() << "Message from: " << this->socketDescriptor << ";;" << socket->socketDescriptor();
	//socket->setParent(this);
	QByteArray Data = socket->readAll();
	socket->write("Hello:" + Data);
	
}
void ServerThread::updateNumberClients(int num) {
	numberOfClients = num;
	qDebug() << "Thread:" << this->socketDescriptor << "Number of clients: " << numberOfClients;
	if (numberOfClients > 0)
	{
		emit sendMessage();
	}
}
void ServerThread::sendMessageSig() {

}