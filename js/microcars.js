/*global THREE*/

 /*********************************************Variaveis globais*************************************************************/
var camera, scene, renderer;

/* tamanho da area visivel */
var frustumSize = 60;
/*-------------------------------------------------------------------------------------------------------------------------*/

/******************************************Criacao da cena*****************************************************************/
function createScene() {

	'use strict';

	scene = new THREE.Scene();
	scene.add(new THREE.AxisHelper(10));

	createTable(0, 0, 0);
	createTrack();
}
/*------------------------------------------------------------------------------------------------------------------------*/


/********************************************Criacao da camera************************************************************/
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
/*------------------------------------------------------------------------------------------------------------------------*/

/**********************************************Cricao do render************************************************************/
function render() {

	'use strict';

	renderer.render(scene, camera);
}
/*-------------------------------------------------------------------------------------------------------------------------*/

/**********************************************Criacao da mesa e suas partes***********************************************/
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

function addTableTop(obj, material, x, y, z) {

	'use strict';

	var tabletop_geometry = new THREE.CubeGeometry(110, 0, 40);
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

/*-----------------------------------------------------------------------------------------------------------------------*/


function createTrack() {

	'use strict';

	var path_material = new THREE.LineBasicMaterial( { color: 0xbada55,
													  linewidth: 3,
												    });
	var path_geometry = new THREE.Geometry();

	var curve = new THREE.CatmullRomCurve3( [

		new THREE.Vector3(-8 * 6, 0.5, -0.5 * 6),
		new THREE.Vector3(-7 * 6, 0.5, 1.5 * 6),
		new THREE.Vector3(-1.5 * 6, 0.5, 0.5 * 6),
		new THREE.Vector3(0.5 * 6, 0.5, -2 * 6),
		new THREE.Vector3(4 * 6, 0.5, 1.5 * 6),
		new THREE.Vector3(6.5 * 6, 0.5, 3 * 6)
	] );

	path_geometry.vertices = curve.getPoints(100);

	var track = new THREE.Line(path_geometry, path_material);

	createBorders(track, curve);

	scene.add(track);

	track.translateZ(-6);
}

function createBorders(obj, line) {

	'use strict';

	var border_Point;
	
	for (var i = 0; i < 1; i += 0.03) {
		
		border_Point = line.getPoint(i);

		addBoundTorus(obj, border_Point);
	}

	var border2 = obj.clone(true);

	scene.add(border2);

	border2.translateZ(3);
}

function addBoundTorus(obj, location) {

	var torus_geometry = new THREE.TorusGeometry( 1, 0.5, 16, 100, Math.PI * 2 );
	var torus_material = new THREE.MeshBasicMaterial( { color: 0xff0000 } );
	var torus = new THREE.Mesh( torus_geometry, torus_material );
	var above_vector = new THREE.Vector3(location.getComponent(0), 
									     1, 
									     location.getComponent(2));

	torus.position.set(location.getComponent(0), 
					   0.5, 
					   location.getComponent(2));

	obj.add(torus);
	torus.lookAt(above_vector);
}
/*----------------------------------------------------------------------------------------------------------------*/

/***************************************Detalhes de visualizacao e controlo****************************************/

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
/*---------------------------------------------------------------------------------------------------------------------------------*/

/***************************************************Inicializacao*******************************************************************/
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
