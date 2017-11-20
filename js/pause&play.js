/*************************************************************************************************/
/* Esta classe cria duas mensagens: pause e game over. */
// As mensagens fazem overlay por cima da cena principal.
class Messages {

    constructor(width, height) {

        'use strict';

        var pauseMessage = new THREE.TextureLoader().load("resources/messages/pause.png");
        var overMessage = new THREE.TextureLoader().load("resources/messages/over.jpg");

        var pauseMaterial = new THREE.SpriteMaterial( {map: pauseMessage, color: 0xffffff } );
        var overMaterial = new THREE.SpriteMaterial( {map: overMessage, color: 0xffffff } );

        var pause = new THREE.Sprite( pauseMaterial );
        var over = new THREE.Sprite( overMaterial );

        pause.scale.x *= width;
        pause.scale.y *= height;

        over.scale.x *= width;
        over.scale.y *= height;

        pause.position.set(0, 0, 0);
        over.position.set(0, 0, 0);

        pause.visible = false;
        over.visible = false;

        pause.name = "pause";
        over.name = "over";

        overlaidScene.add(pause);
        overlaidScene.add(over);
    }
}

/*************************************************************************************************/

class Timer {

    constructor(callback, delay, argument) {

        this.callback = callback;
        this.remaining = delay;
        this.argument = argument;

        this.resume();
    }

    pause() {

        this.clear();
        this.remaining -= new Date() - this.start;
    };

    resume() {

        this.start = new Date();
        this.clear();
        this.timerId = setTimeout(this.callback, this.remaining, this.argument);
    };

    clear() {

        clearTimeout(this.timerId);
    }
}
/*************************************************************************************************/

function killCheck(posX, posY, posZ) {

    car.inner_object.setSpeed(0);

    if (lives > 0) { //se o carro ainda tiver vidas, voltar à origem

        car.inner_object.car_object.position.set(0, 2.8, 5); //car's initial position

        var initial_vector = new THREE.Vector3(car.inner_object.car_object.position.x,
                                               car.inner_object.car_object.position.y,
                                               car.inner_object.car_object.position.z);

        car.inner_object.car_object.lookAt(initial_vector);
    }

    else { //se o carro não tiver mais vidas, colocá-lo na posição anterior

        car.inner_object.car_object.position.set(posX, posY, posZ);
    }
}

/*************************************************************************************************/
function removeLife() {

    if (lives_objects.length > 0) {

        var life = lives_objects.pop();

        overlaidScene.remove(life.car_object);
    }
}

/* recebe o nome de uma mensagem e mostra-a  */
function showMessage(name) {

    overlaidScene.getObjectByName(name).visible = true;
}


/* recebe o nome de uma mensagem e esconde-a  */
function hideMessage(name) {

    overlaidScene.getObjectByName(name).visible = false;
}

/*************************************************************************************************/

function togglePause() {

    if (!gameOver) {

        paused = !paused;

        if (globalClock.running) {               //if the clock is running, stop time

            globalClock.stop();
            intervalClock.stop();

            if (orangeTimeout !== undefined) {

                orangeTimeout.pause();
            }
        }

        else {                                   //if time is stopped, resume it

            globalClock.start();
            intervalClock.start();

            if (orangeTimeout !== undefined) {

                orangeTimeout.resume();
            }
        }
    }
}

/*************************************************************************************************/

function checkPauseMessage() {

    'use strict';

    if (paused) {

        showMessage("pause");
    }

    else {  //if the game is not paused

        hideMessage("pause");
    }
}

/*************************************************************************************************/

function checkOverMessage() {

    'use strict';

    if (gameOver) {

        showMessage("over");
    }

    else {  //if the game is not paused

        hideMessage("over");
    }
}

/*************************************************************************************************/


function endGame() {

    globalClock.stop();
    intervalClock.stop();
    orangeTimeout.clear();
    showMessage("over");
}

/*************************************************************************************************/

// retira todos os objectos da cena e reinicia o jogo
function restart() {

    var to_remove = [];

    hideMessage("over");

    /* Remoção dos objectos da cena */
    scene.traverse ( function (node) {
        if (node instanceof THREE.Object3D) {

            to_remove.push( node );
        }
    } );

    for ( var i = 0; i < to_remove.length; i++ ) {

        scene.remove( to_remove[i] );
    }

    /* reiniciar tempo */
    globalClock.start();
    intervalClock.start();

    init();
}
/*************************************************************************************************/
