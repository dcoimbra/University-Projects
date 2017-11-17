/*************************************************************************************************/
/* Esta classe cria duas mensagens: pause e game over.
 * As mensagens são adicionadas à câmara. */
class Messages {

                /* altura, escala, camara */
    constructor(z, width, height, camera) {

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

        pause.position.set(0, 0, z);
        over.position.set(0, 0, z);

        pause.visible = false;
        over.visible = false;

        pause.name = "pause";
        over.name = "over";

        camera.add(pause);
        camera.add(over);
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

        clearTimeout(this.timerId);
        this.remaining -= new Date() - this.start;
    };

    resume() {

        this.start = new Date();
        clearTimeout(this.timerId);
        this.timerId = setTimeout(this.callback, this.remaining, this.argument);
    };

    clear() {

        clearTimeout(this.timerId);
    }
}
/*************************************************************************************************/

/* recebe o nome de uma mensagem e mostra-a na camara ativa */
function showMessage(name) {

    var orthographic = 0;
    var perspective = 1;
    var moving = 2;

    if (views[orthographic]) {

        orthographicCamera.getObjectByName(name).visible = true;
        perspectiveCamera.getObjectByName(name).visible = false;
        car.inner_object.getCamera().getObjectByName(name).visible = false;
    }

    else if (views[perspective]) {

        orthographicCamera.getObjectByName(name).visible = false;
        perspectiveCamera.getObjectByName(name).visible = true;
        car.inner_object.getCamera().getObjectByName(name).visible = false;
    }

    else if (views[moving]) {

        orthographicCamera.getObjectByName(name).visible = false;
        perspectiveCamera.getObjectByName(name).visible = false;
        car.inner_object.getCamera().getObjectByName(name).visible = true;
    }

    render();
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

        orthographicCamera.getObjectByName("pause").visible = false;
        perspectiveCamera.getObjectByName("pause").visible = false;
        car.inner_object.getCamera().getObjectByName("pause").visible = false;
    }
}

/*************************************************************************************************/

function checkOverMessage() {

    'use strict';

    if (gameOver) {

        showMessage("over");
    }

    else {  //if the game is not paused

        orthographicCamera.getObjectByName("over").visible = false;
        perspectiveCamera.getObjectByName("over").visible = false;
        car.inner_object.getCamera().getObjectByName("over").visible = false;
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

    orthographicCamera.getObjectByName("over").visible = false;
    perspectiveCamera.getObjectByName("over").visible = false;
    car.inner_object.getCamera().getObjectByName("over").visible = false;

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
