$(document).ready(function () {

    new QWebChannel(qt.webChannelTransport, function (channel) {
       // now you retrieve your object
        window.pokerOperations = channel.objects.pokerOperations;
      

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
        //$("p").hide();
        pokerOperations.sendCheckButtonClicked();
            

    });

    $("#raiseButton").click(function () {
        //$("p").hide();
        pokerOperations.sendRaiseButtonClicked();


    });

    $("#foldButton").click(function () {
        //$("p").hide();
        pokerOperations.sendFoldButtonClicked();


    });

    $("#callButton").click(function () {
        //$("p").hide();
        pokerOperations.callFoldButtonClicked();


    });
   
   
    // jQuery methods go here...

});