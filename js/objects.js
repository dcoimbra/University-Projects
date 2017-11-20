/***************************************Classe das colisoes**********************************************************/
class Collidable {

	constructor(obj) {

		'use strict';

		this.inner_object = obj;
	}

	/*---------------------------------------------------------------------------------------------------------------*/
	collisionSphere(other_collidable) {

	    'use strict';

        var radiusSum = this.inner_object.bounding.radius + other_collidable.inner_object.bounding.radius;
        var object_center = this.inner_object.bounding.center;

		//distancia dos centros < soma dos raios --> colisão
       	return other_collidable.inner_object.bounding.center.distanceToSquared(object_center) <= ( radiusSum * radiusSum );
	}
}

/********************************************Classe das laranjas****************************************************/

class Orange {

    constructor(x, y, z) {

        'use strict';

        /* Orange: radius: 4
                   width segments: 17
                   height segments: 13*/

        var orange_geometry = new THREE.SphereGeometry(4, 17, 13);

        var orange_material = new THREE.MeshPhongMaterial({ color: 0xe49600,
            												specular: 0xdb4200,
														    shininess: 10,
															wireframe: false,
															opacity: 0,
															transparent: false,
															map: new THREE.TextureLoader().load('resources/textures/hm2.jpg')});


        this.orange_object = new THREE.Mesh(orange_geometry, orange_material);

        this.orange_object.name = "orange";

        this.orange_object.userData = { speed: 5, changePos: false,
									    specular: orange_material.specular,
			                            shininess: orange_material.shininess};

        this.orange_object.position.set(x, y, z);

        this.createOrangeStalk();

        scene.add(this.orange_object);

        this.makeBounding();

    }
	/*--------------------------------------------------------------------------------------------------------------*/
    //Criação do caule da laranja
    createOrangeStalk() {

        'use strict';

        var stalk_geometry = new THREE.CylinderGeometry(0.5, 0.5, 1, 32);
        var stalk_material = new THREE.MeshPhongMaterial({ color: 0x00ff00,
													       specular: 0x00ee00,
														   shininess: 0,
														   wireframe: false,
														   opacity: 0,
														   transparent: false });

        var stalk = new THREE.Mesh(stalk_geometry, stalk_material);

        stalk.userData = { specular: stalk_material.specular,
						   shininess: stalk_material.shininess };

        stalk.position.set(0, 4, 0);

        stalk.name = 'stalk';

        this.orange_object.add(stalk);
    }

    makeBounding() {

        'use strict';
        var orange_radius = this.orange_object.geometry.parameters.radius;
        var orange_center = this.orange_object.getWorldPosition();

        this.bounding = new THREE.Sphere(orange_center, orange_radius );
    }

    increaseOrangeSpeed(speed) {
    	'use strict';
        this.orange_object.userData.speed += speed;
    }

    getOrangeSpeed() {
    	'use strict';
        return this.orange_object.userData.speed;
    }

    setOrangeSpeed(speed) {
    	'use strict';
        this.orange_object.userData.speed = speed;
    }

    setOrangeTransparency(){
    	'use strict';
    	this.orange_object.material.transparent = true;

    	var stalk = this.orange_object.getObjectByName('stalk');
		stalk.material.transparent = true;
    }

    move(delta) {
    	'use strict';

    	var orange_Xposition = this.orange_object.position.x;
    	var orange_Zposition = this.orange_object.position.z;

    	//Se a laranja ultrapassar os limites da table
    	if(orange_Xposition > 55 || orange_Xposition < -55 ||
			 orange_Zposition> 55 || orange_Zposition < -55){

			if (this.orange_object.userData.changePos){

				this.setOrangeTransparency();
				orangeTimeout = new Timer(this.changingPos, 5000, this.orange_object);
				this.orange_object.userData.changePos = false;
			}
		}

		else{
    		this.orange_object.userData.changePos = true;

    		var orange_speed = this.getOrangeSpeed();
			var distance = orange_speed * delta;

			//Mudanca da posicao da laranja (vai rolar sobre a table)
			// Temos a nossa distancia a ser percorrida mas como queremos que a sua direção tenha um ângulo
			// vamos ter de calcular as componentes em x e z
			// x: cos = comp_x / distance <=> comp_x = cos(angulo atual)*distance
			// z: sin = comp_z / distance <=> comp_z = sin(angulo atual)*distance
			this.orange_object.position.x += distance * Math.cos(this.orange_object.rotation.y);
			this.orange_object.position.z += distance * Math.sin(this.orange_object.rotation.y);

			//Rolamento da laranja (si mesma)
			this.orange_object.rotateZ(-0.01);
		}

		this.bounding.center = this.orange_object.getWorldPosition();
	}

	//Mudamos a laranja de posicao
	changingPos(object) {
		'use strict';

    	//Nova posicao random da laranja (x, z)
    	object.position.x = randomPos();
		object.position.z = randomPos();
		object.userData.speed = (Math.random()*10);


		//Angulo de rotacao random à volta de y
		object.rotation.y += (2*Math.PI*Math.random());

		//Colocar a laranja e o caule visiveis
		object.material.transparent = false;

		var stalk = object.getObjectByName('stalk');
		stalk.material.transparent = false;
	}
}

/*--------------------------------------------------------------------------------------------------------------*/

function checkOrangeInterval() {

    var time = intervalClock.getElapsedTime();

    if (time >= 5) {

        updateOrangeSpeed();
        intervalClock.start();
    }
}

/*--------------------------------------------------------*/
function updateOrangeSpeed() {

	'use strict';

	for (var i = 0; i < oranges.length; i++) {

		oranges[i].inner_object.increaseOrangeSpeed(5);
	}
}
/*--------------------------------------------------------*/

//Funcao que retorna um numero random entre -55 e 55
function randomPos(){
	return Math.floor((Math.random() * 110) + (-55));
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
		var butterPackage_material = new THREE.MeshPhongMaterial({
																   specular: 0x00aeee,
																   shininess: 5,
																   wireframe: false,
															       map: new THREE.TextureLoader().load(("resources/textures/hm6.jpg"))});

		this.butterPackage_object = new THREE.Mesh(butterPackage_geometry, butterPackage_material);

		this.butterPackage_object.name ="butterp";

        this.butterPackage_object.userData = { specular: butterPackage_material.specular,
                                               shininess: butterPackage_material.shininess };

		scene.add(this.butterPackage_object);

        this.butterPackage_object.position.set(x, y, z);

		this.makeBounding();
	}

	makeBounding() {
		'use strict';

		var butterPackage_width = this.butterPackage_object.geometry.parameters.width;

        this.bounding = new THREE.Sphere(this.butterPackage_object.position, butterPackage_width / 2 + 0.25);
    }
}
/*******************************************************************************************************************/

/******************************************* Classe dos torus da pista**********************************************/

class BorderTorus {

    /* BorderTorus:

        radius: 0.5
        tube diameter: 0.25
        radial segments: 4
        tubular segments: 8
        central angle: 2π
    */

    constructor(border, location) {

        'use strict';

        var torus_geometry = new THREE.TorusGeometry(0.75, 0.3, 4, 8, Math.PI * 2);
        var torus_material = new THREE.MeshPhongMaterial({ color: 0x000000,
                                                           specular: 0x111111,
                                                           shininess: 5,
                                                           wireframe: false });

        this.torus_object = new THREE.Mesh(torus_geometry, torus_material);

        this.torus_object.userData = { speed: 0,
                                       car_collision_direction: new THREE.Vector3(0, 0, 0),
                                       torus_collision_direction: new THREE.Vector3(0, 0, 0),
                                       specular: torus_material.specular,
                                       shininess: torus_material.shininess };

        this.torus_object.position.set(location.getComponent(0),
                                       1.75,
                                       location.getComponent(2));

        //Make torus look up
        var above_vector = new THREE.Vector3(this.torus_object.getWorldPosition().getComponent(0),
                                             10,
                                             this.torus_object.getWorldPosition().getComponent(2));

        //LookAt: Rotates the object to face a point in world space.
        this.torus_object.lookAt(above_vector);

        border.add(this.torus_object);

        this.makeBounding();
    }
	/*---------------------------------------------------------------------------------------------------------------*/
    move(delta) {

        'use strict';

        var speed = this.torus_object.userData.speed;

        if (speed > 0) {

            var displacement = speed * delta;

            /* transformar o vetor nas coordenadas do toro para as coordenadas do mundo e aplicar a translação */
            this.torus_object.position.x += this.torus_object.userData.car_collision_direction.x * displacement;
            this.torus_object.position.z += this.torus_object.userData.car_collision_direction.z * displacement;

            this.torus_object.position.x += this.torus_object.userData.torus_collision_direction.x * displacement;
            this.torus_object.position.z += this.torus_object.userData.torus_collision_direction.z * displacement;

            /* mover bounding sphere para a nova posição */

            this.bounding.center = new THREE.Vector3((this.torus_object.position.getComponent(0) + 5),
                                                      1.75,
                                                      (this.torus_object.position.getComponent(2) - 2));


            /* desacelerar o toro */
            this.torus_object.userData.speed -= (20 * delta);

            if (this.torus_object.userData.speed < 0) {

                this.torus_object.userData.speed = 0;
			}
        }
	}

	/*-------------------------------------------------------------------------------------------------------------*/
    makeBounding() {
		'use strict';

       var torus_position = new THREE.Vector3((this.torus_object.position.getComponent(0) + 5),
			 						           1.75,
            						          (this.torus_object.position.getComponent(2) - 2));

       var torus_radius = this.torus_object.geometry.parameters.radius;
       var torus_tube = this.torus_object.geometry.parameters.tube;

       this.bounding = new THREE.Sphere(torus_position, torus_radius + torus_tube);
    }
    /*-------------------------------------------------------------------------------------------------------------*/
}

/**********************************************Criacao da mesa e suas partes******************************************/


class Table {

	constructor(x, y, z) {

	    'use strict';

	    this.table_object = new THREE.Object3D();

	    var table_material = new THREE.MeshPhongMaterial({ color: 0xffffff,
                                                           specular: 0x222222,
                                                           shininess: 10,
                                                           wireframe: false,
                                                            map: new THREE.TextureLoader().load('resources/textures/hm1.jpg')});

	    this.addTableTop(table_material, 0, 0, 0);

	    scene.add(this.table_object);

	    //Posicionamento da mesa
	    this.table_object.position.set(x, y, z);
	}

	/*-----------------------------------------------------------------------------------------------------------------*/
	addTableTop(material, x, y, z) {

		'use strict';

		/*TableTop: width: 110   (relative to camera)
					height: 110
					depth: 3*/

		var tabletop_size = 110;

		var tabletop_geometry = new THREE.BoxGeometry(tabletop_size , 3, tabletop_size);
		var tabletop_mesh = new THREE.Mesh(tabletop_geometry, material);

        tabletop_mesh.name = "table";

        tabletop_mesh.userData = { specular: tabletop_mesh.material.specular,
           						   shininess: tabletop_mesh.material.shininess,
        						 };

		tabletop_mesh.position.set(x, y, z);

		this.table_object.add(tabletop_mesh);

		//this.addTableCloth(1.5, tabletop_size);
	}

	addTableCloth(y, size) {

	    var tablecloth_geometry = new THREE.BoxGeometry(size, 0.005, size);
	    var tablecloth_material = new THREE.MeshPhongMaterial({
                                                                specular: 0x222222,
                                                                shininess: 10,
                                                                wireframe: false,
                                                                map: new THREE.TextureLoader().load('resources/textures/hm1.jpg')});

        var tablecloth_mesh = new THREE.Mesh(tablecloth_geometry, tablecloth_material);

        tablecloth_mesh.userData = { specular: tablecloth_mesh.material.specular,
                                     shininess: tablecloth_mesh.material.shininess,
                                   };

        tablecloth_mesh.position.set(0, y, 0);

        this.table_object.children[0].add(tablecloth_mesh);
    }
}


/*------------------------------------------------------------------------------------------------------------------*/

function createBorderLine() {

	'use strict';

	//Criacao da geometria, material e respetiva caminho/linha
	var path_geometry1 = new THREE.Geometry();
	var path_geometry2 = new THREE.Geometry();


	var path_material = new THREE.LineBasicMaterial( { color: 0xaaaaaa,
													   linewidth: 3
												   	 });

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
		new THREE.Vector3(-54, 0.5, 40.5)
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
		new THREE.Vector3(-45, 0.5, 45)
	] );

    path_geometry1.vertices = curve1.getPoints( 100 );
    path_geometry2.vertices = curve2.getPoints( 100 );

    var border1 = new THREE.Line(path_geometry1, path_material);
    var border2 = new THREE.Line(path_geometry2, path_material);

    border1.userData = { toruses: [] };
    border2.userData = { toruses: [] };

	//Adicionar border1 e border2 à cena
	scene.add(border1);
	scene.add(border2);

	border1.position.x += 5;
	border1.position.z += -2;
    border2.position.x += 5;
    border2.position.z += -2;
	
	//Criar a border de torus
	createTorusBorders(border1, curve1);
	createTorusBorders(border2, curve2);


    border_lines.push(border1);
    border_lines.push(border2);
}

/*-----------------------------------------------------------------------------------------------------------*/
function createTorusBorders(obj, line) {

	'use strict';

	var border_point;

	var length = line.getLength();

	var division = length / 20000;

	//Adiciona toros ao longo da linha
	//getPointAt: Returns a vector for point t of the curve where t is between 0 and 1.
	for (var i = 0; i < 1; i += division) {

		border_point = line.getPointAt(i);

        obj.userData.toruses.push(new Collidable(new BorderTorus(obj, border_point)));
	}
}

/*------------------------------------------------------------------------------------------------------*/
/********************************************************************************************************************/