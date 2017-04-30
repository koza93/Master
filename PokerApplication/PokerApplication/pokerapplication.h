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
	void notifyOnTurn(bool);											//notify js
	void notifyOnGameStarted(bool);										//notify js
	void notifyOnCanCheck(bool);										//notify js
	void notifyOnHandDealt(bool);										//notify js
	void notifyOnFlopDealt(bool);										//notify js
	void notifyOnTurnDealt(bool);										//notify js
	void notifyOnRiverDealt(bool);										//notify js
	void notifyOnRefreshDealt(bool);									//notify js
	void notifyOnAllPlayersConnected(int,int);							//notify js
	void notifyOnAssignId(bool);										//notify js

	void notifyOnUpdate(int, int, int, int);							//notify js
	void notifyOnBet(int, int, int, int);								//notify js
	void notifyOnWin(int);												//notify js
	void notifyOnDraw(int flag, QString drw);
	void notifyOnEndGame(int);											//notify js

	void error(QTcpSocket::SocketError error);
public slots :
	void readyRead();
	void disconnected();
	void closing();


public:
	PokerApplication();
	~PokerApplication();
	Q_INVOKABLE bool ConnectToDb();
	Q_INVOKABLE void webChannelTest(QString text);					    //Web channel test used for debugging, works a bit like qDebug for js code
	Q_INVOKABLE int webChannelTest();
	Q_INVOKABLE bool checkUserAndPassword(QString usr, QString psd);    //check for user name and password called form js
	Q_INVOKABLE bool joinTable();										//try to join table called from js
	Q_INVOKABLE int getNumberOfPlayers();								//return number of players for js
	Q_INVOKABLE int getPlayerNumber();									//return player number for js thread id
	Q_INVOKABLE int* getAllPlayerNumbers();						//return all player numbers to js actual thread ids
	Q_INVOKABLE QString getHandCards(int n);							//return player number for js
	Q_INVOKABLE QString getTableCards(int n);							//return player number for js
	Q_INVOKABLE void sendCheckButtonClicked();							//send a message to server notifying of check button clicked
	Q_INVOKABLE void sendRaiseButtonClicked(int);							//send a message to server notifying of raise button clicked
	Q_INVOKABLE void sendFoldButtonClicked();							//send a message to server notifying of fold button clicked
	Q_INVOKABLE void sendCallButtonClicked();							//send a message to server notifying of call button clicked
	Q_INVOKABLE void delay(int);

private:
	Q_INVOKABLE bool AcquireConnectionDb();								//acquires connection to db
	Q_INVOKABLE bool validateUserAndPassword(QString usr, QString psd); //validates user+password form db
	Q_INVOKABLE bool connectToServer();									//try to connect to server

	QSqlDatabase db;
	QString currentUsername = "";
	bool isUserLoggedIn = false;
	QTcpSocket *socket;
	int myClientNumber = 0;												//thread number from server
	QVector<int>  allPlayerNumbers;									//vector containing 
	bool isGameStarted = false;											//will be received from server 
	bool isMyTurn = false;												//will be received from server 
	int numberOfPlayers = 0;											//will be recieved from server
	int myTotalChips = 0;
	//int totalPot = 0;
	int currentBet = 0;

	QString myCards[2];												//String values for cards on hand
	QString cardsOnTable[5];											//String values for cards on table
};

#endif // POKERAPPLICATION_H
