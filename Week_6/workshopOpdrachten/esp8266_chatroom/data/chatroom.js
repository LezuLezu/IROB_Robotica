
document.getElementById("message").value = "";
let dataObject = {};
loadCSV();

const input = document.getElementById("message");
input.addEventListener("keyup", (event) => {
    if (event.key === "Enter") {
        sendMessage();
    }
});

function loadCSV() {
    let xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            parseCSV(this.responseText)
            console.log(dataObject);
            insertParagraph(dataObject);
        }
    };
    xmlhttp.open("GET", "chat.csv", true);
    xmlhttp.send();
}

function parseCSV(string) {
    let object = {};
    let lines = string.split("\n");
    for (let i = 0; i < lines.length; i++) {
        let lineData = lines[i].split(",", 3);
        dataObject[i] = lineData;
    }
}

// functrion to inserts paragraphs into chat section
function insertParagraph(dataObject) {
    let chatSection = document.getElementById("chat");
    
    for (let i = 0; i < Object.keys(dataObject).length; i++) {
        if(dataObject[i][2] != null) {
            let textElement = document.createElement("p");
            textElement.className = "chat__message";
            let text = document.createTextNode(`${dataObject[i][0]} - ${dataObject[i][1]}: ${dataObject[i][2]}`);
            textElement.appendChild(text);
            chatSection.appendChild(textElement);
        }
    }
    chatSection.scrollTop = chatSection.scrollHeight - chatSection.clientHeight;
}

// function to send message to csv on submit
function sendMessage() {
    let message = document.getElementById("message").value.toString();
    let today = new Date();
    let date = today.getDate() + "/" + today.getMonth() + "/" + today.getFullYear();
    let time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    
    let data = {date: date, time: time, message: message};
    let dataString = JSON.stringify(data);

    let xmlhttp = new XMLHttpRequest();
    xmlhttp.open("POST", "/upload", true);
    xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

    xmlhttp.send(dataString);

    setTimeout(() => {
        window.location.reload()
    }, 2000);
}