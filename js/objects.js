/**************************Classe das laranjas****************************************************/

class Orange {

    constructor(x, y, z) {

        'use strict';

        /* Orange: radius: 4
                   width segments: 17
                   height segments: 13*/

        var orange_geometry = new THREE.SphereGeometry(4, 17, 13);
        var orange_material = new THREE.MeshBasicMaterial({color: 0xe49600, wireframe: true});
        this.orange_object = new THREE.Mesh(orange_geometry, orange_material);

        this.orange_object.userData = {speed: 10};

        this.orange_object.position.set(x, y, z);

        this.createOrangeStalk();

        scene.add(this.orange_object);

    }

    createOrangeStalk() {

        'use strict';

        var stalk_geometry = new THREE.CylinderGeometry(0.5, 0.5, 1, 32);
        var stalk_material = new THREE.MeshBasicMaterial({color: 0x00ff00, wireframe: true});
        var stalk = new THREE.Mesh(stalk_geometry, stalk_material);

        stalk.position.set(0, 4, 0);

        this.orange_object.add(stalk);
    }

    increaseOrangeSpeed(speed) {
        this.orange_object.userData.speed += speed;
    }

    setOrangeSpeed(speed) {
        this.orange_object.userData.speed = speed;
    }

    getOrangeSpeed() {
        return this.orange_object.userData.speed;
    }


    move(delta) {


        var orange_speed = this.getOrangeSpeed();

        var distance = orange_speed * delta;

        this.orange_object.translateX(distance);
    }
}

function updateOrangeSpeed() {

	'use strict';

	for (var i = 0; i < oranges.length; i++) {

		oranges[i].increaseOrangeSpeed(5);
	}
}

/******************************************************************************************************************/


/************************************Classe das manteigas**********************************************************/

class ButterPackage {

	constructor(x, y, z) {

		'use strict';

		/* ButterPackage: width: 13   (relative to camera)
						  height: 7
						  depth: 2
		*/

		var butterPackage_geometry = new THREE.BoxGeometry(13, 2, 7);
		var butterPackage_material = new THREE.MeshBasicMaterial({ color: 0x00bfff, wireframe: true });

		this.butterPackage_object = new THREE.Mesh(butterPackage_geometry, butterPackage_material);

		this.butterPackage_object.position.set(x, y, z);

		scene.add(this.butterPackage_object);
	}
}
/*******************************************************************************************************************/


/**********************************************Criacao da mesa e suas partes******************************************/


class Table {

	constructor(x, y, z) {

	'use strict';

	this.table_object = new THREE.Object3D();

	var table_material = new THREE.MeshBasicMaterial({ color: 0x007300, wireframe: true });

	this.addTableTop(table_material, 0, 0, 0);

	scene.add(this.table_object);

	//Posicionamento da mesa
	this.table_object.position.set(x, y, z);
	}

/*-------------------------------------------------------------------------------------------------------------------*/
	addTableTop(material, x, y, z) {

		'use strict';

		/*TableTop: width: 110   (relative to camera)
					height: 110
					depth: 3*/

		var tabletop_size = 110;

		var tabletop_geometry = new THREE.BoxGeometry(tabletop_size , 3, tabletop_size);
		var tabletop_mesh = new THREE.Mesh(tabletop_geometry, material);
		tabletop_mesh.position.set(x, y, z);

		this.table_object.add(tabletop_mesh);
	}
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

		new THREE.Vector3(-53.1, 0.5, 40.5),
		new THREE.Vector3(-44.1, 0.5, 36),
		new THREE.Vector3(-43.2, 0.5, 34.2),
		new THREE.Vector3(-40.5, 0.5, 36),
		new THREE.Vector3(-36, 0.5, -10),
		new THREE.Vector3(-57.6, 0.5, -10),
		new THREE.Vector3(-48, 0.5, -40),
		new THREE.Vector3(-16, 0.5, -50),
		new THREE.Vector3(-10, 0.5, -30),
		new THREE.Vector3(-10, 0.5, -1),
		new THREE.Vector3(10, 0.5, -1),
		new THREE.Vector3(30, 0.5, -30),
		new THREE.Vector3(45, 0.5, -10),
		new THREE.Vector3(25, 0.5, 40),
		new THREE.Vector3(23, 0.5, 42),
		new THREE.Vector3(20, 0.5, 45),
		new THREE.Vector3(15, 0.5, 48),
		new THREE.Vector3(15, 0.5, 49),
		new THREE.Vector3(-36, 0.5, 54.9),
		new THREE.Vector3(-54, 0.5, 53.1),
		new THREE.Vector3(-54, 0.5, 45),
		new THREE.Vector3(-54, 0.5, 40.5),
	] );

	var curve2 = new THREE.CatmullRomCurve3( [

		new THREE.Vector3(-45, 0.5, 45),
		new THREE.Vector3(-31.5, 0.5, 40.5),
		new THREE.Vector3(-28, 0.5, -20),
		new THREE.Vector3(-44, 0.5, -20),
		new THREE.Vector3(-48, 0.5, -25),
		new THREE.Vector3(-20, 0.5, -42.5),
		new THREE.Vector3(-20, 0.5, -30),
		new THREE.Vector3(-15, 0.5, 8),
		new THREE.Vector3(15, 0.5, 10),
		new THREE.Vector3(30, 0.5, -20),
		new THREE.Vector3(35, 0.5, -10),
		new THREE.Vector3(10, 0.5, 40),
		new THREE.Vector3(-45, 0.5, 47.25),
		new THREE.Vector3(-45, 0.5, 45),
	] );

	//Adicionar border1 e border2 à cena
	scene.add(border1);
	scene.add(border2);

	//Posicionar
	border1.translateX(5);
	border1.translateZ(-2);

	border2.translateX(5);
	border2.translateZ(-2);

	//Criar a border de torus
	createTorusBorders(border1, curve1);
	createTorusBorders(border2, curve2);
}

/*-----------------------------------------------------------------------------------------------------------*/
function createTorusBorders(obj, line) {

	'use strict';

	var border_point;

	var length = line.getLength();

	var division = length / 40000;

	//Adiciona toros ao longo da linha
	//getPoint: Returns a vector for point t of the curve where t is between 0 and 1.
	for (var i = 0; i < 1; i += division) {

		border_point = line.getPointAt(i);

		addBoundTorus(obj, border_point);
	}
}

/*------------------------------------------------------------------------------------------------------*/
function addBoundTorus(obj, location) {

	'use strict';

	/* BoundTorus:

		radius: 0.5
		tube diameter: 0.25
		radial segments: 4
		tubular segments: 8
		central angle: 2π 
	*/

	var torus_geometry = new THREE.TorusGeometry(0.5, 0.25, 4, 8, Math.PI * 2);
	var torus_material = new THREE.MeshBasicMaterial( { color: 0xff0000, wireframe: true } );
	var torus = new THREE.Mesh(torus_geometry, torus_material);

	//Make torus look to the camera
	var above_vector = new THREE.Vector3(location.getComponent(0),
									     10,
									     location.getComponent(2));

	torus.position.set(location.getComponent(0),
					   1.75,
					   location.getComponent(2));

	obj.add(torus);

	//LookAt: Rotates the object to face a point in world space.
	torus.lookAt(above_vector);
}
/*-------------------------------------------------------------------------------------------------------*/
/********************************************************************************************************************/