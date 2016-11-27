#ifndef POKERAPPLICATION_H
#define POKERAPPLICATION_H

#include <QtWidgets/QMainWindow>
#include <QtSql>
#include <QTcpSocket>

class PokerApplication : public QObject
{
	Q_OBJECT
	Q_INVOKABLE

signals :
	void error(QTcpSocket::SocketError error);
public slots :
	void readyRead();
	void disconnected();


public:
	PokerApplication();
	~PokerApplication();
	Q_INVOKABLE bool ConnectToDb();
	Q_INVOKABLE void webChannelTest(QString text); //Web channel test used for debugging, works a bit like qDebug for js code
	Q_INVOKABLE int webChannelTest();
	Q_INVOKABLE bool checkUserAndPassword(QString usr, QString psd); //check for user name and password called form js
	Q_INVOKABLE bool joinTable(); //try to join table called from js

private:
	Q_INVOKABLE bool AcquireConnectionDb(); //acquires connection to db
	Q_INVOKABLE bool validateUserAndPassword(QString usr, QString psd); //validates user+password form db
	Q_INVOKABLE bool connectToServer(); //try to connect to server

	QSqlDatabase db;
	QString currentUsername = "";
	bool isUserLoggedIn = false;
	QTcpSocket *socket;
	int myClientNumber = 0; //thread number from server
	bool isGameStarted = false; //will be received from server 
	bool isMyTurn = false; //will be received from server 
	int numberOfPlayers = 0; //will be recieved from server
};

#endif // POKERAPPLICATION_H
