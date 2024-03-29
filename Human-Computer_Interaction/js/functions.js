var lastdiv;
var count = 0;
var hash = {};
var iterator = 0;
var totalPrice = 0;
var inPreview = [];
var countQueue = 0;
var loggedin = 'false';
var arealoggedin = 'false';
var divname = '#ecrainicial';
var divloginname = '#loginpage';
var oldusername, oldnif;
var firstTime = 1;
var nifCount = 0;
var paymentselected = 0;

function makeBarInvisible() {
    var i;
    var popup = document.getElementsByClassName("popup");
    for (i = 0; i < popup.length; i++)
        popup[i].style.visibility='hidden';
}

function showBar(name, idname) {
    var i;
    var popup = document.getElementsByClassName(name);
    for (i = 0; i < popup.length; i++)
        popup[i].style.visibility='visible';
    move(idname);
}

function move(name) {
    var elem = document.getElementById(name);
    var width = 0;
    var id = setInterval(frame, 10);
    function frame() {
        if (width == 100) {
            clearInterval(id);
            $(divloginname).hide();
            login();
            $('#pessoal').show();
            $('.backbutton').show();
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

function storeNIF(value) {
    var nif = document.getElementById(value);
    sessionStorage.setItem("nif", nif.value);
}

function checkchange(id, nifvalue) {
    var change = 0;
    var username = document.getElementById(id).value;
    var nif = document.getElementById(nifvalue).value;
    
    if ((username != "") && (username != oldusername)) {
        sessionStorage.setItem("username", username);
        oldusername = username;
        change = 1;
    }
        
    if ( (nif != "") && (nif != oldnif)) {
        sessionStorage.setItem("nif", nif);
        oldnif = nif;
        change = 1;
    }
    
    if(change) { $('#dadosmodal').modal(); }
}

/* ----- LISTA DE PEDIDOS ----- */
function adicionaPedido(coisa, price, estado, prepTime) {
    var order = findCoisa(coisa+estado, 0);
    var new_quantity = findCoisa(coisa+estado, 1);
    console.log('tempo: '+prepTime);
    if(estado == 'orderPreview') {
        inPreview.push({nome: coisa, preco: price, tempo: prepTime, contador:iterator++});
        
        count++;
        countQueue++;
        switchbottomrightbutton();
    }
    
    if(!order) {
        var row1 = document.createElement('tr');

        row1.className = 'order_row';
        row1.id = coisa+estado;
        
        if(estado == 'orderPreview') {
            var td0 = document.createElement('td');
        }

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
        
        resetcount();
    }
    else if (estado == 'orderCooking') {
        console.log('tempo cozinha: '+prepTime+' segundos');
        setTimeout(changeState, prepTime*1000, coisa, price, estado, 'orderReady', prepTime);
        
        resetcount();
    }
    
    else if (estado == 'orderReady') {
        countQueue--;
        if (countQueue == 0) {
            $('#finishmeal').show();
            $('#readymodal').modal();
        }
    }
    
    checkPreviewList();
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
    
    count--;
    countQueue--;
    switchbottomrightbutton();

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

function setNIF() {
    var nif = sessionStorage.getItem("nif");
    
    if (nif != "undefined") {
        document.getElementById('nifinput').value = nif;
        nifCount = nif.length;
    }
}

function checkPreviewList() {
    
    if (!inPreview.length) {
        
        console.log("tem cenas: " + inPreview.length);
    }
    else
        console.log("tem cenas: " + inPreview.length);
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

function showhelp() {
    
    $('#helpbutton').attr('data-target','#helpmodal');
    $('#helpbutton').attr('data-toggle','modal');
}

function ordercheck() {
    $('#sendorder').attr('data-target','#ordermodal');
    $('#sendorder').attr('data-toggle','modal');
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
    saveldivname('#loginpage');
}

function savedivname(name) {
    divname = name;
}

function saveldivname(name) {
    divloginname = name;
}

function savelastdiv(name) {
    lastdiv = name;
}

function choosesavediv(name) {
    if ($.inArray(name, ['#pessoal', '#promocoes', '#sugestoes', '#dadospessoais']) > -1) {
        saveldivname(name);
    } else {
        savedivname(name);
    }
}

function choosegetdiv() {
    if (arealoggedin == 'true') {
        exit();
        return divloginname;
    } else {
        return divname;
    }
}

function switchbottomrightbutton() {
    if (count > 0) {
        $('#finishmeal').hide();
        $('#sendorder').show();   
    } else {
        $('#sendorder').hide();
        $('#finishmeal').show(); 
    }
}

$('#finishmeal').click(function() {
    $(choosegetdiv()).hide();
    $('#finishmeal').hide();
    $('#terminar').show();
    $('.backbutton').show();
    savelastdiv(choosegetdiv());
    savedivname('#terminar');
    setNIF();
});

function resetcount() {
    count = 0;
    $('#sendorder').hide();
}

$('#waitbutton').click(function() {
    $(divname).hide();
    $('#apospedido').show();
    savedivname('#apospedido');
    $('#waitbutton').hide();
});

function changeglow() {
    
    document.getElementById("waiterbutton").style.boxShadow = "0px 0px 50px forestgreen";
    
    setTimeout(function() { document.getElementById("waiterbutton").style.boxShadow = "0px 0px 50px transparent"; }, 10000);
}

function showKeyboard() {
     $("#keyboardImage").show();
}

function hideKeyboard() {
     $("#keyboardImage").hide();
}


function changePayMethod(multibanco) {

    if(multibanco) {
        paymentselected = 1;
        document.getElementById("pMultibanco").style.boxShadow = "0px 0px 50px forestgreen";
        document.getElementById("pDinheiro").style.boxShadow = "0px 0px 50px transparent";
    }
    else {
        paymentselected = 1;
        document.getElementById("pDinheiro").style.boxShadow = "0px 0px 50px forestgreen";
        document.getElementById("pMultibanco").style.boxShadow = "0px 0px 50px transparent";       
    }
    
}

function writeNumber(num) {
    var nif = (document.getElementById("nifinput")).value;
    if(firstTime) {
        ((document.getElementById("nifinput")).value) = num;
        firstTime = 0;
        nifCount++;
    }
    else if(nifCount < 9) {
        ((document.getElementById("nifinput")).value) += num;
        nifCount++;
    }
    
    console.log(nifCount);
}

function eraseNumber() {
    if(nifCount > 0 ) {
       var nif =  (document.getElementById("nifinput")).value;
        if (nif > 10) 
            nif = Math.floor(nif/10);
        else
            nif = "";
        
        (document.getElementById("nifinput")).value = nif;
        nifCount--;
        console.log(nifCount);
    }
}

function checkpayment() {
    if (paymentselected == 0)
        $('#paymentmodal').modal();
    else {
        $('#terminar').hide();
        $('#fim').show();
        $('.backbutton').hide();
        $('#loginbutton').hide();
        $('#order').hide();
        $('#waiterbutton').hide();
        $('#helpbutton').hide()
    }
}
