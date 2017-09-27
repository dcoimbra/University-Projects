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

	//Posicionamento dos vários objetos
	createTable(0, 0, 0);
	createBorderLine();
	createOrange(0, 0, 0);
	createOrange(15, 10, 5);
	createOrange(25, 15, 20);
	createButterPackages();


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

/**********************************************Criacao da mesa e suas partes***********************************************/
function createTable(x, y, z) {

	'use strict';

	var table = new THREE.Object3D();
	var table_material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true });

	addTableTop(table, table_material, 0, 0, 0);

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

/*-----------------------------------------------------------------------------------------------------------------------*/

/*****************************************Criacao da pista e suas borders**************************************************/

function createBorderLine() {

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

/************************************Construção das manteigas e laranjas******************************************/
function createOrange(x, y, z){

	var orange_geometry = new THREE.SphereGeometry(7, 23, 13);
	var orange_material = new THREE.MeshBasicMaterial({color: 0xe49600, wireframe: true });
	var orange = new THREE.Mesh(orange_geometry, orange_material);

	orange.position.x = x;
	orange.position.y = y;
	orange.position.z = z;

	scene.add(orange);

}
/*----------------------------------------------------------------------------------------------------------------*/
function createButterPackages(){

	var x = 0;
	var y = 2;
	var z = 0;

	for(var i = 0; i <= 5; i++){

		var butterPackage_geometry = new THREE.CubeGeometry(15, 0, 15);
		var butterPackage_material = new THREE.MeshBasicMaterial({ color: 0x00bfff, wireframe: true });
		var butterPackage = new THREE.Mesh(butterPackage_geometry, butterPackage_material);

		x += -10;
		y += 7;
		z += 5;

		butterPackage.position.x = x;
		butterPackage.position.y = y;
		butterPackage.position.z = z;

		scene.add(butterPackage);
	}
}
/***************************************Detalhes de visualizacao e controlo****************************************/

function onResize() {

	'use strict';

	var aspect = window.innerWidth / window.innerHeight;

	camera.left = frustumSize * aspect / - 2;
	camera.right = frustumSize * aspect / 2;
	camera.top = frustumSize / 2;
	camera.bottom = frustumSize / - 2;

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

	//Adicionados eventos, quando resize and keydown
	window.addEventListener('resize', onResize);
	window.addEventListener("keydown", onKeyDown);
}
