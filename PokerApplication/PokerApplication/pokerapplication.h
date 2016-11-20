#ifndef POKERAPPLICATION_H
#define POKERAPPLICATION_H

#include <QtWidgets/QMainWindow>
#include <QtSql>
#include <QTcpSocket>

class PokerApplication : public QObject
{
	Q_OBJECT
	Q_INVOKABLE
public:
	PokerApplication();
	~PokerApplication();
	Q_INVOKABLE bool ConnectToDb();
	Q_INVOKABLE void webChannelTest(QString text);
	Q_INVOKABLE int webChannelTest();
	Q_INVOKABLE bool checkUserAndPassword(QString usr, QString psd);
	Q_INVOKABLE bool joinTable();

private:
	Q_INVOKABLE bool AcquireConnectionDb();
	Q_INVOKABLE bool validateUserAndPassword(QString usr, QString psd);
	Q_INVOKABLE bool connectToServer();

	QSqlDatabase db;
	QString currentUsername = "";
	bool isUserLoggedIn = false;
	QTcpSocket *socket;

};

#endif // POKERAPPLICATION_H
