
#include <QtCore/QCoreApplication>
#include "pokerapplication.h"
#include <QtWidgets/QApplication>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <qtwebenginewidgetsglobal.h>
#include <QDebug>


int main(int argc, char *argv[])
{
	PokerApplication pokerOperations;
	QApplication a(argc, argv);
	QWebEngineView *view = new QWebEngineView();
	QWebEnginePage *page = view->page();
	QWebChannel *webChannel = new QWebChannel(page);
	QUrl baseUrl = QUrl::fromLocalFile("C:/CollegeProject/PokerApplication/HTML/Example.html");
	//view->load(baseUrl);

	//atempting database connection
	while (!pokerOperations.ConnectToDb())
	{
		qDebug() << "cannot connect to db";
	}
	qDebug() << "connected to db";

	//register pokerOperations with webChannel
	view->page()->setWebChannel(webChannel);
	webChannel->registerObject("pokerOperations", &pokerOperations);

	//load html
	view->load(QUrl(baseUrl));
	view->show();
	qDebug() << "loloo";
	//app.show();
	
	return a.exec();
}
