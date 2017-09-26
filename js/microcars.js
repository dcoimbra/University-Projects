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
	careateBorderLine();
}
/*------------------------------------------------------------------------------------------------------------------------*/


/********************************************Criacao da camera************************************************************/
function createCamera() {

	'use strict';

	var aspect = window.innerWidth / window.innerHeight;

	/*Camera ortogonal inicializada tal como na documentacao do three.js */
	camera = new THREE.OrthographicCamera(frustumSize * aspect / - 2,
										  frustumSize * aspect / 2,
										  frustumSize / 2,
										  frustumSize / - 2,
										  1,
										  1000);

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

	//Posicionamento da mesa
	table.position.x = x;
	table.position.y = y;
	table.position.z = z;
}

/*-------------------------------------------------------------------------------------------------------------------*/
function addTableTop(obj, material, x, y, z) {

	'use strict';

	var tabletop_geometry = new THREE.CubeGeometry(110, 0, 45);
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

/*****************************************Criacao da pista e suas borders**************************************************/

function careateBorderLine() {

	'use strict';

	//Criacao da geometria, material e respetiva caminho/linha
	var path_geometry = new THREE.Geometry();
	var path_material = new THREE.LineBasicMaterial( { color: 0x000000,
													  												 linewidth: 3,
													  											 	 opacity: 0,
													  											   transparent: true
												    									 			});
	var border1 = new THREE.Line(path_geometry, path_material);


	//Criacao dos pontos principais que vão constituir a curva
	var curve = new THREE.CatmullRomCurve3( [

		new THREE.Vector3(-8 * 7, 0.5, -0.5 * 6),
		new THREE.Vector3(-7 * 7, 0.5, 1.5 * 6),
		new THREE.Vector3(-1.5 * 7, 0.5, 0.5 * 6),
		new THREE.Vector3(0.5 * 7, 0.5, -2 * 6),
		new THREE.Vector3(4 * 7, 0.5, 1.5 * 6),
		new THREE.Vector3(6.5 * 7, 0.5, 3 * 6)
	] );

	//Junção final da linha curva + vértices do caminho = border1
	//Vertices: The array of vertices holds the position of every vertex in the model.
	//GetPoints: Returns a set of divisions + 1 points using getPoint( t ).
	path_geometry.vertices = curve.getPoints(100);

	//Adicionar a border1 à cena
	scene.add(border1);

	//Posicionamento da pista
	border1.translateZ(-8.5);
	border1.translateX(3);

	//Criar a border de torus
	createTorusBorders(border1, curve);

	//Criação da segunda border2
	//clone: Returns a clone of this Line object and its descendants
	var border2 = border1.clone();

	//Adicionar a border2 à cena
	scene.add(border2);

	//Posicionamento da border2
	border2.translateZ(10);
}

/*-----------------------------------------------------------------------------------------------------*/
function createTorusBorders(obj, line) {

	'use strict';

	var border_Point;

	//Adiciona toros ao longo da linha
	//getPoint: Returns a vector for point t of the curve where t is between 0 and 1.
	for (var i = 0; i < 1; i += 0.025) {

		border_Point = line.getPoint(i);

		addBoundTorus(obj, border_Point);
	}

}

/*---------------------------------------------------------------------------------------------------*/
function addBoundTorus(obj, location) {

	//Criação do toro
	var torus_geometry = new THREE.TorusGeometry(0.5, 0.25, 16, 100, Math.PI * 2);
	var torus_material = new THREE.MeshBasicMaterial( { color: 0xff0000 } );
	var torus = new THREE.Mesh(torus_geometry, torus_material);

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
