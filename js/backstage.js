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

// Lighting flags
var lighting_on;
var phong_shading;

// Game management
var paused;  //true if the game is paused
var gameOver;
var lives;
var firstTime = true;

// tamanho da area visivel
var frustumSize;

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
    createPerspectiveCamera(0, 50, 100);
}

/*--------------------------------------------------------------------------------------------------------------------*/

function createOrtographicCamera(x, y, z) {

	'use strict';

    var aspect = window.innerWidth / window.innerHeight;
    var livesAspect = (window.innerWidth / 2) / (window.innerHeight/7);

    frustumSize = 60;

    /*Camera ortogonal inicializada com frustumSize fixo */

    if (window.innerWidth > window.innerHeight) {

        orthographicCamera = new THREE.OrthographicCamera(- frustumSize * aspect,
                                                            frustumSize * aspect,
                                                            frustumSize,
                                                         -  frustumSize,
                                                            1,
                                                            1000);

        livesCamera = new THREE.OrthographicCamera(- frustumSize * livesAspect,
                                                     frustumSize * livesAspect,
                                                     frustumSize,
                                                   - frustumSize,
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

        livesCamera = new THREE.OrthographicCamera( - frustumSize,
                                                      frustumSize,
                                                     frustumSize / livesAspect,
                                                   - frustumSize / livesAspect,
                                                         1,
                                                     1000);
    }

    scene.add(orthographicCamera);
    livesScene.add(livesCamera);

    /*Camera posicionada em vista de topo */
    orthographicCamera.position.set(x, y, z);
    orthographicCamera.lookAt(scene.position);

    livesCamera.position.set(x, y, z);
    livesScene.lookAt(livesScene.position);

    new Messages(-10, 100, 50, orthographicCamera);
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
     new Messages(-15, 15, 7.5, perspectiveCamera);
 }
 /*******************************************************************************************************************/

/**********************************************Renderizacao************************************************************/
function render() {

	'use strict';

	var orthographic = 0;
	var perspective = 1;
	var moving = 2;

    renderer.setScissorTest(true);

    renderer.setViewport(0, 0, window.innerWidth, window.innerHeight);
    renderer.setScissor(0, 0, window.innerWidth, window.innerHeight);

    renderer.clear();

    if (views[orthographic]) {

	    renderer.render(scene, orthographicCamera);
    }

    else if (views[perspective]) {

        renderer.render(scene, perspectiveCamera);
    }

    else if (views[moving]) {

		renderer.render(scene, car.inner_object.getCamera());
	}

    renderer.setViewport(0, 0, window.innerWidth / 2, window.innerHeight / 7);
    renderer.setScissor(0, 0, window.innerWidth / 2, window.innerHeight / 7);

    renderer.clear();

    renderer.render(livesScene, livesCamera);

    renderer.setScissorTest(false);
}

/*****************************************************************************************************************/


/***************************************Detalhes de visualizacao***************************************************/

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

/*************************************Verificacao de colisoes********************************************************/

 /* verificar colisões */
 function checkCollision(posX, posY, posZ) {

	 'use strict';

	 /* concatena todos os toros existentes */
	 var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

	 var torus_position;
	 var car_position;
	 var otherTorus_position;

	 /* colisão entre laranjas e carro */
	carOrangesCollision();

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
 function carOrangesCollision(){
	'use strict';

 	for (var i = 0; i < oranges.length; i++) {

		 /* se há colisão, colocar o carro na posicao inicial */
		 if (oranges[i].collisionSphere(car)) {

			 car.inner_object.setSpeed(0);
			 car.inner_object.car_object.position.set(0, 2.8, 5); //car's initial position

			 var initial_vector = new THREE.Vector3(car.inner_object.car_object.position.x,
				                                    car.inner_object.car_object.position.y,
				                                    car.inner_object.car_object.position.z);

			 car.inner_object.car_object.lookAt(initial_vector);

			 lives--;
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

        renderer.autoClear = false;
        renderer.setSize(window.innerWidth, window.innerHeight);

        document.body.appendChild(renderer.domElement);

        lighting_on = true;
        phong_shading = true;

        views = [true, false, false];

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

    paused = false;
	gameOver = false;
    lives = 5;

	createSceneElements();
    createCameras();
    createLights();
}

/********************************************************************************************************************/