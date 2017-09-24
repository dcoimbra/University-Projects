/*global THREE*/

var camera, scene, renderer;

/* tamanho da area visivel */
var frustumSize = 200;

function createCamera() {
	
	'use strict'

	var aspect = window.innerWidth / window.innerHeight;

	/* camara ortogonal inicializada tal como na documentacao do three.js */
	camera = new THREE.OrthographicCamera(frustumSize * aspect / - 2, 
										  frustumSize * aspect / 2, 
										  frustumSize / 2, 
										  frustumSize / - 2, 
										  1, 
										  1000);

	/* camara posicionada em vista de topo */
	camera.position.x = 0;
	camera.position.y = 50;
	camera.position.z = 0;
	camera.lookAt(scene.position);
}

function createScene() {
	
	'use strict';

	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(10));
}

function onResize() {

	'use strict';

	var aspect = window.innerWidth / window.innerHeight; 

	camera.left = - frustumSize * aspect / 2;
	camera.right = frustumSize * aspect / 2;
	camera.top = frustumSize / 2;
	camera.bottom = - frustumSize / 2;
	
	camera.updateProjectionMatrix();

	renderer.setSize(window.innerWidth, window.innerHeight);
	
	render();
}

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
	}

	render();
}

function render() {

	'use strict';

	renderer.render(scene, camera);
}

function init() {

	'use strict';

	renderer = new THREE.WebGLRenderer({ antialias: true });

	renderer.setSize(window.innerWidth, window.innerHeight);

	document.body.appendChild(renderer.domElement);

	createScene();
	createCamera();

	render();

	window.addEventListener('resize', onResize);
	window.addEventListener("keydown", onKeyDown);
}