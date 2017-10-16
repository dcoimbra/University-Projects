 /*********************************************Variaveis globais*****************************************************/
var scene, renderer;

var orthographicCamera, perspectiveCamera;

var car;
var oranges = [];

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

	var orange_height = 5.5;

	var orange1 = new Orange(-40, orange_height, 6);
	var orange2 = new Orange(15, orange_height, -15);
	var orange3 = new Orange(35, orange_height, 5);

	oranges.push(orange1);
	oranges.push(orange2);
	oranges.push(orange3);

	orange2.setOrangeSpeed(15);
	orange3.setOrangeSpeed(20);

	var butterPackage_height = 2.5;

	var butterPackage1 = new ButterPackage(10, butterPackage_height, 5);
	var butterPackage2 = new ButterPackage(45, butterPackage_height, 15);
	var butterPackage3 = new ButterPackage(-10, butterPackage_height, 0);
	var butterPackage4 = new ButterPackage(-25, butterPackage_height, 4);
	var butterPackage5 = new ButterPackage(5, butterPackage_height, -15);

	car = new Car(0, 2.8, 0);

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

		renderer.render(scene, car.getCamera());
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
    car.getCamera().aspect = aspect;

    orthographicCamera.updateProjectionMatrix();
    perspectiveCamera.updateProjectionMatrix();
    car.getCamera().updateProjectionMatrix();
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
			car.setMovingDirection(0, true);
			break;
			
		case 39: //Right arrow
			car.setMovingDirection(2, true);
			break;

		case 38: //Up arrow
			car.setMovingDirection(1, true);
			car.setMovingState(true);
			break;

		case 40: //Down arrow
			car.setMovingDirection(3, true);
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
			car.setMovingDirection(0, false);
			break;

		case 39: //Right arrow
			car.setMovingDirection(2, false);
			break;

		case 38: //Up arrow
			car.setMovingDirection(1, false);
			break;

		case 40: //Down arrow
			car.setMovingDirection(3, false);
			break;

		default:
			break;

	}
}

/*******************************************************************************************************************/

/********************************************Animation**************************************************************/

function animate() {
	'use strict';

    var delta = clock.getDelta(); //Get the seconds passed since the last time it was called.

	render();

	car.move(delta);

    for ( var i = 0; i < oranges.length; i++ ) {

    	oranges[i].move(delta);

    }

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