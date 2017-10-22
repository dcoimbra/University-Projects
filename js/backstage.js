 /*********************************************Variaveis globais*****************************************************/
var scene, renderer;

var orthographicCamera, perspectiveCamera;

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

	//Posicionamento dos vários objetos
	var table = new Table(0, 0, 0);

	createBorderLine();

	/*var orange_height = 5.5;

	var orange1 = new Collidable(new Orange(-40, orange_height, 6));
	var orange2 = new Collidable(new Orange(15, orange_height, -15));
	var orange3 = new Collidable(new Orange(35, orange_height, 5));

	oranges.push(orange1);
	oranges.push(orange2);
	oranges.push(orange3);

	orange2.inner_object.setOrangeSpeed(10);
	orange3.inner_object.setOrangeSpeed(15);*/

	var butterPackage_height = 2.5;

	var butterPackage1 = new Collidable(new ButterPackage(10, butterPackage_height, 5));
	var butterPackage2 = new Collidable(new ButterPackage(45, butterPackage_height, 15));
	var butterPackage3 = new Collidable(new ButterPackage(-10, butterPackage_height, 0));
	var butterPackage4 = new Collidable(new ButterPackage(-25, butterPackage_height, 4));
	var butterPackage5 = new Collidable(new ButterPackage(5, butterPackage_height, -15));

    butter_packages.push(butterPackage1);
    butter_packages.push(butterPackage2);
    butter_packages.push(butterPackage3);
    butter_packages.push(butterPackage4);
    butter_packages.push(butterPackage5);


    car = new Collidable(new Car(0, 2.8, 0));

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

		case 65: //A
		case 97: //a

			scene.traverse(function (node) {

				if (node instanceof THREE.Mesh) {
					node.material.wireframe = !node.material.wireframe;
				}
			});
			break;

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

function checkCollision(posX, posY, posZ) {

	for (var i = 0; i < oranges.length; i++) {

		if (oranges[i].collisionSphere(car)) {

		    car.inner_object.setSpeed(0);
			car.inner_object.car_object.position.set(0, 2.8, 0); //car's initial position
			
			var initial_vector = new THREE.Vector3(car.inner_object.car_object.position.x,
									     car.inner_object.car_object.position.y,
									     car.inner_object.car_object.position.z);

			car.inner_object.car_object.lookAt(initial_vector);
		}
	}

	for (var j = 0; j < butter_packages.length; j++) {

		if (butter_packages[j].collisionSphere(car)) {

            car.inner_object.setSpeed(0);
            car.inner_object.car_object.position.set(posX, posY, posZ);
        }
	}

	var torii = border_lines[0].userData.torii.concat(border_lines[1].userData.torii);

	for (var k = 0; k < torii.length; k++) {

        torii[k].inner_object.torus_object.material.wireframe = !car.collisionSphere(torii[k]);
    }
}

function update(delta) {

    car.inner_object.move(delta);

    for ( var i = 0; i < oranges.length; i++ ) {

        oranges[i].inner_object.move(delta);
    }
}

/********************************************Animation**************************************************************/

function animate() {
	'use strict';

    var delta = clock.getDelta(); //Get the seconds passed since the last time it was called.

	render();

	var carX = car.inner_object.car_object.position.x;
	var carY = car.inner_object.car_object.position.y;
	var carZ = car.inner_object.car_object.position.z;

	update(delta);

    checkCollision(carX, carY, carZ);

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

	//Adicionados eventos, quando resize e keydown
	window.addEventListener('resize', onResize);
	window.addEventListener('keydown', onKeyDown);
	window.addEventListener('keyup', onKeyUp);

    setInterval(updateOrangeSpeed, 5000);
}

/********************************************************************************************************************/