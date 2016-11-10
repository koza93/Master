#ifndef POKERAPPLICATION_H
#define POKERAPPLICATION_H

#include <QtWidgets/QMainWindow>
#include <QtSql>
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

private:
	Q_INVOKABLE bool AcquireConnectionDb();
	Q_INVOKABLE bool validateUserAndPassword(QString usr, QString psd);

	QSqlDatabase db;
	QString currentUsername = "";

};

#endif // POKERAPPLICATION_H
