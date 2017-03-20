$(document).ready(function () {

    new QWebChannel(qt.webChannelTransport, function (channel) {
       // now you retrieve your object
        window.pokerOperations = channel.objects.pokerOperations;
      
        //varaibles
        var myPlayerNumber;
        var handCards;
        var flopCards;
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
        //notifies whenever myturn
        pokerOperations.notifyOnGameStarted.connect(function (returnValue) {
            if (returnValue === true) {
                $(".theGameplay").css("visibility", "visible");
                //to set timeout
                var numOfPlayers;
                pokerOperations.getNumberOfPlayers(function (returnValue) {
                    numOfPlayers = returnValue;

                    //show players connected
                    $("#seatTaken6").css("visibility", "visible");
                    for (var i = 1; i < numOfPlayers; i++) {

                        var seat = "#seatTaken" + i;
                        $(seat).css("visibility", "visible");
                    }
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
                //pokerOperations.webChannelTest("valid password");
                $(".login").css("visibility","hidden");
                $(".joinTable").css("visibility", "visible");
                $("#userNameLabel").text("Welcome "+username + ". What do you want to do?");
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
                $(".theTable").css("visibility", "visible");
                alert("joined Table waiting for players");

            }
        });

    });
    $("#checkButton").click(function () {
        pokerOperations.sendCheckButtonClicked();
        $("#checkButton").css("visibility", "hidden");
        $("#callButton").css("visibility", "hidden");
    });

    $("#raiseButton").click(function () {
        //$("p").hide();
        pokerOperations.sendRaiseButtonClicked();
        $("#checkButton").css("visibility", "hidden");
        $("#callButton").css("visibility", "hidden");

    });

    $("#foldButton").click(function () {
        //$("p").hide();
        pokerOperations.sendFoldButtonClicked();
        $("#checkButton").css("visibility", "hidden");
        $("#callButton").css("visibility", "hidden");

    });

    $("#callButton").click(function () {
        //$("p").hide();
        pokerOperations.callFoldButtonClicked();
        $("#callButton").css("visibility", "hidden");
        $("#checkButton").css("visibility", "hidden");


    });
   
   
    // jQuery methods go here...

});