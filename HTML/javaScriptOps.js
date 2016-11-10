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
    });
    // jQuery methods go here...

});