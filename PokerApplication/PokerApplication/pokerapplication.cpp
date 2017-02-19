#include "pokerapplication.h"
#include <QDebug>

PokerApplication::PokerApplication()
{
	qDebug() << "PokerObject Created";
}

PokerApplication::~PokerApplication()
{
	socket->close();
}

bool PokerApplication::ConnectToDb() 
{
	qDebug() << "Attempting db connection";
	if (AcquireConnectionDb()) {

		return true;
	}
	return false;
}

bool PokerApplication::AcquireConnectionDb() 
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("127.0.0.1");
	db.setPort(3306);
	//db.setDatabaseName("DRIVER = { Microsoft Access Driver(*.mdb) }; FIL = { MS Access }; DBQ = myaccessfile.mdb");
	db.setUserName("root");
	db.setPassword("root");
	bool ok = db.open();
	if (ok == true)
		return true;
	return false;
}
Q_INVOKABLE bool PokerApplication::checkUserAndPassword(QString usr, QString psd)
{
	if (validateUserAndPassword(usr,psd)) {
		return true;
	}
	return false;
}

Q_INVOKABLE bool PokerApplication::validateUserAndPassword(QString usr, QString psd)
{
	QSqlQuery query;
	if (query.exec("SELECT * FROM poker_database.users WHERE user = '"+ usr +"' and password = '"+ psd +" '"))
	{
		while (query.next())
		{
			qDebug() << "valid user name and password for user:" +query.value(1).toString();
			isUserLoggedIn = true;
			currentUsername = usr;
			return true;
		}
		qDebug() << "invalid user name and password";
		return false;
		
	}
	else
	{
		qDebug() << "invalid user name and password";
	}
	return false;
}

Q_INVOKABLE bool PokerApplication::joinTable()
{
	if (connectToServer()) {
		qDebug() << "connected to server";
		return true;
	}

	return false;
}

Q_INVOKABLE bool PokerApplication::connectToServer()
{
	socket = new QTcpSocket(this);

	socket->connectToHost("localhost", 1234);
	if (socket->waitForConnected(2000))
	{
		qDebug() << "Connected to host";
		socket->write("WhoAmI");
		connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
		connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
		return true;
	}
	else
	{
		qDebug() << "cannot connect to host";
	}

	
	return false;
}

void PokerApplication::webChannelTest(QString text)
{
	qDebug() << "The text i receive is:" << text;
}
int PokerApplication::webChannelTest()
{
	return 10;
}

void PokerApplication::readyRead()
{	
	QByteArray Data = socket->readAll();
	QString dataString = Data;
	qDebug() << "Message from server is: " + Data;
	QStringList dataList = dataString.split(':');
	if (dataList.length() > 1)
	{
		if (dataList[0] == "Thread")
		{
			myClientNumber = dataList[1].toInt();
			qDebug() <<myClientNumber;
		}	
		if (dataList[0] == "ChangeTurn")
		{
			if (dataList[1] == QString::number(myClientNumber)) {
				isMyTurn = true;
				notifyOnTurn(isMyTurn);
				qDebug("My turn");
			}
			else {
				isMyTurn = false;
				notifyOnTurn(isMyTurn);
				qDebug("Not my turn");
			}
		}
	}	
	if (dataList.length() > 2) {
		if (dataList[0] == "GameStarted") {
			qDebug() << "GameStarted";
			isGameStarted = true;
			numberOfPlayers = dataList[1].toInt(); //note to self: first update numofplayers then notify about game
			notifyOnGameStarted(isGameStarted);
			if (dataList[2] == QString::number(myClientNumber)) {
				isMyTurn = true;
				notifyOnTurn(isMyTurn);
				qDebug("My turn");
			}
			else {
				isMyTurn = false;
				qDebug("Not my turn");
			}
		}
	}
	
}

void PokerApplication::disconnected()
{
	qDebug() << "Client disconnected:" ;
	socket->deleteLater();
	exit(0);
}
int PokerApplication::getNumberOfPlayers()
{
	return numberOfPlayers;
}

void PokerApplication::sendCheckButtonClicked() {
	QString msg = "Check:"+ QString::number(myClientNumber);
	QByteArray message = msg.toStdString().c_str();
	qDebug() << msg;
	socket->write(message);
}

void PokerApplication::sendRaiseButtonClicked() {
	QString msg = "Raise:" + QString::number(myClientNumber) +":50" ;   //note hardcoded 50 raise for now - change when i have actual gui code written
	QByteArray message = msg.toStdString().c_str();
	qDebug() << msg;
	socket->write(message);
}

void PokerApplication::sendFoldButtonClicked() {
	QString msg = "Fold:" + QString::number(myClientNumber);
	QByteArray message = msg.toStdString().c_str();
	socket->write(message);
}

void PokerApplication::callFoldButtonClicked() {
	QString msg = "Call:" + QString::number(myClientNumber);
	QByteArray message = msg.toStdString().c_str();
	socket->write(message);
}