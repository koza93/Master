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
	connect(this, SIGNAL(updateAllPlayers(QVector<int>, int, int)), thread, SLOT(updateAllPlayers(QVector<int>, int, int)), Qt::DirectConnection);
	connect(this, SIGNAL(changeGameStage(int, QVector<int>)), thread, SLOT(changeGameStage(int, QVector<int>)), Qt::DirectConnection);
	connect(this, SIGNAL(updateBetMade(bool)), thread, SLOT(updateBetMade(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateFoldMade(int)), thread, SLOT(updateFoldMade(int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateRaiseMade(int,int)), thread, SLOT(updateRaiseMade(int, int)), Qt::QueuedConnection);
	connect(this, SIGNAL(updateCallMade(int, int)), thread, SLOT(updateCallMade(int, int)), Qt::QueuedConnection);
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

	//will use this vector to send all player numbers to application
	allPlayersVector.append(socketDescriptor);

	if (numberOfClients == 1)
	{
		currentPlayer = socketDescriptor;
		qDebug() << "First player is:" << currentPlayer;
		listOfPlayers[dealerCounter]->setAsDealer(true);				//the first player to join is always the dealer - the dealer token will be passed around
	}
	if (numberOfClients > 0)
	{
		qDebug() << "Emmiting signal from poker server";
		
		initBlinds();
		
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
		currentBiggestBet = 0;							//reset current biggestBet to 0
		//if game stage change the current player must be the player next to the dealer
		for (int i = 0; i < listOfPlayers.length(); i++)
		{
			listOfPlayers[i]->setCurrentBet(currentBiggestBet);
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
		QVector<int> playersToUpdate;					//local vector containing the thread id of players that need to be updated at change game stage
		for (int i = 0; i < listOfPlayers.length(); i++)
		{
			if (!listOfPlayers[i]->isFolded())
			{
				listOfPlayers[i]->setAsBet(false);
				playersToUpdate.append(listOfPlayers[i]->getSocketDescriptor());
			}
		}

		
		
		emit changeGameStage(globalGameStage, playersToUpdate);
		if (globalGameStage == 4)
		{
			checkForWinner();							//TODO gonna have to figure out what happens when everyone folds

			//code here will take note of who won, who lost and move the chips and so arround

			/*
			//TODO
			//TODO
			*/


			//		The code that is going to reset the hand and start the next one
			//if no one has all the chips
			globalGameStage = 0;												//resetGlobalGameStage

			//reset player attributes
			for (int i = 0; i < listOfPlayers.length(); i++) {
				listOfPlayers[i]->setCurrentBet(0);
				listOfPlayers[i]->setAsBigBlind(false);

				//TODO make an if statement checking if player has enough chips to play
				listOfPlayers[i]->setAsFolded(false);
				//gonna set dealer as false here as well
				//listOfPlayers[dealerCounter]->setAsDealer(false);	
				listOfPlayers[i]->setBestCards("");
			}
			//dealerCounter++;													//increment dealer counter
			//if (dealerCounter == numberOfClients) { dealerCounter = 0; }		//just in case

			//delay(10000);
			
			

			for (int i = 0; i < listOfPlayers.length(); i++)
			{
				if (!listOfPlayers[i]->isFolded())
				{
					playersToUpdate.append(listOfPlayers[i]->getSocketDescriptor());
				}
			}
			emit changeGameStage(globalGameStage, playersToUpdate);								//change the global stage
			initBlinds();
			
			
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
			listOfPlayers[i]->setTotalChips(listOfPlayers[i]->getTotalChips() - (currentBiggestBet + amountRaised));
		}
	}
	currentBiggestBet += amountRaised;								//adding the amount raised on top of whateever is already highest bet
	qDebug() << "The current biggest bet at raise is: " << currentBiggestBet;
	emit updateRaiseMade(socketDescriptor, currentBiggestBet);		//if a player has raised then his bet must be current biggest bet				

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
			listOfPlayers[i]->setTotalChips(listOfPlayers[i]->getTotalChips() - (currentBiggestBet));
		}
	}

	emit updateCallMade(socketDescriptor, currentBiggestBet);

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
		//save the best hand of the player for current game
		listOfPlayers[i]->setBestCards(createCardTable(all7OnCards));
	}

	QString winner = compareCards();
	//qDebug() << winner;

	QStringList winnerParams = winner.split(":");
	if (winnerParams[0] == "Draw")
	{
		for (int i = 0; i < winnerParams[1].toInt(); i++) {
			qDebug() << "Draw - Player no:" << listOfPlayers[winnerParams[i + 2].toInt()]->getSocketDescriptor();
		}
	}
	else if (winnerParams[0] == "Win")
	{
		qDebug() << "Win - Player no:" << listOfPlayers[winnerParams[1].toInt()]->getSocketDescriptor();
	}
}

QString PokerServer::createCardTable(Card ** cards)
{
	//TODO need to send the suits or figures with the winning combinatio, need to figure ot straights and change thios function so it returns stuff

	//QList<QString> table;
	QMap<QString, int> figureMap;
	QMap<QString, int> suitMap;
	QMap<int, QString> cardMap;
	
	QString suitOrFigure ="";
	QString highCard = "";
	int pair = 0;
	int triple = 0;
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

	//sort in ascending order
	cardMap[0] = "2";
	cardMap[1] = "3";
	cardMap[2] = "4";
	cardMap[3] = "5";
	cardMap[4] = "6";
	cardMap[5] = "7";
	cardMap[6] = "8";
	cardMap[7] = "9";
	cardMap[8] = "X";
	cardMap[9] = "J";
	cardMap[10] = "Q";
	cardMap[11] = "K";
	cardMap[12] = "A";

	//key, value for suits
	suitMap["C"] = 0;
	suitMap["S"] = 0;
	suitMap["D"] = 0;
	suitMap["H"] = 0;
	
	/*
	//testing cards with manual values
	Card* cards[7];
	cards[0] = new Card('S', 'A');
	cards[1] = new Card('D', 'K');
	cards[2] = new Card('S', 'A');
	cards[3] = new Card('D', 'K');
	cards[4] = new Card('S', '3');

	if (testI == 0)
	{
		cards[5] = new Card('D', '4');
		cards[6] = new Card('D', '5');
	}
	if (testI == 1)
	{
		cards[5] = new Card('C', '6');
		cards[6] = new Card('C', '7');
	}
	if (testI == 2)
	{
		cards[5] = new Card('D', 'J');
		cards[6] = new Card('D', '4');
	}
	testI++;
	*/
	

	//fill in the tables
	
	for (int i = 0; i < 7; i++)
	{
		figureMap[QString(cards[i]->getFigure())] = figureMap.value(QString(cards[i]->getFigure())) + 1;
	}
	
	
	
	for (int i = 0; i < 7; i++)
	{
		suitMap[QString(cards[i]->getSuit())] = suitMap.value(QString(cards[i]->getSuit())) + 1;
	}
	
	QMapIterator<QString, int> itF(figureMap);
	QMapIterator<int, QString> itC(cardMap);
	QMapIterator<QString, int> itS(suitMap);
	/*
	qDebug() << "figure values";
	foreach(int figure, figureMap) {
		qDebug() << figure;
	}

	qDebug() << "suit values";
	foreach(int suit, suitMap) {
		qDebug() << suit;
	}*/

	//StrightFlush//////////////////////////////////////
	int straightCards = 0;
	while (itC.hasNext()) {
		itC.next();
		if (figureMap[itC.value()] > 0) {
			straightCards++;
			if(straightCards == 1)
				suitOrFigure += itC.value();
			else 
				suitOrFigure += ":" + itC.value();
		
		}
		
		else
		{
			if (straightCards < 5)
			{
				suitOrFigure = "";
				straightCards = 0;
			}
		}
	}
	itC.toFront();
	QString colour;
	int strColCards = 0;
	
	//need to figure what to do with more than 5cards
	if (straightCards > 4) {
		while (itS.hasNext()) {
			itS.next();
			if (itS.value() >= 5) {
				colour = itS.key();
			}
		}
		itS.toFront();
		QStringList coloredCards = suitOrFigure.split(":");
		suitOrFigure = "";
		for (int j = 0; j < coloredCards.size(); j++)
		{	
			for (int i = 0; i < 7; i++)
			{
				if (QString(cards[i]->getSuit()) == colour)     //this if statement is just a quick fix
				{
					if (QString(cards[i]->getFigure()) == coloredCards[j])
					{
						if (QString(cards[i]->getSuit()) == colour)
						{
							strColCards++;
							if (strColCards == 1)
								suitOrFigure += QString(cards[i]->getFigure());
							else
								suitOrFigure += ":" + QString(cards[i]->getFigure());
						}

						else
						{
							if (strColCards < 5)
							{
								suitOrFigure += "";
								strColCards = 0;
							}
							else
							{
								return  "StraightFlush:" + suitOrFigure.mid((suitOrFigure.size() - 9), 9);
								suitOrFigure += "";
								strColCards = 0;
							}
						}
					}
				}
			}
		}
		if (strColCards > 4)
		{
			return "StraightFlush:" + suitOrFigure.mid((suitOrFigure.size() - 9), 9);
		}
	}
	
	//4Akind/////////////////////////////////////////////
	while (itF.hasNext()) {
		itF.next();
		if (itF.value() == 4) {
			return "4aKind:" + itF.key() +":N"+":N";
		}
	}
	itF.toFront();

	pair = 0; triple = 0;
	//Fullhouse//////////////////////////////////////////
	suitOrFigure = "";
	while (itF.hasNext()) {
		itF.next();
		if (itF.value() == 3) {
			triple++;
			if (triple == 1)
				suitOrFigure += itF.key();
			else
				suitOrFigure += ":" + itF.key();
		}
	}
	itF.toFront();
	if (triple == 2)
	{
		QList<int> val;
		val << 0 << 0;
		QStringList  arrayOfFigures = suitOrFigure.split(":");
		while (itC.hasNext()) {
			itC.next();
			if (itC.value() == arrayOfFigures[0]) {
				val[0] = itC.key();
			}
			if (itC.value() == arrayOfFigures[1]) {
				val[1] = itC.key();
			}

		}
		itC.toFront();
		qSort(val.begin(), val.end());
		suitOrFigure = cardMap[val[1]] + ":" + cardMap[val[0]];
		return "FullHouse:"+suitOrFigure + ":N";
	}
	if (triple == 1)
	{
		pair = 0;
		while (itF.hasNext()) {
			itF.next();
			if (itF.value() == 2) {
				pair++;
				suitOrFigure += ":" + itF.key();
			}
		}
		itF.toFront();
		if (pair > 1)
		{
			QList<int> val;
			val << 0 << 0 ;
			QStringList  arrayOfFigures = suitOrFigure.split(":");
			while (itC.hasNext()) {
				itC.next();
				if (itC.value() == arrayOfFigures[1]) {
					val[0] = itC.key();
				}
				if (itC.value() == arrayOfFigures[2]) {
					val[1] = itC.key();
				}

			}
			itC.toFront();
			qSort(val.begin(), val.end());
			suitOrFigure = arrayOfFigures[0] +":"+cardMap[val[1]] ;
			return "FullHouse:" + suitOrFigure + ":N";
		}
		else if (pair > 0)
			return "FullHouse:" + suitOrFigure + ":N";
	}
		
	//Flush//////////////////////////////////////////////
	/*while (itS.hasNext()) {
		itS.next();
		if (itS.value() >= 5) {
			return "Flush:" +itS.key();
		}
	}
	itS.toFront();*/


	//Flush v2///////////////////////////////////////////
	
	QString coloured = "N";
	suitOrFigure = "";
	//need to figure what to do with more than 5cards
	while (itS.hasNext()) {
		itS.next();
		if (itS.value() >= 5) {
			coloured = itS.key();
		}
	}
	itS.toFront();
	if (coloured != "N")
	{
		//clear the figuremap
		while (itF.hasNext()) {
			itF.next();
			figureMap[itF.key()] = 0;
		}
		itF.toFront();
		for (int i = 0; i < 7; i++)
		{
			if (QString(cards[i]->getSuit()) == coloured)     //this if statement is just a quick fix
			{
				//fill in the card from map if coloured
				figureMap[QString(cards[i]->getFigure())] = 1;
			}
		}

		QString oldHighCard;
		QString olderHighCard;
		while (itC.hasNext()) {
			itC.next();
			if (figureMap[itC.value()] == 1) {
				olderHighCard = oldHighCard;
				oldHighCard = suitOrFigure;
				suitOrFigure = itC.value();
			}
		}
		return "Flush:" + suitOrFigure + ":" + oldHighCard + ":" + olderHighCard;


	}
	//Straight///////////////////////////////////////////
	straightCards = 0;
	while (itC.hasNext()) {
		itC.next();
		if (figureMap[itC.value()] > 0) {
			straightCards++;
			suitOrFigure += ":" + itC.value();
		}
		else
		{
			if (straightCards < 5)
			{
				suitOrFigure += "";
				straightCards = 0;
			}
			else
			{
				return "Straight:" + suitOrFigure.mid((suitOrFigure.size() - 9), 9);
				suitOrFigure += "";
				straightCards = 0;
			}
		}
	}
	itC.toFront();
		//need to figure what to do with more than 5cards
	if (straightCards > 4) {
		return "Straight:" + suitOrFigure.mid((suitOrFigure.size() - 9), 9);
	}
	suitOrFigure = "";
	QString oldSuitOrFigure;
	//3Akind/////////////////////////////////////////////
	while (itF.hasNext()) {
		itF.next();

		if (itF.value() == 3) {
			figureMap[itF.key()] = 0;
			while (itC.hasNext()) {
				itC.next();
				if (figureMap[itC.value()] == 1) {
					oldSuitOrFigure = suitOrFigure;
					suitOrFigure = ":" + itC.value();
				}
			}
			return "3aKind:" + itF.key() + suitOrFigure + oldSuitOrFigure;
		}
	}
	itF.toFront();
	
	//pair///////////////////////////////////////////////
	
	suitOrFigure = "";
	pair = 0;
	while (itF.hasNext()) {
		itF.next();
		if (itF.value() == 2) {
			
			pair++;
			if(pair == 1)
				suitOrFigure += itF.key();
			else
				suitOrFigure += ":" + itF.key();
		}
	}
	if (pair > 2)
	{
		QList<int> val ;
		val << 0 << 0 << 0;
		QStringList  arrayOfFigures = suitOrFigure.split(":");
		while (itC.hasNext()) {
			itC.next();
			if (itC.value() == arrayOfFigures[0]) {
				val[0] = itC.key();
			}
			if (itC.value() == arrayOfFigures[1]) {
				val[1] = itC.key();
			}
			if (itC.value() == arrayOfFigures[2]) {
				val[2] = itC.key();
			}
		}
		itC.toFront();
		qSort(val.begin(), val.end());
		suitOrFigure = cardMap[val[2]] + ":" + cardMap[val[1]];

		//to determine the highest card less 2 pairs delete the pairs from the list
		figureMap[cardMap[val[2]]] = 0;
		figureMap[cardMap[val[1]]] = 0;
		while (itC.hasNext()) {
			itC.next();
			if (figureMap[itC.value()] >= 1) {
				highCard = ":" + itC.value();
			}
		}
		return "2pair:" + suitOrFigure + highCard;				//need to distinguish beteween highest 2pairs
	}
	else if (pair > 1)
	{
		QList<int> val;
		val << 0 << 0;
		QStringList  arrayOfFigures = suitOrFigure.split(":");
		while (itC.hasNext()) {
			itC.next();
			if (itC.value() == arrayOfFigures[0]) {
				val[0] = itC.key();
			}
			if (itC.value() == arrayOfFigures[1]) {
				val[1] = itC.key();
			}

		}
		itC.toFront();
		qSort(val.begin(), val.end());
		suitOrFigure = cardMap[val[1]] + ":" + cardMap[val[0]];

		//to determine the highest card less 2 pairs delete the pairs from the list
		figureMap[cardMap[val[1]]] = 0;
		figureMap[cardMap[val[0]]] = 0;
		while (itC.hasNext()) {
			itC.next();
			if (figureMap[itC.value()] == 1) {
				highCard = ":" + itC.value();
			}
		}
		return "2pair:" + suitOrFigure + highCard;
	}
	
	else if (pair > 0) {
		//to determine the highest card less 2 pairs delete the pairs from the list
		highCard = "";
		QString oldHighCard;
			figureMap[suitOrFigure] = 0;
		while (itC.hasNext()) {
			itC.next();
			if (figureMap[itC.value()] == 1) {
				oldHighCard = highCard;
				highCard = ":" + itC.value();
			}
		}
		return "pair:" + suitOrFigure + highCard + oldHighCard;
		itF.toFront();
	}
	/////////////////////////////////////////////////////
	
	//High Card
	suitOrFigure = "";
	QString oldHighCard;
	QString olderHighCard;
	while (itC.hasNext()) {
		itC.next();
		if (figureMap[itC.value()] == 1) {
			olderHighCard = oldHighCard;
			oldHighCard = suitOrFigure;
			suitOrFigure = itC.value();
		}
	}
	return "HighCard:" + suitOrFigure + ":" + oldHighCard + ":" + olderHighCard ;
	
	/////////////////////////////////
	//qDebug() << "***************";
}

QString PokerServer::compareCards()
{
	QString theWinner = "";
	QMap<QString, int> ranks;
	QMap<QString, int> highCard;
	
	//key, value for figures
	ranks["HighCard"] = 0;				
	ranks["pair"] = 1;
	ranks["2pair"] = 2;
	ranks["3aKind"] = 3;
	ranks["Straight"] = 4;
	ranks["Flush"] = 5;
	ranks["FullHouse"] = 6;
	ranks["4aKind"] = 7;
	ranks["StraightFlush"] = 8;

	//sort in ascending order
	highCard["N"] = -1;
	highCard["2"] = 0;
	highCard["3"] = 1;
	highCard["4"] = 2;
	highCard["5"] = 3;
	highCard["6"] = 4;
	highCard["7"] = 5;
	highCard["8"] = 6;
	highCard["9"] = 7;
	highCard["X"] = 8;
	highCard["J"] = 9;
	highCard["Q"] = 10;
	highCard["K"] = 11;
	highCard["A"] = 12;
	
	QList<QStringList> listOfHands;
	int highestRank = 0;
	//get the highest rank out of each player put the parameters into stringlist and put the string list into a qlist
	for (int i = 0; i < listOfPlayers.size(); i++)
	{
		QStringList rankList;
		qDebug() << listOfPlayers[i]->getBestCards();
		rankList = listOfPlayers[i]->getBestCards().split(":");
		listOfHands.append(rankList);
	}

	//debug
	for (int i = 0; i < listOfHands.length(); i++)
	{
		qDebug() << listOfHands[i].at(0) << ":"<<listOfHands[i].at(1) <<":" <<listOfHands[i].at(2) ;
	}

	//get the highest rank in the list
	for (int i = 0; i < listOfHands.length(); i++)
	{
		int rank = ranks[listOfHands[i].at(0)];
		if (rank > highestRank)
		{
			highestRank = rank;
		}
	}

	QList<int> playersWithBestHand;

	// search which player/players had the highest rank, append them to list
	for (int i = 0; i < listOfPlayers.size(); i++)
	{
		if (ranks[listOfHands[i].at(0)] == highestRank) {
			playersWithBestHand.append(i);
			//qDebug() << "Player: " << i << "appended";
		}
	}

	//gonna reuse highestRank
	highestRank = 0;

	//if only one player has the highest rank he wins the hand
	if (playersWithBestHand.size() == 1) {
		qDebug() << "Win:Player: " << playersWithBestHand[0] << "has highest rank: " << listOfHands[playersWithBestHand[0]].at(0);
		
		return "Win:"+ QString::number(playersWithBestHand[0]);
	}

	//from here on im looking on the high cards ie 33384 > 2229X

	//else determine who has stronger hand of the same rank
	else
	{
		for (int i = 0; i < playersWithBestHand.size(); i++)
		{
			int rank = highCard[listOfHands[playersWithBestHand[i]].at(1)];
			if (rank > highestRank)
			{
				highestRank = rank;
			}
		}
	}

	QList<int> pWBHsearch1; //playersWithBestHand search 1
	
	// search which player/players had the highest card 1st, append them to list
	for (int i = 0; i <  playersWithBestHand.size(); i++)
	{
		if (highCard[listOfHands[playersWithBestHand[i]].at(1)] == highestRank) {
			pWBHsearch1.append(playersWithBestHand[i]);
			//qDebug() << "Player: " << i << "appended search1";
		}
	}


	//gonna reuse highestRank
	highestRank = 0;

	//if only one player has the highest card at search 1 he wins the hand
	if (pWBHsearch1.size() == 1) {
		qDebug() << "Win:Player: " << pWBHsearch1[0] << "has highest rank: " << listOfHands[pWBHsearch1[0]].at(0) << ":"<< listOfHands[pWBHsearch1[0]].at(1);
		return "Win:" + QString::number(pWBHsearch1[0]);
	}

	//round 2

	//else determine who has the strongest second card ie second pair or highest card
	else
	{
		for (int i = 0; i < pWBHsearch1.size(); i++)
		{
			int rank = highCard[listOfHands[pWBHsearch1[i]].at(2)];
			if (rank > highestRank)
			{
				highestRank = rank;
			}
		}
	}

	QList<int> pWBHsearch2; //playersWithBestHand search 2

	// search which player/players had the highest card 2nd, append them to list
	for (int i = 0; i < pWBHsearch1.size(); i++)
	{
		if (highCard[listOfHands[pWBHsearch1[i]].at(2)] == highestRank) {
			pWBHsearch2.append(pWBHsearch1[i]);
			//qDebug() << "Player: " << i << "appended search2";
		}
	}

	//gonna reuse highestRank
	highestRank = 0;

	//if only one player has the highest card at search 1 he wins the hand
	if (pWBHsearch2.size() == 1) {
		qDebug() << "Win:Player: " << pWBHsearch2[0] << "has highest rank: " << listOfHands[pWBHsearch2[0]].at(0) << ":" << listOfHands[pWBHsearch2[0]].at(1) << listOfHands[pWBHsearch2[0]].at(2);
		return "Win:" + QString::number(pWBHsearch2[0]);
	}

	//round 3

	//else determine who has the strongest third card ie highest card or second highest card;
	else
	{
		for (int i = 0; i < pWBHsearch2.size(); i++)
		{
			int rank = highCard[listOfHands[pWBHsearch2[i]].at(3)];
			if (rank > highestRank)
			{
				highestRank = rank;
			}
		}
	}

	QList<int> pWBHsearch3; //playersWithBestHand search 3

	// search which player/players had the highest card 3rd, append them to list
	for (int i = 0; i < pWBHsearch2.size(); i++)
	{
		if (highCard[listOfHands[pWBHsearch2[i]].at(3)] == highestRank) {
			pWBHsearch3.append(pWBHsearch2[i]);
			//qDebug() << "Player: " << i << "appended search 3";
		}
	}

	//if only one player has the highest card at search 1 he wins the hand
	if (pWBHsearch3.size() == 1) {
		qDebug() << "Win:Player: " << pWBHsearch3[0] << "has highest rank: " << listOfHands[pWBHsearch3[0]].at(0) << ":" << listOfHands[pWBHsearch3[0]].at(1) << listOfHands[pWBHsearch3[0]].at(2) << listOfHands[pWBHsearch3[0]].at(3);
		return "Win:" + QString::number(pWBHsearch3[0]);
	}

	else
	{
		QString drawers = "Draw";
		//some draw code in a loop
		for (int i = 0; i < pWBHsearch3.size(); i++)
		{
			qDebug() << "Player: " << pWBHsearch3[i] << "has drew";
			drawers += ":" + QString::number(pWBHsearch3[i]);
		}
		return drawers;				//returns draw : number of drawers : drawer x : drawer x1 ....    ie Draw:3:1:2:3  Draw:2:5:2
	}
}

void PokerServer::initBlinds() 
{
	
	//this line of code will have to be moved somewhere else it is going to initialise the big blind and small blind 
	if (numberOfClients == numberOfPlayersToStartGame)
	{
		//set big blind and small blind
		if (numberOfClients < 3 && numberOfClients > 1)
		{
			//TODO will need to change the big blind to next person every time
			listOfPlayers[0]->setCurrentBet(smallBlind);
			listOfPlayers[0]->setAsSmallBlind(true);
			listOfPlayers[1]->setCurrentBet(bigBlind);
			listOfPlayers[1]->setAsBigBlind(true);
			currentBiggestBet = bigBlind;
			currentPlayer = listOfPlayers[0]->getSocketDescriptor();
			globalI = 0;										//globalI is used to increment and identifier of the player in the list of players
		}
		else if (numberOfClients == 3)
		{
			listOfPlayers[1]->setCurrentBet(smallBlind);
			listOfPlayers[1]->setAsSmallBlind(true);
			listOfPlayers[2]->setCurrentBet(bigBlind);
			listOfPlayers[2]->setAsBigBlind(true);
			currentBiggestBet = bigBlind;
			currentPlayer = listOfPlayers[0]->getSocketDescriptor();
			globalI = 0;
		}
		else
		{
			listOfPlayers[1]->setCurrentBet(smallBlind);
			listOfPlayers[1]->setAsSmallBlind(true);
			listOfPlayers[2]->setCurrentBet(bigBlind);
			listOfPlayers[2]->setAsBigBlind(true);
			currentBiggestBet = bigBlind;
			currentPlayer = listOfPlayers[3]->getSocketDescriptor();
			globalI = 3;
		}
	}

	int bbThreadId;
	int sbThreadId;

	for (int i = 0; i < listOfPlayers.length(); i++) {
		if (listOfPlayers[i]->isBigBlind()) {
			bbThreadId = listOfPlayers[i]->getSocketDescriptor();
		}
		if (listOfPlayers[i]->isSmallBlind()) {
			sbThreadId = listOfPlayers[i]->getSocketDescriptor();
		}
	}

	emit updateNoOfPlayersToStartGame(numberOfPlayersToStartGame);
	emit updateNoClients(numberOfClients);
	emit updateCurrentPlayer(currentPlayer);
	emit updateAllPlayers(allPlayersVector, bbThreadId, sbThreadId);

	//for now this piece of code creates a deck, i might fit it elsewhere 
	if (numberOfClients == numberOfPlayersToStartGame) {

		playingDeck.shuffleDeck();
		dealCards();
	}
}