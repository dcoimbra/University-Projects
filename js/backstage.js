 /*********************************************Variaveis globais*************************************************************/
var camera, scene, renderer;

var car, table;

/* tamanho da area visivel */
var frustumSize;

var clock = new THREE.Clock();


/*-------------------------------------------------------------------------------------------------------------------------*/

/******************************************Criacao da cena*****************************************************************/
function createScene() {

	'use strict';

	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(10));

	//Posicionamento dos vários objetos
	table = new Table(0, 0, 0);

	createBorderLine();

	var orange1 = new Orange(-40, 0, 6);
	var orange2 = new Orange(15, 10, -15);
	var orange3 = new Orange(35, 15, 5);

	var butterPackage1 = new ButterPackage(10, 2, 5);
	var butterPackage2 = new ButterPackage(45, 2, 15);
	var butterPackage3 = new ButterPackage(-10, 2, 0);
	var butterPackage4 = new ButterPackage(-25, 2, 4);
	var butterPackage5 = new ButterPackage(5, 2, -15);

	car = new Car(0, 6, 0);

}
/*------------------------------------------------------------------------------------------------------------------------*/


/********************************************Criacao da camera************************************************************/
function createCamera() {

	'use strict';

	var aspect = window.innerWidth / window.innerHeight;

	frustumSize = 60;

	/*Camera ortogonal inicializada tal como na documentacao do three.js */

	if (window.innerWidth > window.innerHeight) {

		camera = new THREE.OrthographicCamera(- frustumSize * aspect,
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


	scene.add(camera);

	/*Camera posicionada em vista de topo */
	camera.position.x = 0;
	camera.position.y = 25;
	camera.position.z = 0;
	camera.lookAt(scene.position);
}
/*------------------------------------------------------------------------------------------------------------------------*/

/**********************************************Cricao do render************************************************************/
function render() {

	'use strict';

	renderer.render(scene, camera);
}
/*-------------------------------------------------------------------------------------------------------------------------*/
/***************************************Detalhes de visualizacao e controlo****************************************/

function onResize() {

	'use strict';

	var aspect = window.innerWidth / window.innerHeight;

	renderer.setSize(window.innerWidth, window.innerHeight);

	if (window.innerWidth > window.innerHeight) {

		camera.left   = -  frustumSize * aspect;
		camera.right  =    frustumSize * aspect;
		camera.top	  =	   frustumSize;
		camera.bottom =	-  frustumSize;
	}

	else {

		camera.left   = -  frustumSize;
		camera.right  =	   frustumSize;
		camera.top    =	   frustumSize / aspect;
		camera.bottom =	-  frustumSize / aspect;
	}

	camera.updateProjectionMatrix();
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
			car.setMovingState(true);
			break;

		default:
			break;

	}
}
/*---------------------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------*/
function animate() {
	'use strict';

	render();

	car.move();

	requestAnimationFrame(animate);
}

/***************************************************Inicializacao*******************************************************************/
function init() {

	'use strict';

	renderer = new THREE.WebGLRenderer({ antialias: true });

	renderer.setSize(window.innerWidth, window.innerHeight);

	document.body.appendChild(renderer.domElement);

	createScene();
	createCamera();

	render();

	//Adicionados eventos, quando resize and keydown
	window.addEventListener('resize', onResize);
	window.addEventListener('keydown', onKeyDown);
	window.addEventListener('keyup', onKeyUp);
}
