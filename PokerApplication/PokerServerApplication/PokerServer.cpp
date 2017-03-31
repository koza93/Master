#include "PokerServer.h"
#include "Deck.h"
#include "Card.h"
#include <QTime>
#include <QCoreApplication>


PokerServer::PokerServer(QObject * parent) : QTcpServer(parent)
{


}



void PokerServer::StartServer()
{

	if (!this->listen(QHostAddress::Any, 1234))
	{
		qDebug() << "Failed to start server";
	}
	else
	{
		qDebug() << "Server started, waiting for connection...";
		PokerServer s;

	}

	/*
	gameThread = new GameThread(numberOfClients, this);
	connect(gameThread, SIGNAL(finished()), gameThread, SLOT(deleteLater()));
	gameThread->start();*/
}

void PokerServer::incomingConnection(qintptr  socketDescriptor)
{
	qDebug() << socketDescriptor << "Connecting...";
	ServerThread *thread = new ServerThread(socketDescriptor, this);
	//clientThreads.append(thread);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	connect(this, SIGNAL(updateHand(Card*, Card*,int)), thread, SLOT(updateMyCurrentHand(Card*, Card*,int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateCardsOnTable(Card**)), thread, SLOT(updateCardsOnTable(Card**)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateNoClients(int)), thread, SLOT(updateNumberClients(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateNoOfPlayersToStartGame(int)), thread, SLOT(updateNoOfPlayersToStartGame(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateCurrentPlayer(int)), thread, SLOT(updateCurrentPlayer(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(changeGameStage(int)), thread, SLOT(changeGameStage(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateBetMade(bool)), thread, SLOT(updateBetMade(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateFoldMade(int)), thread, SLOT(updateFoldMade(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateRaiseMade(int,int)), thread, SLOT(updateRaiseMade(int, int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateCallMade(int)), thread, SLOT(updateCallMade(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateCheckMade(int)), thread, SLOT(updateCheckMade(int)), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnBet()), this, SLOT(incrementCurrentPlayer()), Qt::QueuedConnection); // signal commes from socketThread noteToSelf: this solves my issue of communicating back from the thread
	connect(thread, SIGNAL(notifyOnBet()), this, SLOT(detectBetWasMade()), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnRaise(int,int)), this, SLOT(detectRaiseWasMade(int,int)), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnCall(int)), this, SLOT(detectCallWasMade(int)), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnCheck(int)), this, SLOT(detectCheckWasMade(int)), Qt::QueuedConnection);
	connect(thread, SIGNAL(notifyOnFold(int)), this, SLOT(detectFoldWasMade(int)), Qt::QueuedConnection);

	qDebug() << "main" << QThread::currentThreadId();
	thread->start();
	listOfPlayers.append(new Player(socketDescriptor, 500));
	numberOfClients++;
	if (numberOfClients == 1)
	{
		currentPlayer = socketDescriptor;
		qDebug() << "First player is:" << currentPlayer;
		listOfPlayers[0]->setAsDealer(true);				//the first player to join is always the dealer - the dealer token will be passed around
	}
	if (numberOfClients > 0)
	{
		qDebug() << "Emmiting signal from poker server";

		//this line of code will have to be moved somewhere else it is going to initialise the big blind and small blind 
		if (numberOfClients == numberOfPlayersToStartGame)
		{
			//set big blind and small blind
			if (numberOfClients < 3 && numberOfClients > 1)
			{
				listOfPlayers[0]->setCurrentBet(smallBlind);
				listOfPlayers[1]->setCurrentBet(bigBlind);
				listOfPlayers[1]->setAsBigBlind(true);
				currentBiggestBet = bigBlind;
				currentPlayer = listOfPlayers[0]->getSocketDescriptor();
				globalI = 0;										//globalI is used to increment and identifier of the player in the list of players
			}
			else if(numberOfClients == 3)
			{
				listOfPlayers[1]->setCurrentBet(smallBlind);
				listOfPlayers[2]->setCurrentBet(bigBlind);
				listOfPlayers[2]->setAsBigBlind(true);
				currentBiggestBet = bigBlind;
				currentPlayer = listOfPlayers[0]->getSocketDescriptor();
				globalI = 0;
			}
			else
			{
				listOfPlayers[1]->setCurrentBet(smallBlind);
				listOfPlayers[2]->setCurrentBet(bigBlind);
				listOfPlayers[2]->setAsBigBlind(true);
				currentBiggestBet = bigBlind;
				currentPlayer = listOfPlayers[3]->getSocketDescriptor();
				globalI = 3;
			}
		}

		emit updateNoOfPlayersToStartGame(numberOfPlayersToStartGame);
		emit updateNoClients(numberOfClients);
		emit updateCurrentPlayer(currentPlayer);

		//for now this piece of code creates a deck, i might fit it elsewhere 
		if (numberOfClients == numberOfPlayersToStartGame) {
			
			playingDeck.shuffleDeck();
			dealCards();
			/*/
			for (int i = 0; i < 5; i++) {
				qDebug() << cardsOnTable[i]->getFigure() << cardsOnTable[i]->getSuit();
			}*/
		}
		//if i dont fit it elswhere i am going to send a signal containing the deck to all the threads and they can deal with it from there
		//UPDATE:: gonna need to deal the cards first then send the signal
		//UPDATE2:: so i think i gonna need to send a signal from each thread that will say its ready to receive the cards and then send the card with another signal
	}



	//emit updateNoClients(numberOfClients);
	qDebug() << "Number of clients: " << numberOfClients;
}


void PokerServer::delay(int millisecondsToWait)
{
	QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
	while (QTime::currentTime() < dieTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

void PokerServer::incrementCurrentPlayer()
{
	bool allBetsMadeFlag = true;										//local flag to check if everyone made a called/folded bet during the round
	
	listOfPlayers[globalI]->setAsBet(true);								//takes not of the fact that the player already bet this turn - needs to be changed for all players after game stage change

	globalI++;
	if (globalI >= listOfPlayers.length())
	{
		globalI = 0;
	}

	while (listOfPlayers[globalI]->isFolded())
	{
		globalI++;
		if (globalI >= listOfPlayers.length())
		{
			globalI = 0;
		}
	}

	for (int i = 0; i < listOfPlayers.length(); i++)
	{
		if (!listOfPlayers[i]->isFolded())
		{
			if (listOfPlayers[i]->getCurrentBet() < currentBiggestBet || !listOfPlayers[i]->isBet() ) {	//makes sure all players have bet
				allBetsMadeFlag = false;
			}
			qDebug() << "The current bet for each player is: PLayer: " << listOfPlayers[i]->getSocketDescriptor() << " Bet: " << listOfPlayers[i]->getCurrentBet();
		}
	}

	if (allBetsMadeFlag)//if (allBetsMadeFlag && (bigBlindBet || dealerBet))//only if all bets are equal and the player on the big blind has already played change the game stage
	{
		previousGlobalGameStage = globalGameStage;
		globalGameStage++;
		//if game stage change the current player must be the player next to the dealer
		for (int i = 0; i < listOfPlayers.length(); i++)
		{
			if (listOfPlayers[i]->isDealer())
			{
				globalI = i+1;
				if (globalI >= listOfPlayers.length())
				{
					globalI = 0;
				}
				while (listOfPlayers[globalI]->isFolded())
				{
					globalI++;
					if (globalI >= listOfPlayers.length())
					{
						globalI = 0;
					}
				}
				currentPlayer = listOfPlayers[globalI]->getSocketDescriptor();
				emit updateCurrentPlayer(currentPlayer);
			}
		}
	}
	else
	{
		currentPlayer = listOfPlayers[globalI]->getSocketDescriptor();
		emit updateCurrentPlayer(currentPlayer);
	}
	if (allBetsMadeFlag)
	{
		for (int i = 0; i < listOfPlayers.length(); i++)
		{
			if (!listOfPlayers[i]->isFolded())
			{
				listOfPlayers[i]->setAsBet(false);
			}
		}
		
		emit changeGameStage(globalGameStage);
		if (globalGameStage == 4)
		{
			checkForWinner();							//TODO gonna have to figure out what happens when everyone folds
		}
	}
}

void PokerServer::detectRaiseWasMade(int socketDescriptor, int amountRaised)
{
	for (int i = 0; i < listOfPlayers.length(); i++)
	{
		if (listOfPlayers[i]->getSocketDescriptor() == socketDescriptor)
		{
			listOfPlayers[i]->setCurrentBet(currentBiggestBet+amountRaised);
		}
	}
	currentBiggestBet += amountRaised;								//adding the amount raised on top of whateever is already highest bet
	qDebug() << "The current biggest bet at raise is: " << currentBiggestBet;
	emit updateRaiseMade(socketDescriptor, amountRaised);				

}

void PokerServer::detectFoldWasMade(int socketDescriptor)
{
	int foldedNumber = 0;
	for (int i = 0; i < listOfPlayers.length(); i++)
	{
		if (listOfPlayers[i]->getSocketDescriptor() == socketDescriptor)
		{
			listOfPlayers[i]->setAsFolded(true);
			qDebug() << "Player " << i << "has folded!";
		}
	}
	
	emit updateFoldMade(socketDescriptor);

}

void PokerServer::detectCallWasMade(int socketDescriptor)
{
	for (int i = 0; i < listOfPlayers.length(); i++)
	{
		if (listOfPlayers[i]->getSocketDescriptor() == socketDescriptor)
		{
			qDebug() << "The current biggest bet at call is: " << currentBiggestBet;
			listOfPlayers[i]->setCurrentBet(currentBiggestBet);
			
		}
	}

	emit updateCallMade(socketDescriptor);

}
void PokerServer::detectCheckWasMade(int socketDescriptor)
{
	emit updateCheckMade(socketDescriptor);
}

void PokerServer::detectBetWasMade()
{

	if (listOfPlayers[globalI]->getCurrentBet() == currentBiggestBet)
		emit updateBetMade(true); //(int CanCheck)  send 1 if can check, send 0 if not
	else
		emit updateBetMade(false);
}

void PokerServer::dealCards()
{
	int k = 0;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < numberOfClients; i++) {
			listOfPlayers[i]->setMyCurrentHand(playingDeck.getCardFromDeck(k),j);
			k++;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		cardsOnTable[i] = playingDeck.getCardFromDeck(k);
		k++;
	}

	//notify socket threads of their cards
	for (int i = 0; i < numberOfClients; i++) {
		emit updateHand(listOfPlayers[i]->getMyCards(0), listOfPlayers[i]->getMyCards(1), listOfPlayers[i]->getSocketDescriptor());
	}

	emit updateCardsOnTable(cardsOnTable);
}

void PokerServer::checkForWinner()
{
	for (int i = 0; i < listOfPlayers.size(); i++)
	{
		Card* all7OnCards[7];
		for (int j = 0; j < 5; j++)
		{
			all7OnCards[j] = cardsOnTable[j];
		}
		for (int j = 5; j < 7; j++)
		{
			all7OnCards[j] = listOfPlayers[i]->getMyCards(j - 5);
		}
		createCardTable(all7OnCards);
		/*
		qDebug() << "All71 is:" << all7OnCards[0]->getFigure() << all7OnCards[0]->getSuit();
		qDebug() << "All72 is:" << all7OnCards[1]->getFigure() << all7OnCards[1]->getSuit();
		qDebug() << "All73 is:" << all7OnCards[2]->getFigure() << all7OnCards[2]->getSuit();
		qDebug() << "All74 is:" << all7OnCards[3]->getFigure() << all7OnCards[3]->getSuit();
		qDebug() << "All75 is:" << all7OnCards[4]->getFigure() << all7OnCards[4]->getSuit();
		qDebug() << "All76 is:" << all7OnCards[5]->getFigure() << all7OnCards[5]->getSuit();
		qDebug() << "All77 is:" << all7OnCards[6]->getFigure() << all7OnCards[6]->getSuit();
		*/
	}
}

void PokerServer::createCardTable(Card ** cards)
{
	//QList<QString> table;
	QMap<QString, int> figureMap;
	QMap<QString, int> suitMap;
	//key, value for figures
	figureMap["2"] = 0;
	figureMap["3"] = 0;
	figureMap["4"] = 0;
	figureMap["5"] = 0;
	figureMap["6"] = 0;
	figureMap["7"] = 0;
	figureMap["8"] = 0;
	figureMap["9"] = 0;
	figureMap["X"] = 0;
	figureMap["J"] = 0;
	figureMap["Q"] = 0;
	figureMap["K"] = 0;
	figureMap["A"] = 0;

	//key, value for suits
	suitMap["C"] = 0;
	suitMap["S"] = 0;
	suitMap["D"] = 0;
	suitMap["H"] = 0;

	for (int i = 0; i < 7; i++)
	{
		figureMap[QString(cards[i]->getFigure())] = figureMap.value(QString(cards[i]->getFigure())) + 1;
	}
	for (int i = 0; i < 7; i++)
	{
		suitMap[QString(cards[i]->getSuit())] = suitMap.value(QString(cards[i]->getSuit())) + 1;
	}
	/*
	qDebug() << "figure values";
	foreach(int figure, figureMap) {
		qDebug() << figure;
	}

	qDebug() << "suit values";
	foreach(int suit, suitMap) {
		qDebug() << suit;
	}*/

	foreach(int suit, suitMap) {
		if (suit == 5) {
			qDebug() <<"Flush";
		}
	}

	foreach(int figure, figureMap) {
		if (figure == 4) {
			qDebug() << "4aKind";
		}
	}

	int pair = 0;
	foreach(int figure, figureMap) {
		if (figure == 3) {

			foreach(int figure, figureMap) {
				if (figure == 2) {
					pair++;
				}
			}
			if(pair > 0)
				qDebug() << "FullHouse";
			else
				qDebug() << "3aKind";
		}
	}

	pair = 0;
	foreach(int figure, figureMap) {
		if (figure == 2) {
			pair++;
		}
		if (pair > 1)
			qDebug() << "2pair";
		else if(pair >0)
			qDebug() << "pair";
	}
	qDebug() << "***************";
}