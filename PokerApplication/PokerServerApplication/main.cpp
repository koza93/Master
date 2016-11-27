
#include "PokerServer.h"
#include <QtCore/QCoreApplication>
#include <QtNetwork>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	PokerServer server;
	server.StartServer();
	
	return a.exec();
}
