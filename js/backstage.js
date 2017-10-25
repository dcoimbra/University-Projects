 /*********************************************Variaveis globais*****************************************************/
var scene, renderer;

var orthographicCamera, perspectiveCamera;

var sun;
var candles = [];

var car;
var oranges = [];
var butter_packages = [];
var border_lines = [];

/* tamanho da area visivel */
var frustumSize;

var views;

var clock = new THREE.Clock();


/**********************************************************************************************************************/

/******************************************Criacao da cena************************************************************/
function createScene() {

	'use strict';

	scene = new THREE.Scene();

	createSceneElements();
}
/********************************************************************************************************************/

function createSceneElements() {

    //Posicionamento dos vários objetos
    var table = new Table(0, 0, 0);

    createBorderLine();

    var orange_height = 5.5;

    var orange1 = new Collidable(new Orange(-40, orange_height, 6));
    var orange2 = new Collidable(new Orange(15, orange_height, -15));
    var orange3 = new Collidable(new Orange(35, orange_height, 5));

    oranges.push(orange1);
    oranges.push(orange2);
    oranges.push(orange3);

    orange2.inner_object.setOrangeSpeed(10);
    orange3.inner_object.setOrangeSpeed(15);

    var butterPackage_height = 2.5;

    var butterPackage1 = new Collidable(new ButterPackage(17, butterPackage_height, 10));
    var butterPackage2 = new Collidable(new ButterPackage(47, butterPackage_height, 16));
    var butterPackage3 = new Collidable(new ButterPackage(-39, butterPackage_height, -48.5));
    var butterPackage4 = new Collidable(new ButterPackage(-33, butterPackage_height, 4.7));
    var butterPackage5 = new Collidable(new ButterPackage(36, butterPackage_height, -35));

    butter_packages.push(butterPackage1);
    butter_packages.push(butterPackage2);
    butter_packages.push(butterPackage3);
    butter_packages.push(butterPackage4);
    butter_packages.push(butterPackage5);

    butter_packages[0].inner_object.butterPackage_object.rotateY( 5 * Math.PI / 3 );
    butter_packages[1].inner_object.butterPackage_object.rotateY( 6.2 * Math.PI / 3 );
    butter_packages[2].inner_object.butterPackage_object.rotateY( 5.9 * Math.PI / 3 );
    butter_packages[4].inner_object.butterPackage_object.rotateY( Math.PI / 2 );

    car = new Collidable(new Car(0, 2.8, 0));
    car.inner_object.car_object.translateZ(5);
}

 function createLights() {

     sun = new Sun(0xffffff, 1);

     candle1 = new Candle(5, 2.3, -5);
     candle2 = new Candle(36, 2.3, -15);
     candle3 = new Candle(10, 2.3, 35);
     candle4 = new Candle(-35, 2.3, 44);
     candle5 = new Candle(-17, 2.3, 0);
     candle6 = new Candle(-25, 2.3, -35);

     candles.push(candle1)
     candles.push(candle2);
     candles.push(candle3);
     candles.push(candle4);
     candles.push(candle5);
     candles.push(candle6);
 }


/********************************************Criacao das cameras******************************************************/
function createCameras() {

    'use strict';

    createOrtographicCamera(0, 25, 0);
    createPerspectiveCamera(0, 50, 100);
}

/*--------------------------------------------------------------------------------------------------------------------*/

function createOrtographicCamera(x, y, z) {

	'use strict';

    var aspect = window.innerWidth / window.innerHeight;

    frustumSize = 60;

    /*Camera ortogonal inicializada com frustumSize fixo */

    if (window.innerWidth > window.innerHeight) {

        orthographicCamera = new THREE.OrthographicCamera(- frustumSize * aspect,
                                                            frustumSize * aspect,
                                                            frustumSize,
                                                         -  frustumSize,
                                                            1,
                                                            1000);

    }

    else {

        orthographicCamera = new THREE.OrthographicCamera( - frustumSize,
                                                             frustumSize,
                                                             frustumSize / aspect,
                                                           - frustumSize / aspect,
                                                             1,
                                                             1000);
    }


    scene.add(orthographicCamera);

    /*Camera posicionada em vista de topo */
    orthographicCamera.position.set(x, y, z);
    orthographicCamera.lookAt(scene.position);
}

 /*-----------------------------------------------------------------------------------------------------------------*/

 function createPerspectiveCamera(x, y, z) {

 	'use strict';

     var aspect = window.innerWidth / window.innerHeight;

     /* Camara perspetiva inicializada */

     perspectiveCamera = new THREE.PerspectiveCamera( 60, aspect, 1, 1000 );

     scene.add(perspectiveCamera);

     perspectiveCamera.position.set(x, y, z);
     perspectiveCamera.lookAt(scene.position);
 }
 /*******************************************************************************************************************/

/**********************************************Renderizacao************************************************************/
function render() {

	'use strict';

	var orthographic = 0;
	var perspective = 1;
	var moving = 2;

	if (views[orthographic]) {

	    renderer.render(scene, orthographicCamera);
    }

    else if (views[perspective]) {

        renderer.render(scene, perspectiveCamera);
    }

    else if (views[moving]) {

		renderer.render(scene, car.inner_object.getCamera());
	}
}

/*****************************************************************************************************************/


/***************************************Detalhes de visualizacao e controlo****************************************/

function onResize() {

    'use strict';

    var aspect = window.innerWidth / window.innerHeight;

    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerWidth > window.innerHeight) {

        orthographicCamera.left = -frustumSize * aspect;
        orthographicCamera.right = frustumSize * aspect;
        orthographicCamera.top = frustumSize;
        orthographicCamera.bottom = -frustumSize;
    }

    else {
        orthographicCamera.left = -frustumSize;
        orthographicCamera.right = frustumSize;
        orthographicCamera.top = frustumSize / aspect;
        orthographicCamera.bottom = -frustumSize / aspect;
    }

    perspectiveCamera.aspect = aspect;
    car.inner_object.getCamera().aspect = aspect;

    orthographicCamera.updateProjectionMatrix();
    perspectiveCamera.updateProjectionMatrix();
    car.inner_object.getCamera().updateProjectionMatrix();
}

/*------------------------------------------------------------------------------------------------------------------*/
function onKeyDown(e) {

	'use strict';

	switch (e.keyCode) {

		case 37: //left arrow
			car.inner_object.setMovingDirection(0, true);
			break;
			
		case 39: //Right arrow
			car.inner_object.setMovingDirection(2, true);
			break;

		case 38: //Up arrow
			car.inner_object.setMovingDirection(1, true);
			car.inner_object.setMovingState(true);
			break;

		case 40: //Down arrow
			car.inner_object.setMovingDirection(3, true);
			break;

        case 49: //1
            views = [true, false, false];
            break;

        case 50: //2
            views = [false, true, false];
            break;

		case 51: //3
			views = [false, false, true];
			break;

        case 65: //A

            scene.traverse(function (node) {

                if (node instanceof THREE.Mesh) {
                    node.material.wireframe = !node.material.wireframe;
                }
            });
            break;

        case 67: //C
            flickCandles();
            break;

        case 78: //N
            sun.flick();
            break;

		default:
			break;

	}
}
/*-----------------------------------------------------------------------------------------------------------------*/
function onKeyUp(e) {

	'use strict';

	switch (e.keyCode) {

		case 37: //left arrow
			car.inner_object.setMovingDirection(0, false);
			break;

		case 39: //Right arrow
			car.inner_object.setMovingDirection(2, false);
			break;

		case 38: //Up arrow
			car.inner_object.setMovingDirection(1, false);
			break;

		case 40: //Down arrow
			car.inner_object.setMovingDirection(3, false);
			break;

		default:
			break;

	}
}

function flickCandles() {

    for (var i = 0; i < candles.length; i++) {

        candles[i].flick();
    }
}

/*******************************************************************************************************************/

/* verificar colisões */
function checkCollision(posX, posY, posZ) {

	'use strict';

	/* concatena todos os toros existentes */
    var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

    var torus_position;
    var car_position;
    var otherTorus_position;


    /* colisão entre laranjas e carro */
	for (var i = 0; i < oranges.length; i++) {

		/* se há colisão, colocar o carro na posicao inicial */
		if (oranges[i].collisionSphere(car)) {

		    car.inner_object.setSpeed(0);
			car.inner_object.car_object.position.set(0, 2.8, 5); //car's initial position
			
			var initial_vector = new THREE.Vector3(car.inner_object.car_object.position.x,
									     car.inner_object.car_object.position.y,
									     car.inner_object.car_object.position.z);

			car.inner_object.car_object.lookAt(initial_vector);
		}
	}

	/* colisão entre manteigas e carro */
	for (var j = 0; j < butter_packages.length; j++) {

		if (butter_packages[j].collisionSphere(car)) {

			/* se há colisão, colocar o carro na posição em que
				estava imediatamente antes */

            car.inner_object.setSpeed(0);
            car.inner_object.car_object.position.set(posX, posY, posZ);
        }
	}

	/* colisão entre o carro e os toros */
	for (var k = 0; k < toruses.length; k++) {

		/* se houve uma colisão entre o carro e o toro, determina a direção do vetor entre eles
			e altera a sua velocidade para ser igual à do carro.
		 */

        torus_position = new THREE.Vector3((toruses[k].inner_object.torus_object.position.x + 5),
                                                1.75,
                                                (toruses[k].inner_object.torus_object.position.z - 2));

        car_position = new THREE.Vector3(car.inner_object.car_object.position.x,
                                            1.75,
                                            car.inner_object.car_object.position.z);


        if (car.collisionSphere(toruses[k])) {

			/* se há colisão, a velocidade do toro fica igual à velocidade do carro */
            toruses[k].inner_object.torus_object.userData.speed = Math.abs(car.inner_object.getSpeed());

            /* determina a direção do vetor entre os dois objectos */
            toruses[k].inner_object.torus_object.userData.car_collision_direction = torus_position.sub(car_position); //direção da colisão
            toruses[k].inner_object.torus_object.userData.car_collision_direction.normalize();
		}

		/* procedimento semelhante para as colisóes entre toros */
		for (var l = 0; l < toruses.length; l++) {

        	if ( k !== l && toruses[l].collisionSphere(toruses[k])) {

        		if (toruses[k].inner_object.torus_object.userData.speed !== 0) {

        			//velocidade do toro atingido fica igual à do toro que o atingiu
                    toruses[l].inner_object.torus_object.userData.speed = toruses[k].inner_object.torus_object.userData.speed;

                    otherTorus_position = new THREE.Vector3((toruses[l].inner_object.torus_object.position.x + 5),
                        1.75,
                        (toruses[l].inner_object.torus_object.position.z - 2));

                    toruses[l].inner_object.torus_object.userData.torus_collision_direction = otherTorus_position.sub(torus_position);
                    toruses[l].inner_object.torus_object.userData.torus_collision_direction.normalize();
                }
            }
		}
    }
}


/* atualizar posicao dos objectos */
function update(delta) {

    'use strict';

    car.inner_object.move(delta);

   for ( var i = 0; i < oranges.length; i++ ) {

        oranges[i].inner_object.move(delta);
    }

    var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

    for (var m = 0; m < toruses.length; m++) {

        toruses[m].inner_object.move(delta);
    }
}

/********************************************Animation**************************************************************/

function animate() {
	'use strict';

    var delta = clock.getDelta(); //Get the seconds passed since the last time it was called.

	render();

	/*guardar posicao inicial do carro*/

	var carX = car.inner_object.car_object.position.x;
	var carY = car.inner_object.car_object.position.y;
	var carZ = car.inner_object.car_object.position.z;

	update(delta);

    checkCollision(carX, carY, carZ, delta);

	requestAnimationFrame(animate);
}

/******************************************************************************************************************/

/***************************************************Inicializacao******************************************************/
function init() {

	'use strict';

	renderer = new THREE.WebGLRenderer({ antialias: true });

	renderer.setSize(window.innerWidth, window.innerHeight);

	document.body.appendChild(renderer.domElement);

	views = [true, false, false];

	createScene();
	createCameras();
	createLights();

	//Adicionados eventos, quando resize, keydown e keyup
	window.addEventListener('resize', onResize);
	window.addEventListener('keydown', onKeyDown);
	window.addEventListener('keyup', onKeyUp);

    setInterval(updateOrangeSpeed, 5000);
}

/********************************************************************************************************************/