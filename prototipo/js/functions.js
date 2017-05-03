var totalPrice = 0;

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

function storeUsername(id) {

    var username = document.getElementById(id);
    sessionStorage.setItem("username", username.value);
}

function adicionaPedido(coisa, price) {

    var row1 = document.createElement('tr');

    row1.className = 'order_row';

    var td1 = document.createElement('td');
    var td2 = document.createElement('td');

    row1.innerHTML = '<input type="button" value="-" onclick="removeRow(this);subtractFromTotal(' + price + ')">';
    td1.innerHTML = ' '+coisa;
    td2.innerHTML = price+' €';
    td2.style = "text-align: right";

    row1.appendChild(td1);
    row1.appendChild(td2);

    document.getElementById('ordertable').appendChild(row1);

    addToTotal(price);
}

function removeRow(input) {
    
    document.getElementById('ordertable').removeChild( input.parentNode );
    
    document.getElementById("totalprice").innerHTML = totalPrice + ' €';
}

function addToTotal(price) {
    totalPrice += price;

    document.getElementById("totalprice").innerHTML = totalPrice + ' €';
}

function subtractFromTotal(price) {

    totalPrice -= price;
    document.getElementById("totalprice").innerHTML = totalPrice + ' €';
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
        sessionStorage.setItem('lastpage', window.location.href);
}

function getlastpage() {

    var lastpage = sessionStorage.getItem('lastpage');
    window.location = lastpage;
}
