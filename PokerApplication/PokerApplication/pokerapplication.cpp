#include "pokerapplication.h"
#include <QDebug>

PokerApplication::PokerApplication()
{
	qDebug() << "PokerObject Created";
}

PokerApplication::~PokerApplication()
{

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
	if (query.exec(""))
	{

	}
	else
	{

	}
	qDebug() << "attempted u+p validation";
}
void PokerApplication::webChannelTest(QString text)
{
	qDebug() << "The text i receive is:" << text;
}
int PokerApplication::webChannelTest()
{
	return 10;
}