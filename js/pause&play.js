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

    clock.start();
    clearInterval(orangeInterval);

    scene.traverse ( function( node ) {
        if (node instanceof THREE.Mesh || node instanceof THREE.PointLightHelper || node instanceof THREE.Line) {
            to_remove.push( node );
        }
    } );

    scene.remove(car.inner_object.car_object);
    scene.remove(table.table_object);

    for ( var i = 0; i < to_remove.length; i++ ) {
        scene.remove( to_remove[i] );
    }

    init();
}
