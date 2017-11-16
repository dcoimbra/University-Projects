function checkAnimation() {

    'use strict';

    var orthographic = 0;
    var perspective = 1;
    var moving = 2;

    paused = !paused; //pauses or unpauses game

    if (paused) {

        if (views[orthographic]) {

            orthographicCamera.getObjectByName("sprite").visible = true;
            perspectiveCamera.getObjectByName("sprite").visible = false;
            car.inner_object.getCamera().getObjectByName("sprite").visible = false;
        }

        else if (views[perspective]) {

            orthographicCamera.getObjectByName("sprite").visible = false;
            perspectiveCamera.getObjectByName("sprite").visible = true;
            car.inner_object.getCamera().getObjectByName("sprite").visible = false;
        }

        else if (views[moving]) {

            orthographicCamera.getObjectByName("sprite").visible = false;
            perspectiveCamera.getObjectByName("sprite").visible = false;
            car.inner_object.getCamera().getObjectByName("sprite").visible = true;
        }
    }

    else {  //if the game is not paused

        orthographicCamera.getObjectByName("sprite").visible = false;
        perspectiveCamera.getObjectByName("sprite").visible = false;
        car.inner_object.getCamera().getObjectByName("sprite").visible = false;

        clock.start(); //Starts clock and sets the startTime and oldTime to the current time.
        animate(); //restart animation
    }
}

class Pause {

    constructor(x, y, z, width, height, camera) {

        'use strict';

        var message = new THREE.TextureLoader().load("resources/messages/pause.png");
        var spriteMaterial = new THREE.SpriteMaterial( {map: message, color: 0xffffff } );
        var sprite = new THREE.Sprite( spriteMaterial );

        sprite.name = "sprite";

        camera.add( sprite );
        sprite.scale.x *= width;
        sprite.scale.y *= height;

        sprite.position.set(x, y, z);

        sprite.visible = false;
    }
}

function restart() {

    var to_remove = [];


    /* Remoção dos objectos da cena */
    scene.traverse ( function( node ) {
        if (node instanceof THREE.Object3D) {
            to_remove.push( node );
        }
    } );

    for ( var i = 0; i < to_remove.length; i++ ) {
        scene.remove( to_remove[i] );
    }


    /* Esvaziamento dos arrays.
        Ao fazer length = 0, o garbage collector
        automaticamente apaga o conteúdo do array
     */
    candles.length = 0;
    oranges.length = 0;
    butter_packages.length = 0;
    border_lines[0].length = 0;
    border_lines[1].length = 0;
    border_lines.length = 0;

    /* reiniciar tempo */
    clock.start();
    clearInterval(orangeInterval);

    init();
}
