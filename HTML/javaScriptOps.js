$(document).ready(function () {
    var myChips = 0;
    var biggestBet = 0;
    var myCurrentBet = 0;
    var user;
    new QWebChannel(qt.webChannelTransport, function (channel) {
       // now you retrieve your object
        window.pokerOperations = channel.objects.pokerOperations;
      
        //varaibles
        var numOfPlayers;
        var myPlayerNumber;         //thread number
        var myArrayNumber           //number in the array
        var handCards;
        var flopCards;
        var arrayOfPlayers = {};
        var seatArray = {};
        
        pokerOperations.webChannelTest("Created JS Object");

        //notifies whenever myturn
        pokerOperations.notifyOnTurn.connect(function (returnValue) {
            if (returnValue === true) {
                $(".theBetting").css("visibility", "visible");
            }
            else if (returnValue === false) {
                $(".theBetting").css("visibility", "hidden");
            }
        });
        pokerOperations.notifyOnAllPlayersConnected.connect(function (returnValue, returnValue1) {
            arrayOfPlayers[returnValue] = returnValue1;
        });


        pokerOperations.notifyOnBet.connect(function (playerID, chips, totalChips,totalPot) {
            
            document.getElementById('totalPot').innerHTML = '<label>'+ "TotalPot: " +totalPot + '</label>';
            var currentNo;
            if (playerID == myPlayerNumber) {
                document.getElementById('seatTaken6').innerHTML = '<img src="Seat3.png" style="width:10.3vw;height:14vh;">';
                document.getElementById('totalChips6').innerHTML = '<label>Chips: ' + totalChips + '</label>';
                document.getElementById('playerName6').innerHTML = '<label>' + playerID + '</label>';
                document.getElementById('bets6').innerHTML = '<label>Bet:' + chips + '</label>';
                myChips = totalChips;
                myCurrentBet = chips;
            }
            else {
                for (var i = 0; i < 5; i++) {
                    if (playerID === arrayOfPlayers[i]) {
                        currentNo = i;
                        //alert(currentNo);
                        for (var j = 0; j < 5; j++) {
                            if (seatArray[j] == i) {
                                document.getElementById('seatTaken' + (j + 1)).innerHTML = '<img src="Seat2.png" style="width:10.3vw;height:14vh;">';
                                document.getElementById('totalChips' + (j + 1)).innerHTML = '<label>Chips: ' + totalChips + '</label>';
                                document.getElementById('playerName' + (j + 1)).innerHTML = '<label>' + arrayOfPlayers[i] + '</label>';
                                document.getElementById('bets' + (j + 1)).innerHTML = '<label>Bet: ' + chips + '</label>';
                                if (chips > biggestBet) {
                                    biggestBet = chips;
                                }
                            }
                        }
                    }
                }
            }
        });

        pokerOperations.notifyOnWin.connect(function (winnerID, combination) {

            $(".theBetting").css("visibility", "hidden");
            if(winnerID == myPlayerNumber)
                document.getElementById('totalPot').innerHTML = '<label>' + "You win! " + combination +'</label>';
            else
                document.getElementById('totalPot').innerHTML = '<label>' + "Winner is:" + winnerID +" " + combination+'</label>';
            for (var i = 6; i <= 7; i++) {
                (function (index) {
                    setTimeout(function () {
                        var cards = "card" + index;
                        document.getElementById(cards).innerHTML = '<img src="Cards.png" style="width:7.8vw;height:15vh;">';
                        $(cards).css("visibility", "visible");
                    }, i * 10);
                })(i);
            }
        
        });

        pokerOperations.notifyOnDraw.connect(function (flag, message) {

            $(".theBetting").css("visibility", "hidden");
            if (flag  === 1)
                document.getElementById('totalPot').innerHTML = '<label>' + "I " + message+ '</label>';
            else
                document.getElementById('totalPot').innerHTML = '<label>' + "You lose " + message + '</label>';
            for (var i = 6; i <= 7; i++) {
                (function (index) {
                    setTimeout(function () {
                        var cards = "card" + index;
                        document.getElementById(cards).innerHTML = '<img src="Cards.png" style="width:7.8vw;height:15vh;">';
                        $(cards).css("visibility", "visible");
                    }, i * 10);
                })(i);
            }

        });

        pokerOperations.notifyOnUpdate.connect(function (playerID, chips, totalChips, totalPot) {

            
            var currentNo;
            if (playerID == myPlayerNumber) {
                document.getElementById('seatTaken6').innerHTML = '<img src="Seat3.png" style="width:10.3vw;height:14vh;">';
                //document.getElementById('totalChips6').innerHTML = '<label>Chips: ' + totalChips + '</label>';
                document.getElementById('playerName6').innerHTML = '<label>' + playerID + '</label>';
                document.getElementById('bets6').innerHTML = '<label>Bet: ' + chips + '</label>';
                //myChips = totalChips;
                biggestBet = 0;
                myCurrentBet = 0;
            }
            else {
                for (var i = 0; i < 5; i++) {
                    if (playerID === arrayOfPlayers[i]) {
                        currentNo = i;
                        //alert(currentNo);
                        for (var j = 0; j < 5; j++) {
                            if (seatArray[j] == i) {
                                document.getElementById('seatTaken' + (j + 1)).innerHTML = '<img src="Seat2.png" style="width:10.3vw;height:14vh;">';
                                //document.getElementById('totalChips' + (j + 1)).innerHTML = '<label>Chips: ' + totalChips + '</label>';
                                document.getElementById('playerName' + (j + 1)).innerHTML = '<label>' + arrayOfPlayers[i] + '</label>';
                                document.getElementById('bets' + (j + 1)).innerHTML = '<label>Bet: ' + chips + '</label>';
                            }
                        }
                    }
                }
            }
        });


        //notifies whenever myturn
        pokerOperations.notifyOnGameStarted.connect(function (returnValue) {
            if (returnValue === true) {
                $(".theGameplay").css("visibility", "visible");
                //to set timeout
                pokerOperations.getNumberOfPlayers(function (returnValue) {
                numOfPlayers = returnValue;

                    //show backcards
                    for (var i = 1; i <= 5; i++) {
                        (function (index) {
                            setTimeout(function () {
                                var cards = "#card" + index;
                                $(cards).css("visibility", "visible");
                            }, i * 200);
                        })(i);
                    }
                    alert("Game Started");
                });
                //gets the player thread number from the c++ code
                pokerOperations.getPlayerNumber(function (returnValue) {
                    myPlayerNumber = returnValue;
                });
            }
        });


        //notify of total chips
        pokerOperations.notifyOnAssignId.connect(function (returnValue) {
            if (returnValue === true) {
                $("#totalPot").css("visibility", "visible");
                for (var i = 0; i < numOfPlayers; i++) {

                    if (myPlayerNumber == arrayOfPlayers[i]) {
                        myArrayNumber = i;
                        // alert(myArrayNumber);
                    }
                }
                window.tempMyArrayNumber = myArrayNumber;
                for (var i = 0; i < 5 ; i++) {
                    if ((tempMyArrayNumber + 1 + i) === 6) {
                        tempMyArrayNumber = myArrayNumber-6;
                    }
                    
                    seatArray[i] = tempMyArrayNumber + 1 + i;
                   
                    //alert(i + " " + seatArray[i]);
                    //alert(seatArray[i] + "=" + (tempMyArrayNumber + 1 + i));
                }

                //show players connected
                //document.getElementById('seatTaken6').innerHTML = '<img src="Seat3.png" style="width:10.3vw;height:14vh;"><label>' + arrayOfPlayers[myArrayNumber] + '</label>';
                document.getElementById('seatTaken6').innerHTML = '<img src="Seat3.png" style="width:10.3vw;height:14vh;">';
                document.getElementById('playerName6').innerHTML = '<label>' + arrayOfPlayers[myArrayNumber] + '</label>';
                $("#totalChips6").css("visibility", "visible");
                $("#playerName6").css("visibility", "visible");
                $("#bets6").css("visibility", "visible");

                $("#seatTaken6").css("visibility", "visible");
                //alert(numOfPlayers);
                for (var i = 0; i < numOfPlayers; i++) {
                    for (var j = 0 ; j < 5; j++) {
                        if (seatArray[j] === i) {
                            var seat = "#seatTaken" + (j + 1);

                            //alert(seat);
                            //document.getElementById('seatTaken' + (j + 1)).innerHTML = '<img src="Seat2.png" style="width:10.3vw;height:14vh;"><label>' + arrayOfPlayers[i] + '</label>';
                            document.getElementById('seatTaken' + (j + 1)).innerHTML = '<img src="Seat2.png" style="width:10.3vw;height:14vh;">';
                            document.getElementById('playerName' + (j + 1)).innerHTML = '<label>' + arrayOfPlayers[i] + '</label>';
                            $("#totalChips" + (j + 1)).css("visibility", "visible");
                            $("#playerName" + (j + 1)).css("visibility", "visible");
                            $("#bets" + (j + 1)).css("visibility", "visible");
                            $(seat).css("visibility", "visible");
                            
                        }
                    }
                }
            }
        });

        //notifies hand is refreshed
        pokerOperations.notifyOnRefreshDealt.connect(function (returnValue) {
            if (returnValue === true) {
                //gets the cards dealt
                //show backcards
                for (var i = 1; i <= 5; i++) {
                    (function (index) {
                        setTimeout(function () {
                            var cards = "card" + index;
                            document.getElementById(cards).innerHTML = '<img src="Cards.png" style="width:7.8vw;height:15vh;">';
                            $(cards).css("visibility", "visible");
                        }, i * 50);
                    })(i);
                }
            }
        });

        //notifies hand is dealt
        pokerOperations.notifyOnHandDealt.connect(function (returnValue) {
            if (returnValue === true) {
                //gets the cards dealt
                for (var i = 0; i < 2; i++) {
                    (function (index) {
                        setTimeout(function () {
                            pokerOperations.getHandCards(index, function (returnValue) {
                                handCards = returnValue;
                                var cards = 'card' + (6 + index);
                                //alert(cards);
                                document.getElementById(cards).innerHTML = '<img src="Cards' + handCards + '.png" style="width:7.8vw;height:15vh;">';
                                cards = "#card" + (6+index);
                                $(cards).css("visibility", "visible");
                            });
                        }, i * 200);
                    })(i);
                }
            }
        });

        //notifies hand is dealt
        pokerOperations.notifyOnFlopDealt.connect(function (returnValue) {
            if (returnValue === true) {
                //gets the cards dealt
                for (var i = 0; i < 3; i++) {
                    (function (index) {
                        setTimeout(function () {
                            pokerOperations.getTableCards(index, function (returnValue) {
                                flopCards = returnValue;
                                var cards = 'card' + (1 + index);
                                //alert(flopCards);
                                document.getElementById(cards).innerHTML = '<img src="Cards' + flopCards + '.png" style="width:7.8vw;height:15vh;">';
                                cards = "#card" + (1 + index);
                            });
                        }, i * 200);
                    })(i);
                }
            }
        });

        pokerOperations.notifyOnTurnDealt.connect(function (returnValue) {
            if (returnValue === true) {
                //gets the cards dealt
                pokerOperations.getTableCards(3, function (returnValue) {
                    flopCards = returnValue;
                    var cards = 'card4';
                    //alert(flopCards);
                    document.getElementById(cards).innerHTML = '<img src="Cards' + flopCards + '.png" style="width:7.8vw;height:15vh;">';
                    cards = "#card4";
                });
            }
        });

        pokerOperations.notifyOnRiverDealt.connect(function (returnValue) {
            if (returnValue === true) {
                //gets the cards dealt
                pokerOperations.getTableCards(4, function (returnValue) {
                    flopCards = returnValue;
                    var cards = 'card5';
                    //alert(flopCards);
                    document.getElementById(cards).innerHTML = '<img src="Cards' + flopCards + '.png" style="width:7.8vw;height:15vh;">';
                    cards = "#card5";
                });
            }
        });

        //notifies if player can check or call

        pokerOperations.notifyOnCanCheck.connect(function (returnValue,returnValue2) {
            if (returnValue === true) {
                $("#callButton").css("visibility", "hidden");
                $("#checkButton").css("visibility", "visible");
            }
            else if (returnValue === false) {
                $("#callButton").css("visibility", "visible");
                $("#checkButton").css("visibility", "hidden");
            }
        });

        pokerOperations.notifyOnEndGame.connect(function (winner) {
           $(".theGameplay").css("visibility", "hidden");
           $(".theTable").css("visibility", "hidden");
           $(".console").css("visibility", "hidden");
           $(".theBetting").css("visibility", "hidden");
           $("#seatTaken6").css("visibility", "hidden");
           $("#seatTaken5").css("visibility", "hidden");
           $("#seatTaken4").css("visibility", "hidden");
           $("#seatTaken3").css("visibility", "hidden");
           $("#seatTaken2").css("visibility", "hidden");
           $("#seatTaken1").css("visibility", "hidden");
           $("#card1").css("visibility", "hidden");
           $("#card2").css("visibility", "hidden");
           $("#card3").css("visibility", "hidden");
           $("#card4").css("visibility", "hidden");
           $("#card5").css("visibility", "hidden");
           $("#card6").css("visibility", "hidden");
           $("#card7").css("visibility", "hidden");
           $("#totalChips1").css("visibility", "hidden");
           $("#totalChips2").css("visibility", "hidden");
           $("#totalChips3").css("visibility", "hidden");
           $("#totalChips4").css("visibility", "hidden");
           $("#totalChips5").css("visibility", "hidden");
           $("#totalChips6").css("visibility", "hidden");
           $("#totalChips7").css("visibility", "hidden");
           $("#bets1").css("visibility", "hidden");
           $("#bets2").css("visibility", "hidden");
           $("#bets3").css("visibility", "hidden");
           $("#bets4").css("visibility", "hidden");
           $("#bets5").css("visibility", "hidden");
           $("#bets6").css("visibility", "hidden");
           $("#bets7").css("visibility", "hidden");
           $("#playerName1").css("visibility", "hidden");
           $("#playerName2").css("visibility", "hidden");
           $("#playerName3").css("visibility", "hidden");
           $("#playerName4").css("visibility", "hidden");
           $("#playerName5").css("visibility", "hidden");
           $("#playerName6").css("visibility", "hidden");
           $("#playerName7").css("visibility", "hidden");
           $("#checkButton").css("visibility", "hidden");
           $("#callButton").css("visibility", "hidden");
           $("#totalPot").css("visibility", "hidden");
           
           for (var i = 1; i <= 7; i++) {
               (function (index) {
                   setTimeout(function () {
                       var cards = "card" + index;
                       document.getElementById(cards).innerHTML = '<img src="Cards.png" style="width:7.8vw;height:15vh;">';
                       $(cards).css("visibility", "hidden");
                       var seats = "seatTaken" + index;
                       if(index != 7)
                       $(seats).css("visibility", "hidden");
                   }, i * 50);
               })(i);
           }

           $(".winScreen").css("visibility", "visible");

           if (winner === myPlayerNumber) {
               document.getElementById('winningMsg').innerHTML = '<label>' +'You have won, congratulations'+ '</label>';
           }
           else {
               document.getElementById('winningMsg').innerHTML = '<label>' + 'Player ' +winner +' has won, we wish you better luck next time!'+ '</label>';
           }

           numOfPlayers=0;
           myPlayerNumber=0;         //thread number
           myArrayNumber = 0;           //number in the array
           handCards=0;
           flopCards=0;
           arrayOfPlayers = {};
           seatArray = {};

           
        });
    });

    $("#loginBtn").click(function () {
        //$("p").hide();
        var username = document.getElementById('myUsername').value;
        var password = document.getElementById('myPassword').value;
        pokerOperations.webChannelTest(username + password);
       // pokerOperations.checkUserAndPassword(username, password);
        pokerOperations.checkUserAndPassword(username, password, function (returnValue) {
            var isPasswordValid = returnValue;

            if(isPasswordValid === true)
            {
                user = username;
                //pokerOperations.webChannelTest("valid password");
                $(".login").css("visibility","hidden");
                $(".joinTable").css("visibility", "visible");
                $("#warning").text("");
                $("#userNameLabel").text("Welcome " + username + ".");
                $("#userNameLabel2").text("What do you want to do?");
            }
            else {
                $("#warning").text("Invalid username/password or user logged in already...");
            }
        });
    });

    $("#joinBtn").click(function () {
        //$("p").hide();
        pokerOperations.webChannelTest("clicked joinTable button");
        // pokerOperations.checkUserAndPassword(username, password);
        pokerOperations.joinTable( function (returnValue) {
            var isTableJoined = returnValue;

            if (isTableJoined === true) {
                //pokerOperations.webChannelTest("valid password");
                
                $(".joinTable").css("visibility", "hidden");
                $("#userNameLabel").css("visibility", "hidden");
                $(".console").css("visibility", "visible");
                $(".theTable").css("visibility", "visible");
                alert("joined Table waiting for players");

            }
        });

    });

    $("#statsBtn").click(function () {
        //$("p").hide();
        pokerOperations.webChannelTest("clicked stat button");
        // pokerOperations.checkUserAndPassword(username, password);
        pokerOperations.getStats(user, function (tString) {
            $(".joinTable").css("visibility", "hidden");
            $(".theStats").css("visibility", "visible");
            $("#stats").text(tString);

        });

    });

    $("#chipsBtn").click(function () {
        //$("p").hide();
        pokerOperations.webChannelTest("clicked chips button");
        // pokerOperations.checkUserAndPassword(username, password);
        pokerOperations.addChips(user, function () {
        });

    });


    $("#exitButton").click(function () {
        $(".winScreen").css("visibility", "hidden");
        $(".joinTable").css("visibility", "visible");

    });

    $("#exitButton2").click(function () {
        $(".theStats").css("visibility", "hidden");
        $(".joinTable").css("visibility", "visible");

    });

    $("#checkButton").click(function () {
        pokerOperations.sendCheckButtonClicked();
        $("#checkButton").css("visibility", "hidden");
        $("#callButton").css("visibility", "hidden");
    });

    $("#raiseButton").click(function () {
        //$("p").hide();
        if (document.getElementById('raise').value > 49 && document.getElementById('raise').value <= (myChips-biggestBet+myCurrentBet)) {
            pokerOperations.sendRaiseButtonClicked(document.getElementById('raise').value);
            $("#checkButton").css("visibility", "hidden");
            $("#callButton").css("visibility", "hidden");
        }
    });

    $("#foldButton").click(function () {
        //$("p").hide();
        pokerOperations.sendFoldButtonClicked();
        $("#checkButton").css("visibility", "hidden");
        $("#callButton").css("visibility", "hidden");

    });

    $("#callButton").click(function () {
        //$("p").hide();
        pokerOperations.sendCallButtonClicked();
        $("#callButton").css("visibility", "hidden");
        $("#checkButton").css("visibility", "hidden");


    });
   
   
    // jQuery methods go here...

});