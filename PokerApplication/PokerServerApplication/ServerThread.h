#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
//#include "GameThread.h"
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QFuture>
#include <QtConcurrent>
#include "Card.h"

class ServerThread : public QThread
{
	Q_OBJECT

public:
	explicit ServerThread(int ID, QObject * parent = 0);
	void run();
	void delay(int);
	void checkInputsFromServer();
	//void aFunction();
	//GameThread *gThread;
signals:
	void doSignal();								//semi-dummy signal to invoke messages (hopegully will solve problem with calling functions)	
	void notifyOnBet();								//notifies pokers server that bet was made so it increments current player and emits it back
	void notifyOnCheck(int);						//notifies pokers server that a check was made (int socket descriptor of the player)
	void notifyOnRaise(int,int);					//notifies pokers server that raise was made (int socketdescriptor of the player, int amount raised)
	void notifyOnCall(int);							//notifies pokers server that call wa made (int socket descriptor of the player)
	void notifyOnFold(int);							//notifies pokers server that a fold was made (int socket descriptor of the player)
	void error(QTcpSocket::SocketError error);
	//void sendMessageSig();
	public slots :
	void getSignal();								//semi-dummy slot to invoke messages (hopegully will solve problem with calling functions)	
	void readyRead();
	void disconnected();
	void sendMessage(QString msg);
	void updateBetMade(bool);						//can check 1, can call 0
	void updateRaiseMade(int, int);
	void updateCallMade(int, int);
	void updateCheckMade(int);
	void updateFoldMade(int);
	void updateNumberClients(int num);
	void updateCurrentPlayer(int num);
	void updateAllPlayers(QVector<int>, int bb, int sb);		//vector containing thread numbers of all players , thread number of big blind, thread number of small blind
	void updateNoOfPlayersToStartGame(int num);		//updates number of players needed to start a game
	void changeGameStage(int gameStage, QVector<int>);

	void updateMyCurrentHand(Card*, Card*,int);		//updates current hand if cards are dealt for this thread
	void updateCardsOnTable(Card**);				//update current card on table if dealt

	private slots :

private:
	QTcpSocket *socket;
	int socketDescriptor;
	int numberOfClients = 0;
	int currentBB = 0;								//thread number of current Big Blind
	int currentSB = 0;								//thread number of current Small Blind
	int bigBlind = 50;								//amount of chips for bb TODO code to actually update this value from the main server thread
	int smallBlind = 25;
	int currentPlayer = 0;
	int previousPlayer = 0;
	int betAmount = 0;
	int allBetAmount = 0;							//bet amount used by all threads to read the total bet amoutn from the server
	int numberOfPlayersToStart = 10;				//some reasonable number that will be changed by a lower value
	QVector<int>  allPlayerNumbers;
	QVector<int>  playersToUpdate;					//array of players that need to updated at a time

	bool betMade = false;
	bool betRaised = false;
	bool betCalled = false;
	bool betFolded = false;
	bool betChecked = false;
	bool canCheck = false;
	bool isGameFinished = false;
	bool isPreFlopFinished = false;
	bool isFlopFinished = false;
	bool isTurnFinished = false;
	bool isRiverFinished = false;

	bool handDealt = false;
	bool flopDealt = false;
	bool turnDealt = false;
	bool riverDealt = false;
	bool refreshDealt = false;

	bool allPlayersUpdated = false;

	Card* myCurrentCards[2];
	Card* cardsOnTable[5];
	
};

#endif // SERVERTHREAD_H