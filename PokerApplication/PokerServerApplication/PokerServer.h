#ifndef POKERSERVER_H
#define POKERSERVER_H
#include "ServerThread.h"
#include <QThread>
#include <QDebug>
#include <QTcpServer>
class PokerServer : public QTcpServer
{
	Q_OBJECT

public:
	explicit PokerServer(QObject * parent = 0);
	void StartServer();

signals:
	
public slots :

private slots :

private:

protected:
	void incomingConnection(qintptr  socketDescriptor);
};

#endif // POKERSERVER_H