var hash = {};
var inPreview = [];
var iterator = 0;
var totalPrice = 0;
var loggedin = 'false';
var arealoggedin = 'false';
var divname = '#menupage';
var divloginname = '#loginpage';


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

/* ----- LISTA DE PEDIDOS ----- */
function adicionaPedido(coisa, price, estado, prepTime) {
    var order = findCoisa(coisa+estado, 0);
    var new_quantity = findCoisa(coisa+estado, 1);
    console.log('tempo: '+prepTime);
    if(estado == 'orderPreview') {
    inPreview.push({nome: coisa, preco: price, tempo: prepTime, contador:iterator++});
    }

    if(!order) {
        var row1 = document.createElement('tr');

        row1.className = 'order_row';
        row1.id = coisa+estado;
        
        if(estado == 'orderPreview') {var td0 = document.createElement('td');}

        var td1 = document.createElement('td');
        var td2 = document.createElement('td');
        td2.id = 'order_quantity';
        var td3 = document.createElement('td');

        if(estado == 'orderPreview'){
            row1.innerHTML = '<input type="button" value="-" onclick="removeOnClick(\'' +coisa+ '\', '+price+', \'' +estado+ '\');">';
            td0.innerHTML = '<input type="button" value="+" onclick="adicionaPedido(\'' +coisa+ '\', '+price+', \'' +estado+ '\', \'' +prepTime+ '\');">';
            row1.appendChild(td0);
        }
        else {
            row1.innerHTML = '<input type="button" value="-" id="removeItem" onclick="">';
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
        var i = (estado == 'orderPreview' ? 1 : 0);
        document.getElementById(coisa+estado).cells[0+i].innerHTML =  'x '+ new_quantity;
    	document.getElementById(coisa+estado).cells[2+i].innerHTML =  (price*10*new_quantity/10)+' €';
    }
    addToTotal(price);
    
    if (estado == 'orderQueue') {
        setTimeout(changeState, rndInt()*1000, coisa, price, estado, 'orderCooking', prepTime);
    }
    else if (estado == 'orderCooking') {
        console.log('tempo cozinha: '+prepTime+' segundos');
        setTimeout(changeState, prepTime*1000, coisa, price, estado, 'orderReady', prepTime);
    }

}


function changeState(nome, preco, prev_estado, next_estado, prepTime) {
    removeRow(nome, preco, prev_estado);
    adicionaPedido(nome, preco, next_estado, prepTime);
}

function leavePreviewState() {
    var b = inPreview.length;
    inPreview.forEach( 
        function(element) {
            changeState(element.nome, element.preco, 'orderPreview', 'orderQueue', element.tempo);
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
        var i = (estado == 'orderPreview' ? 1 : 0);
    	var quantidade = findCoisa(coisa+estado, -1);
    	document.getElementById(coisa+estado).cells[0+i].innerHTML =  'x '+ quantidade;
    	document.getElementById(coisa+estado).cells[2+i].innerHTML =  (price*10*quantidade/10)+' €';
    }
    subtractFromTotal(price);
}

function removeOnClick(coisa, price, estado) {
    var result = $.grep(inPreview, function(e){ return e.nome == coisa; });
    var i = inPreview.indexOf(result[(result.length)-1]);
    inPreview.splice(i, 1);
    
    removeRow(coisa, price, estado);

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
/* ----- FIM LISTA DE PEDIDOS ----- */

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

$('#loginbutton').on('click', function() {
    if (loggedin == 'true' && arealoggedin == 'false') {
        $(divname).hide();
        $('#pessoal').show();
        $('.backbutton').show();
        enter();
    } else if (loggedin == 'true' && arealoggedin == 'true') {
        $(this).attr('src', '../images/logout_21.png');
        preparelogout();
    } else {
        makeBarInvisible();
        $(this).hide();
        $(divname).hide();
        $('#loginpage').show();
        $('.backbutton').show();
        $(this).attr('src', '../images/login_21.png');
    }
});

function preparelogout() {

    $('#loginbutton').attr('data-target','#myModal');
    $('#loginbutton').attr('data-toggle','modal');
}

function waiter() {

    $('#waiterbutton').attr('data-target','#waitermodal');
    $('#waiterbutton').attr('data-toggle','modal');
}

function enter() {
    arealoggedin = 'true';
    $('#loginbutton').attr('src', '../images/logout_21.png');
    $('#loginbutton').show();
}

function exit() {
    arealoggedin = 'false';
    $('#loginbutton').removeAttr('data-toggle','modal');
    $('#loginbutton').attr('src', '../images/area_pessoal_21.png');
}

function login() {

    loggedin = 'true';
	enter();
    saveldivname('#pessoal');
}

function logout() {

    loggedin = 'false';
    $(divloginname).hide();
    exit();
    $('#loginbutton').attr('src', '../images/login_21.png');
    $(divname).show();
    saveldivname('#login');
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