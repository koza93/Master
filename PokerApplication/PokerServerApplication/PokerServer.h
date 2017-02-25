#ifndef POKERSERVER_H
#define POKERSERVER_H
#include "ServerThread.h"
#include "Player.h"
#include <QThread>
#include <QDebug>
#include <QTcpServer>
#include <QFuture>
#include <QTConcurrent/QtConcurrentRun>
#include <QList>
class PokerServer : public QTcpServer
{
	Q_OBJECT

public:
	ServerThread *thread;
	//QList<ServerThread*> clientThreads;
	explicit PokerServer(QObject * parent = 0);
	void StartServer();
	void delay(int);
	int currentPlayer = 0;									//thread id  of current player
	QList<Player*> listOfPlayers;
signals:
	void updateNoOfPlayersToStartGame(int num);				//notify socket threads how many players to start game;
	void updateBetMade();									//notify socket threads bet was made
	void updateRaiseMade(int, int);							//notify socket threads a raise was made (int who_made_the_raise,int amount_raised)
	void updateCallMade(int num);							//notify socket threads a call was made (int who_made_the_call)
	void updateCheckMade(int num);							//notify socket threads a check was made (int who_made_the_call)
	void updateFoldMade(int num);							//notify socket threads of current player fold
	void updateCurrentPlayer(int num);						//notify socket threads of current player
	void updateNoClients(int num);							//notify socket threads of number of clients
	void changeGameStage(int num);							//notify socket that game stage changed ie pre flop to flop. 0 for pre flop, 1 for flop ....to change from pre flop to flop send 1
	//void mSignal();
public slots :
	void incrementCurrentPlayer();							//go to next player on the list
	void detectBetWasMade();								//if a socket detects a bet it notifies the pokerserver and then poker server notifies all threads(sockets)
	void detectRaiseWasMade(int,int);						//if asocket detects a raise it notifies who and how much raised
	void detectCallWasMade(int);							//if asocket detects a call it notifies who called the last bet
	void detectCheckWasMade(int);							//if asocket detects a check it notifies who called the last bet
	void detectFoldWasMade(int);							//if asocket detects a fold it notifies who folded the last bet
	//void mySlot();
	private slots :

private:
	int numberOfClients = 0;							
	int globalI = 0;										//used to increment through Currentplayer list
	int globalGameStage = 0;								//determines stage of game {0 pre flop, 1 flop ....} to change from pre flop to flop send 1
	int previousGlobalGameStage = 0;						//temp globalgamestage
	int bigBlind = 50;
	int smallBlind = 25;
	int totalChips = 0;										//perperson
	int totalPot = 0;										//totalPot for a game
	int currentBiggestBet = 0;

	int numberOfPlayersToStartGame = 4;

	bool bigBlindBet = false;									//determines whether the big blind has already bet or not - used during preflop
	bool dealerBet = false;										//determines whether the dealer has already bet or not - used after preflop

protected:
	void incomingConnection(qintptr  socketDescriptor);

};

#endif // POKERSERVER_H