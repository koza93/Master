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
	qDebug() << "run" << QThread::currentThreadId();
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(this, SIGNAL(doSignal()), this, SLOT(getSignal()), Qt::DirectConnection);
	
	//connect(this, SIGNAL(sendMessageSig()), this, SLOT(sendMessage()), Qt::DirectConnection);

	qDebug() << "Client connected:" << this->socketDescriptor;

	while (numberOfClients < 3)
	{

	}
	delay(1000);
	
	//Start the Game
	QString StartGameMsg = "GameStarted:"+ QString::number(numberOfClients) + ":" + QString::number(currentPlayer);
	qDebug()<< StartGameMsg;
	//sendMessage(StartGameMsg);
	emit doSignal();
	//while game is on
	delay(1000); //delay necessary to accomodate socket data exchange
	while (!isGameFinished) {
		//while playing flop
		while (!isFlopFinished) {
			delay(500);
			if (betMade == true) {
				betMade = false;
				sendMessage("ChangeTurn:" + QString::number(currentPlayer));
			}
		}

	}

	
	exec();

	
}

void ServerThread::readyRead()
{
	QByteArray Data = socket->readAll();
	qDebug() << "I am receiving:" << Data << "  ME=" << this->socketDescriptor;
	QString dataToString = Data;
	QStringList arrayOfData = dataToString.split(':');
	QByteArray threadNumber = QByteArray::number(this->socketDescriptor, 10);
	if (Data == "WhoAmI") {
		sendMessage("Thread:" + threadNumber);
	}
	if (arrayOfData[0] == "Check")
	{
		//if current thread made bet (problem solved: current thread must have made bet because it the only socket that is allowed to bet in pok app)
		if (arrayOfData[1] == QString::number(this->socketDescriptor))
		{
			emit notifyOnBet();
		}
	}
	/*/
	else
		socket->write("Recived:" + Data);
	qDebug() <<"readready"<< QThread::currentThreadId();
	*/
}

void ServerThread::disconnected()
{
	qDebug() << "Client disconnected:" << this->socketDescriptor;
	socket->deleteLater();
	exit(0);
}

void ServerThread::sendMessage(QString msg) {
	QByteArray threadNumber = QByteArray::number(this->socketDescriptor, 10);
	
	qDebug() << "Message from: " << this->socketDescriptor << ";;" << socket->socketDescriptor() << "The msg: " <<msg;
	//socket->setParent(this);
	QByteArray message = msg.toStdString().c_str();
	//QByteArray Data = socket->readAll();
	socket->write(message);

}
void ServerThread::updateNumberClients(int num) {
	numberOfClients = num;
	qDebug() << "numClients" << QThread::currentThreadId();
	qDebug() << "Thread:" << this->socketDescriptor << "Number of clients: " << numberOfClients;
	//if (numberOfClients > 0)
	//{
	//	//emit sendMessage();
	//}
}
void ServerThread::updateCurrentPlayer(int num) {
	currentPlayer = num;
	qDebug() << "numClients" << QThread::currentThreadId();
	qDebug() << "Thread:" << this->socketDescriptor << "Current player: " << currentPlayer;
	
}

void ServerThread::updateBetMade() {
	betMade = true;
	qDebug() << "Thread:" << this->socketDescriptor << "BetMade ";
}

void ServerThread::delay(int millisecondsToWait)
{
	QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
	while (QTime::currentTime() < dieTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
void ServerThread::getSignal() {	
	sendMessage("GameStarted:"+ QString::number(numberOfClients) + ":" + QString::number(currentPlayer));
}

void ServerThread::aFunction() {
	//while game is on
	while (!isGameFinished) {
		//while playing flop
		while (!isFlopFinished) {
			if (betMade == true) {
				betMade = false;
				sendMessage("ChangeTurn:" + QString::number(currentPlayer));
			}
		}

	}
}