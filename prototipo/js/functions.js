var totalPrice = 0;
var hash = {};
var all_items = {};
var modifying = 0;

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

function adicionaPedido(coisa, price, estado) {
    var order = findCoisa(coisa+estado, 0);
    var new_quantity = findCoisa(coisa+estado, 1);
    if(!order) { //find: switch case that connects each string to the right global var
        var row1 = document.createElement('tr');

        row1.className = 'order_row';
        row1.id = coisa+estado;

        var td1 = document.createElement('td');
        var td2 = document.createElement('td');
        td2.id = 'order_quantity';
        var td3 = document.createElement('td');

        row1.innerHTML = '<input type="button" id="removeItem" value="-" onclick="removeRow(this, \'' +coisa+ '\', '+price+', \'' +estado+ '\');subtractFromTotal(' + price + ')">';
        td1.innerHTML = 'x '+ new_quantity; //increments the  global var associated to the given string
        td2.innerHTML = ' '+coisa;
        td3.innerHTML = price+' €';
        td3.style = "text-align: right";

        row1.appendChild(td1);
        row1.appendChild(td2);
        row1.appendChild(td3);

        document.getElementById(estado).appendChild(row1);
    }
    else if(order > 0){
        document.getElementById(coisa+estado).cells[0].innerHTML =  'x '+ new_quantity;
    	document.getElementById(coisa+estado).cells[2].innerHTML =  (price*10*new_quantity/10)+' €';
    }

    addToTotal(price);
}

function findCoisa(coisa, int) {
    if(!hash[coisa]) {
        hash[coisa] = 0;
    } 
    return hash[coisa] += int;
}

function removeRow(input, coisa, price, estado) {
    if(findCoisa(coisa+estado, 0) == 1) {
    	document.getElementById(estado).removeChild( input.parentNode );
    	findCoisa(coisa+estado, -1);
    }
    else {
    	var quantidade = findCoisa(coisa+estado, -1);
    	document.getElementById(coisa+estado).cells[0].innerHTML =  'x '+ quantidade;
    	document.getElementById(coisa+estado).cells[2].innerHTML =  (price*10*quantidade/10)+' €';
    }
}

function addToTotal(price) {
    totalPrice += price*10;

    document.getElementById("totalprice").innerHTML = totalPrice/10 + ' €';
}

function subtractFromTotal(price) {

    totalPrice -= price*10;
    document.getElementById("totalprice").innerHTML = totalPrice/10 + ' €';
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

function waiter() {

    $('#waiterbutton').attr('data-target','#waitermodal');
    $('#waiterbutton').attr('data-toggle','modal');
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
