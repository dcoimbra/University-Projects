function goBack() {

    window.history.back();
}

function makeBarInvisible() {
    var i;
    var popup = document.getElementsByClassName("popup");
    for (i = 0; i < popup.length; i++)
        popup[i].style.visibility='hidden';
}

function showBar() {
    var i;
    var popup = document.getElementsByClassName("popup");
    for (i = 0; i < popup.length; i++)
        popup[i].style.visibility='visible';
    move();
}

function move() {
    var elem = document.getElementById("myBar");
    var width = 0;
    var id = setInterval(frame, 10);
    function frame() {
        if (width == 100) {
            clearInterval(id);
            window.location="pessoal.html";
        } else {
            width++;
            elem.style.width = width + '%';
        }
    }
}

function storeUsername() {

    var username = document.getElementById("inputUsername");
    sessionStorage.setItem("username", username.value);
}

function setUsername() {

    var username = sessionStorage.getItem("username");
    
    if (username == null) {

        document.getElementById("header1").innerHTML = "Olá, utilizador!";
    }

    else {
    
        document.getElementById("header1").innerHTML = "Olá, " + username + "!";
    }   
}

function clearUsername() {

    sessionStorage.removeItem("username");
}

function checklogin() {

    var status = sessionStorage.getItem("login");

    if (status === "true") {

        document.getElementById("loginbutton").src = "../images/logout.svg"
    }
}