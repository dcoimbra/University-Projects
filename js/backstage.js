 /*********************************************Variaveis globais*****************************************************/
var scene, livesScene, renderer;

//Câmeras
var orthographicCamera, perspectiveCamera;
var livesCamera;

//Light
var sun;
var candles;

//Objetos

var car;
var oranges;
var butter_packages;
var border_lines;

var lives_objects;

// Lighting flags
var lighting_on;
var phong_shading;

// Game management
var paused;  //true if the game is paused
var gameOver;
var lives;
var firstTime = true;

// perspetivas
var views;

// Time
var globalClock;
var intervalClock;

var orangeTimeout;


/**********************************************************************************************************************/

/******************************************Criacao da cena************************************************************/

function createSceneElements() {
	'use strict';

    var orange_height = 5.5;
    var butterPackage_height = 2.5;

    //Posicionamento dos vários objetos
    new Table(0, 0, 0);

    createBorderLine();

    oranges.push(new Collidable(new Orange(-40, orange_height, 6)));
    oranges.push(new Collidable(new Orange(15, orange_height, -15)));
    oranges.push(new Collidable(new Orange(35, orange_height, 5)));

    oranges[1].inner_object.setOrangeSpeed(10);
    oranges[2].inner_object.setOrangeSpeed(15);

    butter_packages.push(new Collidable(new ButterPackage(17, butterPackage_height, 10)));
    butter_packages.push(new Collidable(new ButterPackage(47, butterPackage_height, 16)));
    butter_packages.push(new Collidable(new ButterPackage(-39, butterPackage_height, -48.5)));
    butter_packages.push(new Collidable(new ButterPackage(-33, butterPackage_height, 4.7)));
    butter_packages.push(new Collidable(new ButterPackage(36, butterPackage_height, -35)));

    butter_packages[0].inner_object.butterPackage_object.rotateY( 5 * Math.PI / 3 );
    butter_packages[1].inner_object.butterPackage_object.rotateY( 6.2 * Math.PI / 3 );
    butter_packages[2].inner_object.butterPackage_object.rotateY( 5.9 * Math.PI / 3 );
    butter_packages[4].inner_object.butterPackage_object.rotateY( Math.PI / 2 );

    car = new Collidable(new Car(0, 2.8, 0));
    scene.add(car.inner_object.car_object);

    car.inner_object.car_object.translateZ(5);
}
/*------------------------------------------------------------------------------------------------------------------*/

function createLives() {

    for (var i = 0; i < 5; i++) {

        var life = new Car(-40 + (i * 20), 93, 0);

        life.car_object.scale.multiplyScalar(3.5);

        livesScene.add(life.car_object);

        lives_objects.push(life);
    }

    new Messages(185, 85);

    livesScene.traverse(toggleLighting);
}

/*------------------------------------------------------------------------------------------------------------------*/

function createLights() {

    'use strict';

     sun = new Sun(0xffffff, 1);

     candles.push(new Candle(5, 8, -5));
     candles.push(new Candle(36, 8, -15));
     candles.push(new Candle(10, 8, 35));
     candles.push(new Candle(-35, 8, 44));
     candles.push(new Candle(-35, 8, -10));
     candles.push(new Candle(-25, 8, -35));
 }
/********************************************************************************************************************/
 
/********************************************Criacao das cameras******************************************************/
function createCameras() {

    'use strict';

    createOrtographicCamera(0, 25, 0);
    createPerspectiveCamera(0, 50, 105);
}

/*--------------------------------------------------------------------------------------------------------------------*/

function createOrtographicCamera(x, y, z) {

	'use strict';

    /*Camera ortogonal inicializada com frustumSize fixo */

    orthographicCamera = buildOrtho(60);
    livesCamera = buildOrtho(100);

    scene.add(orthographicCamera);
    livesScene.add(livesCamera);

    /*Camera posicionada em vista de topo */
    orthographicCamera.position.set(x, y, z);
    orthographicCamera.lookAt(scene.position);

    livesCamera.position.set(0, 0, 50);
    livesScene.lookAt(livesScene.position);
}

function buildOrtho(frustumSize) {

    var aspect = window.innerWidth / window.innerHeight;

    if (window.innerWidth > window.innerHeight) {

        camera = new THREE.OrthographicCamera( - frustumSize * aspect,
                                                 frustumSize * aspect,
                                                 frustumSize,
                                               - frustumSize,
                                                 1,
                                                 1000);
    }

    else {

        camera = new THREE.OrthographicCamera( - frustumSize,
                                                 frustumSize,
                                                 frustumSize / aspect,
                                               - frustumSize / aspect,
                                                 1,
                                                 1000);
    }

    return camera;
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

    renderer.clear(); // Tells the renderer to clear its color, depth ans stencil drawing buffers.
                      // Initializes the color buffer to the current clear color value.

    if (views[orthographic]) {

	    renderer.render(scene, orthographicCamera);
    }

    else if (views[perspective]) {

        renderer.render(scene, perspectiveCamera);
    }

    else if (views[moving]) {

		renderer.render(scene, car.inner_object.getCamera());
	}

    renderer.clearDepth(); //Clear the depth buffer

    renderer.render(livesScene, livesCamera);
}

/*****************************************************************************************************************/


/***************************************Detalhes de visualizacao***************************************************/

function onResize() {

    'use strict';

    var aspect = window.innerWidth / window.innerHeight;

    renderer.setSize(window.innerWidth, window.innerHeight);

    resizeOrtho(orthographicCamera, 60, aspect);
    resizeOrtho(livesCamera, 100, aspect);

    perspectiveCamera.aspect = aspect;
    car.inner_object.getCamera().aspect = aspect;

    perspectiveCamera.updateProjectionMatrix();
    car.inner_object.getCamera().updateProjectionMatrix();
}

/*------------------------------------------------------------------------------------------------------------------*/

function resizeOrtho(camera, frustumSize, aspect) {

    if (window.innerWidth > window.innerHeight) {

        camera.left = -frustumSize * aspect;
        camera.right = frustumSize * aspect;
        camera.top = frustumSize;
        camera.bottom = -frustumSize;
    }

    else {

        camera.left = -frustumSize;
        camera.right = frustumSize;
        camera.top = frustumSize / aspect;
        camera.bottom = -frustumSize / aspect;
    }

    camera.updateProjectionMatrix();
}

/*************************************Verificacao de colisoes********************************************************/

 /* verificar colisões */
 function checkCollision(posX, posY, posZ) {

	 'use strict';

	 /* concatena todos os toros existentes */
	 var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

	 var torus_position;
	 var car_position;
	 var otherTorus_position;

	 /* colisão entre carro e mesa */
	 carTableCollision(posX, posY, posZ);

	 /* colisão entre laranjas e carro */
	carOrangesCollision(posX, posY, posZ);

	 /* colisão entre manteigas e carro */
	carButterPackagesCollision(posX, posY, posZ);

	 /* colisão entre o carro e os toros */
	 for (var k = 0; k < toruses.length; k++) {

		 /* se houve uma colisão entre o carro e o toro, determina a direção do vetor entre eles
			 e altera a sua velocidade para ser igual à do carro.*/

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
 /*------------------------------------------------------------------------------------------------------------------*/
function carTableCollision(posX, posY, posZ) {

    var car_Xposition = car.inner_object.car_object.position.x;
    var car_Zposition = car.inner_object.car_object.position.z;

    //Se a laranja ultrapassar os limites da table
    if (car_Xposition > 55 || car_Xposition < -55 ||
        car_Zposition > 55 || car_Zposition < -55) {

        lives--;
        removeLife();

        killCheck(posX, posY, posZ); //verificar se o carro ficou sem vidas
    }
}

 /*------------------------------------------------------------------------------------------------------------------*/
 function carOrangesCollision(posX, posY, posZ){
	'use strict';

 	for (var i = 0; i < oranges.length; i++) {

		 /* se há colisão, colocar o carro na posicao inicial */
		 if (oranges[i].collisionSphere(car)) {

             lives--;
			 removeLife();

			 killCheck(posX, posY, posZ); //verificar se o carro ficou sem vidas
		 }
	 }
 }
/*---------------------------------------------------------------------------------------------------------*/
 function carButterPackagesCollision(posX, posY, posZ){
	'use strict';

 	for (var j = 0; j < butter_packages.length; j++) {

		 if (butter_packages[j].collisionSphere(car)) {

			 /* se há colisão, colocar o carro na posição em que
				 estava imediatamente antes */

			 car.inner_object.setSpeed(0);
			 car.inner_object.car_object.position.set(posX, posY, posZ);
		 }
	 }
 }

 /********************************************************************************************************************/

 /*************************************Detalhes de controlo***********************************************************/
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

            livesScene.traverse(function (node) {

                if (node instanceof THREE.Mesh) {
                    node.material.wireframe = !node.material.wireframe;
                }
            });
            break;

        case 67: //C
            flickCandles();
            break;

        case 71: //G
            phong_shading = !phong_shading;
            scene.traverse(toggleShading);
            break;

        case 72: //H
            car.inner_object.flickHeadlights();
            break;

        case 76: //L
            lighting_on = !lighting_on;
            scene.traverse(toggleLighting);
            break;

        case 78: //N
            sun.flick();
            break;

        case 82: //R

            if (gameOver) {

                restart();
            }
            break;

        case 83: //S

            togglePause();
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

/*******************************************************************************************************************/

/**********************************************Updates*************************************************************/

/* atualizar posicao dos objectos */
function update(delta) {

    'use strict';

    car.inner_object.move(delta);

    moveOranges(delta);
    moveToruses(delta);

}
/*------------------------------------------------------------------------------------------------------------------*/
function moveOranges(delta){
	'use strict';

	for ( var i = 0; i < oranges.length; i++ ) {

		oranges[i].inner_object.move(delta);
	}
}
/*------------------------------------------------------------------------------------------------------------------*/
function moveToruses(delta){
	'use strict';

	var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

	for (var m = 0; m < toruses.length; m++) {

		toruses[m].inner_object.move(delta);
	}
}
/*******************************************************************************************************************/
/********************************************Animation**************************************************************/

function animate() {
	'use strict';

	var stopped = paused || gameOver;

	/* if the lives ever reach 0, the game is over */
	if (lives === 0) {

	    gameOver = true;
	    endGame();
    }

	checkPauseMessage();
	checkOverMessage(); // precisamos disto para mudar de cameras durante pause ou game over
	checkOrangeInterval();  // verificar se já é altura de aumentar a velocidade das laranjas

    var delta = globalClock.getDelta(); // Get the seconds passed since the time oldTime was set and sets oldTime to the current time.

	/*guardar posicao inicial do carro*/
	var carX = car.inner_object.car_object.position.x;
	var carY = car.inner_object.car_object.position.y;
	var carZ = car.inner_object.car_object.position.z;

	if (!stopped) { // para parar as rotações

        update(delta);
    }

    checkCollision(carX, carY, carZ, delta);

	requestAnimationFrame(animate);

    render();
}

/******************************************************************************************************************/

/***************************************************Inicializacao******************************************************/
function init() {

	'use strict';

	/* da primeira vez, são criados o renderer e as cenas.
	    a partir daí, só é necessário criar os objectos.
	 */

	if (firstTime) {

        renderer = new THREE.WebGLRenderer({antialias: true});
        globalClock = new THREE.Clock();
        intervalClock = new THREE.Clock();

        renderer.autoClear = false; //Disable automatically clearing the renderer output before rendering a frame
        renderer.setSize(window.innerWidth, window.innerHeight);

        document.body.appendChild(renderer.domElement);

        //Adicionados eventos, quando resize, keydown e keyup
        window.addEventListener('resize', onResize);
        window.addEventListener('keydown', onKeyDown);
        window.addEventListener('keyup', onKeyUp);

        scene = new THREE.Scene();
        livesScene = new THREE.Scene();

        firstTime = false;
    }

    candles = [];
    oranges = [];
    butter_packages = [];
    border_lines = [];
    lives_objects = [];

    lighting_on = true;
    phong_shading = true;

    views = [true, false, false];

    paused = false;
	gameOver = false;
    lives = 5;

	createSceneElements();
	createLives();
    createCameras();
    createLights();
}

/********************************************************************************************************************/