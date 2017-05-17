var hash = {};
var inPreview = [];
var iterator = 0;
var totalPrice = 0;
var loggedin = 'false';
var divname = '#menupage';
var divloginname = '#loginpage';
var areapessoal = ['#pessoal', '#promocoes', '#sugestoes', '#dados'];



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
            $(divloginname).hide();
            $('#pessoal').show();
            $('.backbutton').show();
            login();
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
    if(estado == 'orderPreview') {
    inPreview.push({nome: coisa, preco: price, contador:iterator++});
    }

    if(!order) {
        var row1 = document.createElement('tr');

        row1.className = 'order_row';
        row1.id = coisa+estado;

        var td1 = document.createElement('td');
        var td2 = document.createElement('td');
        td2.id = 'order_quantity';
        var td3 = document.createElement('td');

        if(estado == 'orderPreview'){
            row1.innerHTML = '<input type="button" value="-" onclick="removeRow(\'' +coisa+ '\', '+price+', \'' +estado+ '\');">';
        }
        else {
            row1.innerHTML = '<input type="button" value="-" id="removeItem" onclick="removeRow(\'' +coisa+ '\', '+price+', \'' +estado+ '\');">';
        }
        td1.innerHTML = 'x '+ new_quantity; 
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
    
    if (estado == 'orderQueue') {
        setTimeout(changeState, rndInt()*1000, coisa, price, estado, 'orderCooking');
    }
/*    else if (estado == 'orderCooking') {
        setInterval(changeState(coisa, price, estado, 'orderReady'), prepTime);
    }*/
}


function changeState(nome, preco, prev_estado, next_estado) {
    removeRow(nome, preco, prev_estado);
    adicionaPedido(nome, preco, next_estado);
}

function leavePreviewState() {
    var b = inPreview.length;
    inPreview.forEach( 
        function(element) {
            changeState(element.nome, element.preco, 'orderPreview', 'orderQueue');
        });
    inPreview = [];
}

function findCoisa(coisa, int) {
    if(!hash[coisa]) {
        hash[coisa] = 0;
    }
    return hash[coisa] += int;
}

function removeRow(coisa, price, estado) {
    if(findCoisa(coisa+estado, 0) == 1) {
    	document.getElementById(estado).removeChild( document.getElementById(coisa+estado) );
    	findCoisa(coisa+estado, -1);
    }
    else {
    	var quantidade = findCoisa(coisa+estado, -1);
    	document.getElementById(coisa+estado).cells[0].innerHTML =  'x '+ quantidade;
    	document.getElementById(coisa+estado).cells[2].innerHTML =  (price*10*quantidade/10)+' €';
    }
    
    subtractFromTotal(price);
}

function rndInt() {
    return Math.floor(Math.random()*(6-1))+1;
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

    if (loggedin === "true") {
        document.getElementById("loginbutton").src = "../images/area_pessoal_21.png";
        $(divname).hide();
        $('#pessoal').show();
    } else if (loggedin == 'false') {
        document.getElementById("loginbutton").src = "../images/login_21.png";
        $(divname).hide();
        $('#loginpage').show();
        $('.backbutton').show();
        makeBarInvisible();
    } else if ($.inArray(divname, areapessoal)) {
        document.getElementById("loginbutton").src = "../images/logout_21.png";
        preparelogout();
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

    loggedin = 'true';
    saveldivname('#pessoal');
}

function logout() {

    loggedin = 'false';
    saveldivname('#login');
    alert('logout');
}

function savedivname(name) {
    divname = name;
}

function saveldivname(name) {
    divloginname = name;
}


function changeglow() {
    
    document.getElementById("waiterbutton").style.boxShadow = "0px 0px 50px forestgreen";
    
    setTimeout(function() { document.getElementById("waiterbutton").style.boxShadow = "0px 0px 50px transparent"; }, 10000);
}