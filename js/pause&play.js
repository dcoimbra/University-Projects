
/* Esta classe cria duas mensagens: pause e game over.
 * As mensagens são adicionadas à câmara. */
class Messages {

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

        pause.visible = false
        over.visible = false;

        pause.name = "pause";
        over.name = "over";

        camera.add(pause);
        camera.add(over);
    }
}

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

function checkPause() {

    'use strict';

    paused = !paused; //pauses or unpauses game

    if (paused) {

        showMessage("pause");
    }

    else {  //if the game is not paused

        orthographicCamera.getObjectByName("pause").visible = false;
        perspectiveCamera.getObjectByName("pause").visible = false;
        car.inner_object.getCamera().getObjectByName("pause").visible = false;

        clock.start(); //Starts clock and sets the startTime and oldTime to the current time.
        animate(); //restart animation
    }
}

function endGame() {

    clock.stop();
    showMessage("over");
}

function restart() {

    var to_remove = [];

    orthographicCamera.getObjectByName("over").visible = false;
    perspectiveCamera.getObjectByName("over").visible = false;
    car.inner_object.getCamera().getObjectByName("over").visible = false;

    /* Remoção dos objectos da cena */
    scene.traverse ( function( node ) {
        if (node instanceof THREE.Object3D) {
            to_remove.push( node );
        }
    } );

    for ( var i = 0; i < to_remove.length; i++ ) {
        scene.remove( to_remove[i] );
    }

    /* reiniciar tempo */
    clock.start();
    clearInterval(orangeInterval);

    init();
    animate();
}
