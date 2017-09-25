/*global THREE*/

var camera, scene, renderer;

/* tamanho da area visivel */
var frustumSize = 100;

function addTableTop(obj, material, x, y, z) {

	'use strict';

	var tabletop_geometry = new THREE.CubeGeometry(110, 2, 40);
	var tabletop_mesh = new THREE.Mesh(tabletop_geometry, material);
	tabletop_mesh.position.set(x, y, z);

	obj.add(tabletop_mesh);
}

/* DESCOMENTAR QUANDO NECESSARIO 
function addTableLeg(obj, material, x, y, z) {

	'use strict';

	var tableleg_geometry = new THREE.CubeGeometry(2, 6, 2);
	var tableleg_mesh = new THREE.Mesh(tableleg_geometry, material);
	tableleg_mesh.position.set(x, y - 3, z);
	
	obj.add(tableleg_mesh);
} */

function createTable(x, y, z) {

	'use strict';

	var table = new THREE.Object3D();

	var table_material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true });

	addTableTop(table, table_material, 0, 0, 0);
	/* addTableLeg(table, table_material, -25, -1, -8);
	addTableLeg(table, table_material, -25, -1, 8);
	addTableLeg(table, table_material, 25, -1, 8);
	addTableLeg(table, table_material, 25, -1, -8); */

	scene.add(table);

	table.position.x = x;
	table.position.y = y;
	table.position.z = z;
}

function createTrack() {

	'use strict';

	var pathMaterial = new THREE.LineBasicMaterial( { color: 0xbada55, 
													  linewidth: 5,
												    });
	var pathGeometry = new THREE.Geometry();

	var track = new THREE.CatmullRomCurve3( [
		
		new THREE.Vector3(-8 * 6, 0, -0.5 * 6),
		new THREE.Vector3(-7 * 6, 0, 1.5 * 6),
		new THREE.Vector3(-1.5 * 6, 0, 0.5 * 6),
		new THREE.Vector3(0.5 * 6, 0, -2 * 6),
		new THREE.Vector3(4 * 6, 0, 1.5 * 6),
		new THREE.Vector3(6.5 * 6, 0, 3 * 6)
	] );

	pathGeometry.vertices = track.getPoints(100);

	var curve = new THREE.Line(pathGeometry, pathMaterial);

	scene.add(curve);
}

function createCamera() {
	
	'use strict';

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
	camera.position.y = 25;
	camera.position.z = 0;
	camera.lookAt(scene.position);
}

function createScene() {
	
	'use strict';

	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(10));

	createTable(0, 0, 0);
	createTrack();
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