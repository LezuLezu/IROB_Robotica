dataArray = [];
loadCSV();
insertParagraph(dataArray);

function loadCSV() {
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            dataArray = parseCSV(this.responseText);
        }
    };
    xmlhttp.open("GET", "chat.csv", true);
    xmlhttp.send();
}

function parseCSV(string) {
    var array = [];
    var lines = string.split("\n");
    for (var i = 0; i < lines.length; i++) {
        var data = lines[i].split(",", 3);
        // var data = lines[i].split(",", 2); // if only 2 columns
        data[0] = data[0].toString();
        //  parse input to string
        data[1] = data[1].toString();
        data[2] = data[2].toString();
        // daya[1], data[2] = data[1].split(":"); // split username and message if seperated by ":"
        array.push(data);
    }
    return array;
}

// functrion to inserts paragraphs into chat section
function insertParagraph(dataArray) {
    var chatSection = element.getElementById("chat");
    for (var i = 0; i < dataArray.length; i++) {
        // create p element with class 'chat__message'
        var p = document.createElement("p");
        p.className = "chat__message";
        // add line to p element
        var innerHTMLString = dataArray[i][0] + " " + dataArray[i][1] + ": " + dataArray[i][2];
        p.innerHTML = innerHTMLString;
        // add p element to chat section
        chatSection.appendChild(p);
    }
    chatSection.scrollTop = chatSection.scrollHeight - chatSection.clientHeight;
}

// function to send message to csv on submit
function sendMessage() {
    var message = document.getElementById("message").value.toString();
    var today = new Date();
    var date = today.getDate() + "/" + today.getMonth() + "/" + today.getFullYear();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var dateTime = date + " " + time;

    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("POST", "/chat.csv", true);
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.send("dateTime=" + dateTime + "&message=" + message);


}