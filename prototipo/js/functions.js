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
        document.getElementById("loginbutton").src = "../images/area_pessoal_21.png";
        document.getElementById("loginlink").href="pessoal.html";
        $('#loginbutton').attr('data-target','#myModal');
        $('#loginbutton').attr('data-toggle','modal');
    }
}

function preparelogout() {

    $('#loginbutton').attr('data-target','#myModal');
    $('#loginbutton').attr('data-toggle','modal');
}

function login() {

    sessionStorage.setItem('login', 'true');
}

function logout() {

    sessionStorage.setItem('login', 'false');
    getlastpage();
}

function savelastpage() {

    $('#loginbutton').click(function() {
        sessionStorage.setItem('lastpage', window.location.href);
    });
}

function getlastpage() {

    var lastpage = sessionStorage.getItem('lastpage');
    window.location = lastpage;
}
