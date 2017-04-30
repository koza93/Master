#include "ServerThread.h"


ServerThread::ServerThread(int ID, QObject * parent) :QThread(parent)
{

	this->socketDescriptor = ID;
	this->setParent(parent);


	//create thread instead of concurrent thread?
	//gThread = gameThread;

}

void ServerThread::run()
{
	qDebug() << "Starting new thread:" << this->socketDescriptor;
	socket = new QTcpSocket();
	if (!socket->setSocketDescriptor(this->socketDescriptor))
	{
		emit error(socket->error());
		return;
	}
	qDebug() << "run" << QThread::currentThreadId();
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
	connect(this, SIGNAL(doSignal()), this, SLOT(getSignal()), Qt::DirectConnection);
	
	//connect(this, SIGNAL(sendMessageSig()), this, SLOT(sendMessage()), Qt::DirectConnection);

	qDebug() << "Client connected:" << this->socketDescriptor;

	while (numberOfClients < numberOfPlayersToStart)
	{

	}
	delay(1000);
	
	//Start the Game
	QString StartGameMsg = "GameStarted:"+ QString::number(numberOfClients) + ":" + QString::number(currentPlayer);
	qDebug()<< StartGameMsg;
	//sendMessage(StartGameMsg);
	emit doSignal();
	//while game is on
	delay(1000); //delay necessary to accomodate socket data exchange
	while (!isGameFinished) {

		//while playing pre flop
		while (!isPreFlopFinished) {
			checkInputsFromServer();		
		}
		
		//while playing flop
		while (!isFlopFinished) {
			checkInputsFromServer();
		}

		//while playing turn
		while (!isTurnFinished) {
			checkInputsFromServer();
		}

		//while playing river
		while (!isRiverFinished) {
			checkInputsFromServer();
		}
		//isGameFinished = true;
	}

	//if game finished notify the clients
	sendMessage("EndGame:" + QString::number(winner));			//winner ID
	delay(100);
	qDebug() << "I have got out of while isGameFinished loop";
	this->quit();
	exec();
}

void ServerThread::readyRead()
{
	QByteArray Data = socket->readAll();
	qDebug() << "I am receiving:" << Data << "  ME=" << this->socketDescriptor;
	QString dataToString = Data;
	QStringList arrayOfData = dataToString.split(':');
	QByteArray threadNumber = QByteArray::number(this->socketDescriptor, 10);
	if (Data == "WhoAmI") {
		sendMessage("Thread:" + threadNumber);
	}
	if (arrayOfData[0] == "Check")
	{
		//if current thread made bet (problem solved: current thread must have made bet because it the only socket that is allowed to bet in pok app)
		if (arrayOfData[1] == QString::number(this->socketDescriptor))
		{
			emit notifyOnCheck(this->socketDescriptor);
			delay(200);
			emit notifyOnBet();
		}
	}
	if (arrayOfData[0] == "Raise")
	{
		//if current thread made bet (problem solved: current thread must have made bet because it the only socket that is allowed to bet in pok app)
		if (arrayOfData[1] == QString::number(this->socketDescriptor))
		{
			if (arrayOfData[2] != 0)
			{
				betAmount = arrayOfData[2].toInt();
				emit notifyOnRaise(this->socketDescriptor, betAmount);
				qDebug() << betAmount;
				delay(200);
				emit notifyOnBet();
			}
		}
	}
	if (arrayOfData[0] == "Call")
	{
		//if current thread made bet (problem solved: current thread must have made bet because it the only socket that is allowed to bet in pok app)
		if (arrayOfData[1] == QString::number(this->socketDescriptor))
		{
			emit notifyOnCall(this->socketDescriptor); //notifies that this player called a raise
			delay(200);
			emit notifyOnBet();
		}
	}
	if (arrayOfData[0] == "Fold")
	{
		//if current thread made bet (problem solved: current thread must have made bet because it the only socket that is allowed to bet in pok app)
		if (arrayOfData[1] == QString::number(this->socketDescriptor))
		{
			emit notifyOnFold(this->socketDescriptor); //notifies that this player called a raise
			delay(200);
			emit notifyOnBet();
		}
	}

	/*/
	else
		socket->write("Recived:" + Data);
	qDebug() <<"readready"<< QThread::currentThreadId();
	*/
}

void ServerThread::disconnected()
{
	qDebug() << "Client disconnected:" << this->socketDescriptor;
	socket->deleteLater();
	exit(0);
}

void ServerThread::sendMessage(QString msg) {
	QByteArray threadNumber = QByteArray::number(this->socketDescriptor, 10);
	
	qDebug() << "Message from: " << this->socketDescriptor << ";;" << socket->socketDescriptor() << "The msg: " <<msg;
	//socket->setParent(this);
	QByteArray message = msg.toStdString().c_str();
	//QByteArray Data = socket->readAll();
	socket->write(message);

}
void ServerThread::updateNumberClients(int num) {
	numberOfClients = num;
	qDebug() << "numClients" << QThread::currentThreadId();
	qDebug() << "Thread:" << this->socketDescriptor << "Number of clients: " << numberOfClients;
	//if (numberOfClients > 0)
	//{
	//	//emit sendMessage();
	//}
}

void ServerThread::updateNoOfPlayersToStartGame(int num) {
	numberOfPlayersToStart = num;
}

void ServerThread::updateCurrentPlayer(int num) {
	currentPlayer = num;
	qDebug() << "numClients" << QThread::currentThreadId();
	qDebug() << "Thread:" << this->socketDescriptor << "Current player: " << currentPlayer;
	
}

void ServerThread::updateAllPlayers(QVector<int> v, QVector<int> tC, int bb, int sb) {
	allPlayersUpdated = true;
	qDebug() << "Updating all player thread numbers";
	allPlayerNumbers = v;
	allPlayersTC = tC;
	currentBB = bb;
	currentSB = sb;
}

void ServerThread::updateBetMade(bool c) {
	if (!isGameFinished) {
		canCheck = c;
		betMade = true;
		qDebug() << "Thread:" << this->socketDescriptor << "BetMade ";
	}
}

void ServerThread::updateRaiseMade(int playerNo, int amount, int tC, int tP) {
	previousPlayer = playerNo;
	betRaised = true;
	allBetAmount = amount;
	totalChips = tC;
	totalPot = tP;
	qDebug() << "Thread:" << this->socketDescriptor << "RaiseMade By: " <<playerNo << "Amount: " << amount << "TotalChips: " << tC;
	
}

void ServerThread::updateCheckMade(int playerNo) {
	if (!isGameFinished) {
		previousPlayer = playerNo;
		betChecked = true;
		qDebug() << "Thread:" << this->socketDescriptor << "CheckMade By: " << playerNo;
	}
}

void ServerThread::updateCallMade(int playerNo , int amount, int tC, int tP) {
	if (!isGameFinished) {
		previousPlayer = playerNo;
		betCalled = true;
		allBetAmount = amount;
		totalChips = tC;
		totalPot = tP;
		qDebug() << "Thread:" << this->socketDescriptor << "CallMade By: " << playerNo << "TotalChips: " << tC;
	}
}

void ServerThread::updateFoldMade(int playerNo) {
	if (!isGameFinished) {
		previousPlayer = playerNo;
		betFolded = true;
		qDebug() << "Thread:" << this->socketDescriptor << "CallMade By: " << playerNo;
	}
}

void ServerThread::updateMyCurrentHand(Card* c1, Card* c2, int sc)
{
	
	if (socketDescriptor == sc)
	{
		myCurrentCards[0] = c1;
		myCurrentCards[1] = c2;

		qDebug() << "I have cards: " << myCurrentCards[0]->getFigure() << myCurrentCards[0]->getSuit() << "and " << myCurrentCards[1]->getFigure() << myCurrentCards[1]->getSuit();
		handDealt = true;
	}
	
}

void ServerThread::updateCardsOnTable(Card** cards)
{
	//cardsOnTable[0] = cards[0];
	qDebug() << "CardsOnTable";
	for (int i = 0; i < 5; i++)
	{
		cardsOnTable[i] = cards[i];
		qDebug() << "I have cards: " << cardsOnTable[i]->getFigure() << cardsOnTable[i]->getSuit();
	}
}

void ServerThread::updateOnWin(int win, QString hand)
{
	winner = win;
	winnersHand = hand;
	qDebug() << "winner is: " << winner;
	isWinner = true;	
}

void ServerThread::updateOnDraw(int win, QVector<int> pToUpdate)
{
	drawers = pToUpdate;
	qDebug() << "DRAAAAAAAAAAAAAW";
	isDraw = true;
}

void ServerThread::updateOnGameEnd(int win)
{
	winner = win;
	qDebug() << "allwinner is: " << winner;
	isGameFinished = true;
}

void ServerThread::delay(int millisecondsToWait)
{
	QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
	while (QTime::currentTime() < dieTime)
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
void ServerThread::getSignal() {	
	sendMessage("GameStarted:"+ QString::number(numberOfClients) + ":" + QString::number(currentPlayer));
	delay(200);
}

void ServerThread::changeGameStage(int gameStage, QVector<int> pToUpdate)
{  //1 = preflop, 2= flop ...
	qDebug() << "got the SIGNALLLLLLLLL";

	playersToUpdate = pToUpdate;

	//0 will happen on a reset from server
	if (gameStage == 0) {
		flopDealt = false;
		isPreFlopFinished = false;
		turnDealt = false;
		isFlopFinished = false;
		riverDealt = false;
		isTurnFinished = false;
		isRiverFinished = false;

		refreshDealt = true;
	}
	if (gameStage == 1) {
		flopDealt = true;
		isPreFlopFinished = true;
		qDebug() << "changed prefloppppp";
	}
	if (gameStage == 2) {
		turnDealt = true;
		isFlopFinished = true;
		qDebug() << "changed flop";
	}
	if (gameStage == 3) {
		riverDealt = true;
		isTurnFinished = true;
		qDebug() << "changed turn";
	}
	if (gameStage == 4) {
		isRiverFinished = true;
		qDebug() << "changed river";
	}
	//sendMessage("GameStarted:" + QString::number(numberOfClients) + ":" + QString::number(currentPlayer));
}

void ServerThread::checkInputsFromServer()
{
	//TODO - At some stage put all those if statements into a function
	delay(500);

	if (isWinner == true) {
		isWinner = false;

		qDebug() << "Win:" << QString::number(winner) <<":"<< winnersHand;
		sendMessage("Win:" + QString::number(winner) + ":" + winnersHand);
		delay(8000);
	}
	if (isDraw == true) {
		isDraw = false;
		QString drawStr = "Draw:"+ QString::number(drawers.size());
		for (int i = 0; i < drawers.size(); i++) {	
			drawStr += ":"+ QString::number(drawers.at(i));
		}
		qDebug() << drawStr;
		drawers.clear();
		sendMessage(drawStr);
		delay(8000);
	}
	if (allPlayersUpdated == true) {
		QString msg = "AllPlayers";
		for (int i = 0; i < allPlayerNumbers.size(); ++i) {
			msg += ":"+ QString::number(allPlayerNumbers.at(i));
		}
		sendMessage(msg);
		allPlayersUpdated = false;
		delay(500);
		for (int i = 0; i < allPlayerNumbers.size(); ++i) {
			if (allPlayerNumbers.at(i) == currentBB ) {
				sendMessage("Raise:" + QString::number(currentBB) + ":" + QString::number(bigBlind) + ":" + QString::number(allPlayersTC.at(i)) + ":75");		//TODO get an actual number for totalchips from server
				delay(500);
			}
		}
		
		//delay(500);

		for (int i = 0; i < allPlayerNumbers.size(); ++i) {
			if (allPlayerNumbers.at(i) == currentSB) {
				sendMessage("Raise:" + QString::number(currentSB) + ":" + QString::number(smallBlind) + ":"  + QString::number(allPlayersTC.at(i)) + ":75");	//up
				delay(500);
			}
		}
		
		//delay(500);

		for (int i = 0; i < allPlayerNumbers.size(); ++i) {
			if (allPlayerNumbers.at(i) != currentBB && allPlayerNumbers.at(i) != currentSB) {
				sendMessage("Raise:" + QString::number(allPlayerNumbers.at(i)) + ":" + "0:" + QString::number(allPlayersTC.at(i)) + ":75");
				delay(500);
			}		
		}
		allPlayersTC.clear();
		
	}
	if (betMade == true) {
		betMade = false;
		QString msg;
		if (canCheck == true)
		{
			msg = "ChangeTurn:" + QString::number(currentPlayer) + ":CanCheck:1";
			qDebug() << "The msg is:" << msg;
			sendMessage(msg);
		}
		else
		{
			msg = "ChangeTurn:" + QString::number(currentPlayer) + ":CanCheck:0";
			qDebug() << "The msg is:" << msg;
			sendMessage(msg);
		}
		delay(200);
	}

	if (betFolded == true)
	{
		sendMessage("Fold:" + QString::number(previousPlayer));
		betFolded = false;
		delay(200);
	}
	if (betRaised == true)
	{
		sendMessage("Raise:" + QString::number(previousPlayer) + ":" + QString::number(allBetAmount)+ ":" + QString::number(totalChips)+ ":" + QString::number(totalPot));///
		betRaised = false;
		delay(200);
	}
	if (betCalled == true)
	{
		sendMessage("Raise:" + QString::number(previousPlayer) + ":" + QString::number(allBetAmount) + ":" + QString::number(totalChips) + ":" + QString::number(totalPot));///
		betCalled = false;
		delay(200);
	}
	if (betChecked == true)
	{
		sendMessage("Check:" + QString::number(previousPlayer));
		betChecked = false;
		delay(200);
	}
	
	if (handDealt == true) {
		handDealt = false;
		QString msg;
		msg = "HandCards:" + QString(QChar::fromLatin1(myCurrentCards[0]->getSuit())) + QString(QChar::fromLatin1(myCurrentCards[0]->getFigure())) 
			+ ":" + QString(QChar::fromLatin1(myCurrentCards[1]->getSuit())) + QString(QChar::fromLatin1(myCurrentCards[1]->getFigure()));
		qDebug() << msg;
		sendMessage(msg);
		delay(200);
	}

	if (flopDealt == true) {
		flopDealt = false;
		//send first 3 cards
		QString msg;
		msg = "FlopCards:" + QString(QChar::fromLatin1(cardsOnTable[0]->getSuit())) + QString(QChar::fromLatin1(cardsOnTable[0]->getFigure()))
			+ ":" + QString(QChar::fromLatin1(cardsOnTable[1]->getSuit())) + QString(QChar::fromLatin1(cardsOnTable[1]->getFigure()))
			+ ":" + QString(QChar::fromLatin1(cardsOnTable[2]->getSuit())) + QString(QChar::fromLatin1(cardsOnTable[2]->getFigure()));
		sendMessage(msg);
		delay(200);

		for (int i = 0; i < playersToUpdate.size(); ++i) {
			sendMessage("Update:" + QString::number(playersToUpdate.at(i)) + ":0:NA:NA");
			delay(200);
		}
		
	}

	if (turnDealt == true) {
		turnDealt = false;
		//send first 3 cards
		QString msg;
		msg = "TurnCard:" + QString(QChar::fromLatin1(cardsOnTable[3]->getSuit())) + QString(QChar::fromLatin1(cardsOnTable[3]->getFigure()));
		sendMessage(msg);
		delay(200);

		for (int i = 0; i < playersToUpdate.size(); ++i) {
			sendMessage("Update:" + QString::number(playersToUpdate.at(i)) + ":0:NA:NA" );			// will have a separate label for totalchips in javascript so a dummy variable will do (not gonna update tc in js)
			delay(200);
		}
	}
	if (riverDealt == true) {
		riverDealt = false;
		//send first 3 cards
		QString msg;
		msg = "RiverCard:" + QString(QChar::fromLatin1(cardsOnTable[4]->getSuit())) + QString(QChar::fromLatin1(cardsOnTable[4]->getFigure()));
		sendMessage(msg);
		delay(200);

		for (int i = 0; i < playersToUpdate.size(); ++i) {
			sendMessage("Update:" + QString::number(playersToUpdate.at(i)) + ":0:NA:NA");
			delay(200);
		}
	}

	if (refreshDealt == true) {
		refreshDealt = false;
		//send message to reset cards
		QString msg;
		msg = "Refresh:Now"; //second argument is a dummy argument
		sendMessage(msg);
		delay(200);

		/*
		for (int i = 0; i < playersToUpdate.size(); ++i) {
			sendMessage("Update:" + QString::number(playersToUpdate.at(i)) + ":0");
			delay(200);
		}*/
	}
}

