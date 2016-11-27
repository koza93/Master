$(document).ready(function () {

    new QWebChannel(qt.webChannelTransport, function (channel) {
       // now you retrieve your object
        window.pokerOperations = channel.objects.pokerOperations;
      

        pokerOperations.webChannelTest("Created JS Object");
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
                $("#userNameLabel").css("visibility","hidden");
            }
        });

    });

    
    // jQuery methods go here...

});