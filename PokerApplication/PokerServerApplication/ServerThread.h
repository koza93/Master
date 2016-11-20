#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
class ServerThread : public QThread
{
	Q_OBJECT

public:
	explicit ServerThread(int ID, QObject * parent = 0);
	void run();

signals:
	void error(QTcpSocket::SocketError error);
public slots :
	void readyRead();
	void disconnected();
private slots :

private:
	QTcpSocket *socket;
	int socketDescriptor;
};

#endif // SERVERTHREAD_H