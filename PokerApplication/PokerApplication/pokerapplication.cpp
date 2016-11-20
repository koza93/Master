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