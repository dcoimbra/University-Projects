/**************************Construção das manteigas, laranjas****************************************************/
function createOrange(x, y, z){

	'use strict';

	var orange_geometry = new THREE.SphereGeometry(4, 23, 13);
	var orange_material = new THREE.MeshBasicMaterial({color: 0xe49600, wireframe: true });
	var orange = new THREE.Mesh(orange_geometry, orange_material);

	orange.position.x = x;
	orange.position.y = y;
	orange.position.z = z;

	scene.add(orange);

}
/*----------------------------------------------------------------------------------------------------------------*/
function createButterPackages(x, y, z) {

	'use strict';

	var butterPackage_geometry = new THREE.CubeGeometry(6.5 * 2, 0, 3.5 * 2);
	var butterPackage_material = new THREE.MeshBasicMaterial({ color: 0x00bfff, wireframe: true });
	var butterPackage = new THREE.Mesh(butterPackage_geometry, butterPackage_material);

	butterPackage.position.x = x;
	butterPackage.position.y = y;
	butterPackage.position.z = z;

	scene.add(butterPackage);
}
/*----------------------------------------------------------------------------------------------------------------*/

/**********************************************Criacao da mesa e suas partes***********************************************/
function createTable(x, y, z) {

	'use strict';

	var table = new THREE.Object3D();
	var table_material = new THREE.MeshBasicMaterial({ color: 0x007300, wireframe: true });

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

	var tabletop_geometry = new THREE.CubeGeometry(110, 0, 110);
	var tabletop_mesh = new THREE.Mesh(tabletop_geometry, material);
	tabletop_mesh.position.set(x, y, z);

	obj.add(tabletop_mesh);
}
/*------------------------------------------------------------------------------------------------------------------*/

function createBorderLine() {

	'use strict';

	//Criacao da geometria, material e respetiva caminho/linha
	var path_geometry1 = new THREE.Geometry();
	var path_geometry2 = new THREE.Geometry();


	var path_material = new THREE.LineBasicMaterial( { color: 0x555555,
													   linewidth: 3,
													   opacity: 0,
													   transparent: true,
													  
												   	 });

	var border1 = new THREE.Line(path_geometry1, path_material);
	var border2 = new THREE.Line(path_geometry2, path_material);

	//Criacao dos pontos principais que vão constituir a curva
	var curve1 = new THREE.CatmullRomCurve3( [

		new THREE.Vector3(-6 * 9, 0.5, 4.5 * 9),
		new THREE.Vector3(-4.5 * 9, 0.5, 4 * 9),
		new THREE.Vector3(-4.5 * 8, 0.5, -1 * 10),
		new THREE.Vector3(-7.2 * 8, 0.5, -1 * 10),
		new THREE.Vector3(-6 * 8, 0.5, -4 * 10),
		new THREE.Vector3(-2 * 8, 0.5, -5 * 10),
		new THREE.Vector3(-1 * 10, 0.5, -3 * 10),
		new THREE.Vector3(-1 * 10, 0.5, -1),
		new THREE.Vector3(1 * 10, 0.5, -1),
		new THREE.Vector3(3 * 10, 0.5, -3 * 10),
		new THREE.Vector3(4.5 * 10, 0.5, -1 * 10),
		new THREE.Vector3(1.5 * 10, 0.5, 4.9 * 10),
		new THREE.Vector3(-4 * 9, 0.5, 6.1 * 9),
		new THREE.Vector3(-6 * 9, 0.5, 5.9 * 9),
		new THREE.Vector3(-6 * 9, 0.5, 4.5 * 9),
	] );

	var curve2 = new THREE.CatmullRomCurve3( [

		new THREE.Vector3(-5 * 9, 0.5, 5 * 9),
		new THREE.Vector3(-3 * 9, 0.5, 4.5 * 9),
		new THREE.Vector3(-3.5 * 8, 0.5, -2 * 10),
		new THREE.Vector3(-5.5 * 8, 0.5, -2 * 10),
		new THREE.Vector3(-6 * 8, 0.5, -2.5 * 10),
		new THREE.Vector3(-2.5 * 8, 0.5, -4.25 * 10),
		new THREE.Vector3(-2 * 10, 0.5, -3 * 10),
		new THREE.Vector3(-1.5 * 10, 0.5, 8),
		new THREE.Vector3(1.5 * 10, 0.5, 10),
		new THREE.Vector3(3 * 10, 0.5, -2 * 10),
		new THREE.Vector3(3.5 * 10, 0.5, -1 * 10),
		new THREE.Vector3(1 * 10, 0.5, 4 * 10),
		new THREE.Vector3(-5 * 9, 0.5, 5.25 * 9),
		new THREE.Vector3(-5 * 9, 0.5, 5 * 9),
	] );


	//Junção final da linha curva + vértices do caminho = border1
	//Vertices: The array of vertices holds the position of every vertex in the model.
	//GetPoints: Returns a set of divisions + 1 points using getPoint( t ).
	path_geometry1.vertices = curve1.getPoints(100);
	path_geometry2.vertices = curve2.getPoints(100);

	//Adicionar a border1 à cena
	scene.add(border1);
	scene.add(border2);

	border1.translateX(5);
	border1.translateZ(-2);

	border2.translateX(5);
	border2.translateZ(-2);

	border1.scale.multiplyScalar(1);

	//Criar a border de torus
	createTorusBorders(border1, curve1);
	createTorusBorders(border2, curve2);

	//Criação da segunda border2
	//clone: Returns a clone of this Line object and its descendants
  //	var border2 = border1.clone();

	//Adicionar a border2 à cena
	//scene.add(border2);

	//Posicionamento da border2
	//border2.translateZ(10);
}

/*-----------------------------------------------------------------------------------------------------*/
function createTorusBorders(obj, line) {

	'use strict';

	var border_Point;

	var length = line.getLength();

	var division = length / 40000; 

	//Adiciona toros ao longo da linha
	//getPoint: Returns a vector for point t of the curve where t is between 0 and 1.
	for (var i = 0; i < 1; i += division) {

		border_Point = line.getPointAt(i);

		addBoundTorus(obj, border_Point);
	}
}

/*---------------------------------------------------------------------------------------------------*/
function addBoundTorus(obj, location) {

	'use strict';
	//Criação do toro
	var torus_geometry = new THREE.TorusGeometry(0.5, 0.25, 8, 16, Math.PI * 2);
	var torus_material = new THREE.MeshBasicMaterial( { color: 0xff0000 } );
	var torus = new THREE.Mesh(torus_geometry, torus_material);

	var above_vector = new THREE.Vector3(location.getComponent(0),
									     1,
									     location.getComponent(2));

	torus.position.set(location.getComponent(0),
					   0.5,
					   location.getComponent(2));

	obj.add(torus);

	//LookAt: Rotates the object to face a point in world space.
	torus.lookAt(above_vector);
}
/*-------------------------------------------------------------------------------------------------------*/
