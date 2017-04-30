#ifndef POKERSERVER_H
#define POKERSERVER_H
#include "ServerThread.h"
#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include <QThread>
#include <QDebug>
#include <QTcpServer>
#include <QFuture>
#include <QTConcurrent/QtConcurrentRun>
#include <QList>
#include <QtSql>
#define N 2;
class PokerServer : public QTcpServer
{
	Q_OBJECT

public:
	ServerThread *thread;
	//QList<ServerThread*> clientThreads;
	explicit PokerServer(QObject * parent = 0);
	void StartServer();
	void delay(int);
	void dealCards();										//deals cards to the table and the players
	void checkForWinner();									//checks which player has best hand
	void initBlinds();										//initialisesBlinds
	bool AcquireConnectionDb();								//connect to db
	QString compareCards();									//compare cards between players
	QString createCardTable(Card**);						//creates a table which is going to determine the best hand of the player
	int currentPlayer = 0;									//thread id  of current player
	int testI = 0;											//get rid of later gonna use it to test algoritm and input manual cards;
	QList<Player*> listOfPlayers;
	QVector<int> allPlayersVector;
	QVector<int> allPlayersChipsVector;
	QVector<int> arrayOfDrawers;
signals:
	void updateNoOfPlayersToStartGame(int num);				//notify socket threads how many players to start game;
	void updateBetMade(bool);								//notify socket threads bet was made (int canCheck) 0 for call, 1 for check
	void updateRaiseMade(int, int, int, int);				//notify socket threads a raise was made (int who_made_the_raise,int amount_raised)
	void updateCallMade(int num, int, int, int);			//notify socket threads a call was made (int who_made_the_call)
	void updateCheckMade(int num);							//notify socket threads a check was made (int who_made_the_call)
	void updateFoldMade(int num);							//notify socket threads of current player fold
	void updateCurrentPlayer(int num);						//notify socket threads of current player
	void updateAllPlayers(QVector<int>, QVector<int>, int, int);			// notify socket of all thread numbers
	void updateNoClients(int num);							//notify socket threads of number of clients
	void updateOnWin(int, QString);									//notify socket of a winner
	void updateOnDraw(int numofdrawers, QVector<int> thedrawesIds);
	void changeGameStage(int num, QVector<int>);			//notify socket that game stage changed ie pre flop to flop. 0 for pre flop, 1 for flop ....to change from pre flop to flop send 1
	void updateOnGameEnd(int winner);

	void updateHand(Card*, Card*, int);						//notify socket threads and update them with their current hand (car1,card2,socketDescriptor)
	void updateCardsOnTable(Card**);						//notify socket threads and update them with current cards on table
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
	int numberOfClientsInPlay = 0;
	int dealerCounter = 0;									//used to determine who is the dealer
	int smallBlindCounter = 0;
	int bigBlindCounter = 0;
	int globalI = 0;										//used to increment through Currentplayer list
	int globalGameStage = 0;								//determines stage of game {0 pre flop, 1 flop ....} to change from pre flop to flop send 1
	int previousGlobalGameStage = 0;						//temp globalgamestage
	int bigBlind = 50;
	int smallBlind = 25;
	int totalChips = 0;										//perperson
	int totalPot = 0;										//totalPot for a game
	int winnersPot = 0;
	int currentBiggestBet = 0;
	
	int numberOfPlayersToStartGame = N;

	bool bigBlindBet = false;									//determines whether the big blind has already bet or not - used during preflop
	bool dealerBet = false;										//determines whether the dealer has already bet or not - used after preflop

	bool isGameFinished = false;								//game is finished when 1 player has all the chips
	bool isHandFinished = false;								//hand is finished when game stage river is finished

	Deck playingDeck;										//playing deck 
	Card* cardsOnTable[5];									//the card that gonna be dealt to table

	QSqlDatabase db;

protected:
	void incomingConnection(qintptr  socketDescriptor);

};

#endif // POKERSERVER_H