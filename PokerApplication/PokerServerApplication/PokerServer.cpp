#include "PokerServer.h"
#include <QTime>
#include <QCoreApplication>

PokerServer::PokerServer(QObject * parent) : QTcpServer(parent)
{


}



void PokerServer::StartServer()
{

	if (!this->listen(QHostAddress::Any, 1234))
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
	connect(this, SIGNAL(updateCurrentPlayer(int)), thread, SLOT(updateCurrentPlayer(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(changeGameStage(int)), thread, SLOT(changeGameStage(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateBetMade()), thread, SLOT(updateBetMade()), Qt::QueuedConnection);
	connect(this, SIGNAL(updateRaiseMade(int,int)), thread, SLOT(updateRaiseMade(int, int)), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnBet()), this, SLOT(incrementCurrentPlayer()), Qt::QueuedConnection); // signal commes from socketThread noteToSelf: this solves my issue of communicating back from the thread
	connect(thread, SIGNAL(notifyOnBet()), this, SLOT(detectBetWasMade()), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnRaise(int,int)), this, SLOT(detectRaiseWasMade(int,int)), Qt::QueuedConnection);
	qDebug() << "main" << QThread::currentThreadId();
	thread->start();
	listOfPlayers.append(new Player(socketDescriptor, 500));
	numberOfClients++;
	if (numberOfClients == 1)
	{
		currentPlayer = socketDescriptor;
		qDebug() << "First player is:" << currentPlayer;
	}
	if (numberOfClients > 0)
	{
		qDebug() << "Emmiting signal from poker server";

		emit updateNoClients(numberOfClients);
		emit updateCurrentPlayer(currentPlayer);
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

void PokerServer::incrementCurrentPlayer()
{
	globalI++;
	if (globalI >= numberOfClients)
	{
		globalI = 0;
		globalGameStage++;
	}
	currentPlayer = listOfPlayers[globalI]->getSocketDescriptor();
	emit updateCurrentPlayer(currentPlayer);
	if (globalI == 0)
	{
		emit changeGameStage(globalGameStage);
	}
}

void PokerServer::detectRaiseWasMade(int socketDescriptor, int amountRaised)
{
	for (int i = 0; i < listOfPlayers.length; i++)
	{
		if (listOfPlayers[i]->getSocketDescriptor() == socketDescriptor)
		{
			listOfPlayers[i]->setCurrentBet(amountRaised);
		}
	}
	
	emit updateRaiseMade(socketDescriptor, amountRaised);

}
void PokerServer::detectBetWasMade()
{
	emit updateBetMade();
}
