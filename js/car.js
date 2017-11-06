/******************************************Criaçao do carro**************************************/

// Classe carro
class Car {

	constructor(x, y, z) {

		'use strict';

		this.car_object = new THREE.Object3D();

		this.car_object.userData = { speed: 0,
					 			     maxSpeed: 40,
					 			     acceleration: 20,
					 		         movingDirection: [ false, false, false, false ] // [left, up, right, down]
							   	   };

		this.car_object.position.set(x, y, z);

		this.build();
	}

	/*----------------------------------------------------------------------------------------------------------*/
	build() {

		'use strict';

		//Elements are instantiated relative to the car's position
		this.createChassis(-10, -5, 5);

		this.createHood( -9.7, 0, 3);

		this.createWindShieldSide( -4.5,  2.7,  -3.3); // left
		this.createWindShieldSide( -4.5,  2.7,  3.3); // right

		this.createWheel( -8,  -6.5,  5.8); // back-right
		this.createWheel( 4, -6.5, 6.1); // front-right
		this.createWheel( -8,  -6.5, -5.6); // back-left
		this.createWheel( 4,  -6.5,  -5.7); // front-left

        this.createFollowingCamera(-80, 80, 0);

        this.makeBounding();

		scene.add(this.car_object);

        this.car_object.scale.multiplyScalar(0.2);
        this.car_object.getObjectByName("camera").scale.divideScalar(0.2);
	}

	/*----------------------------------------------------------------------------------------------------------*/

	createChassis(x, y, z) {

		'use strict';

		/* Chassis: width: 13  (relative to camera)
		            height: 5
		            depth: 5
		*/

		var chassis_material = new THREE.MeshPhongMaterial({ color: 0x0000ff,
                                                             specular: 0x0000ff,
                                                             shininess: 40,
                                                             wireframe: true });

		// Create the custom mesh.
		var chassis_geometry = new THREE.Geometry();

		// The vertexes
		chassis_geometry.vertices = [

			// bottom base ones
			new THREE.Vector3(0, 0, 0), //index 0
			new THREE.Vector3(10, 0, 0), //index 1
			new THREE.Vector3(0, 0, 20),//index 2
			new THREE.Vector3(10, 0, 20), //index 3

			// top base vertexes
			new THREE.Vector3(0, 5, 0), //index 4
			new THREE.Vector3(10, 5, 0), //index 5
			new THREE.Vector3(0, 5, 20),//index 6
			new THREE.Vector3(10, 5, 20), //index 7

			//center vertexes
			new THREE.Vector3(5, 0, 10), //bottom index 8
			new THREE.Vector3(5, 5, 10),	//top index 9
			new THREE.Vector3(5, 2.5, 0),	//left index 10
			new THREE.Vector3(5, 2.5, 20), //right index 11
			new THREE.Vector3(5, 2.5, 10)	//front index 12

		];

		// The faces
		// Array of faces.
		// The array of faces describe how each vertex in the model is connected to form faces.
		// Additionally it holds information about face and vertex normals and colors
		chassis_geometry.faces = [

			// bottom base face
			new THREE.Face3(0, 8, 1),
			new THREE.Face3(1, 8, 3),
			new THREE.Face3(3, 8, 2),
			new THREE.Face3(2, 8, 0),

			//Right face
			new THREE.Face3(7, 11, 3),
			new THREE.Face3(3, 11, 2),
			new THREE.Face3(2, 11, 6),
			new THREE.Face3(6, 11, 7),

			//Top base
			new THREE.Face3(4, 9, 5),
			new THREE.Face3(5, 9, 7),
			new THREE.Face3(7, 9, 6),
			new THREE.Face3(6, 9, 4),


			//Left face
			new THREE.Face3(4, 10, 0),
			new THREE.Face3(0, 10, 1),
			new THREE.Face3(1, 10, 5),
			new THREE.Face3(5, 10, 4),


			//Front face
			new THREE.Face3(5, 12, 1),
			new THREE.Face3(1, 12, 3),
			new THREE.Face3(3, 12, 7),
			new THREE.Face3(7, 12, 5),


			//Back face
			new THREE.Face3(0, 2, 6),
			new THREE.Face3(6, 4, 0)

		];
		// Compute face normals
		chassis_geometry.computeFaceNormals();
		chassis_geometry.computeVertexNormals();

		// Create the mesh for the body.
		var chassis = new THREE.Mesh(chassis_geometry, chassis_material);
		chassis.name = "chassis";

		chassis.userData = { specular: chassis_material.specular,
                             shininess: chassis_material.shininess };

		this.car_object.add(chassis);
        chassis.position.set(x, y, z);
		chassis.rotateY(Math.PI/2);
	}

	/*----------------------------------------------------------------------------------------------------------*/

	createHood(x, y, z) {

		'use strict';

		/* Hood:    width: 10 (relative to camera)
		            height: 5
		            depth: 6
		*/



		var hood_material = new THREE.MeshPhongMaterial({ color: 0xff0000,
                                                          specular: 0xff0000,
                                                          shininess: 40,
                                                          wireframe: true });

		// Create the custom mesh.
		var hood_geometry = new THREE.Geometry();

		hood_geometry.vertices = [

			// bottom base ones
			new THREE.Vector3(0, 0, 0), //index 0
			new THREE.Vector3(6, 0, 0), //index 1
			new THREE.Vector3(0, 0, 10),//index 2
			new THREE.Vector3(6, 0, 10), //index 3

			// top base vertexes
			new THREE.Vector3(0, 5, 3), //index 4
			new THREE.Vector3(6, 5, 3), //index 5
			new THREE.Vector3(0, 5, 7),//index 6
			new THREE.Vector3(6, 5, 7), //index 7

			//center vertexes
			new THREE.Vector3(6, 0, 5), 		//bottom index 8
			new THREE.Vector3(6, 5, 5),			//top index 9
			new THREE.Vector3(3.5, 2.5, 0),		//left index 10
			new THREE.Vector3(3.5, 2.5, 10), 	//right index 11
			new THREE.Vector3(6, 2.5, 10)		//front index 12

		];

		// The faces
		// Array of faces.
		// The array of faces describe how each vertex in the model is connected to form faces.
		// Additionally it holds information about face and vertex normals and colors
		hood_geometry.faces = [

			// bottom base face
			new THREE.Face3(0, 8, 1),
			new THREE.Face3(1, 8, 3),
			new THREE.Face3(3, 8, 2),
			new THREE.Face3(2, 8, 0),

			//Right face
			new THREE.Face3(7, 11, 3),
			new THREE.Face3(3, 11, 2),
			new THREE.Face3(2, 11, 6),
			new THREE.Face3(6, 11, 7),

			//Top base
			new THREE.Face3(4, 9, 5),
			new THREE.Face3(5, 9, 7),
			new THREE.Face3(7, 9, 6),
			new THREE.Face3(6, 9, 4),


			//Left face
			new THREE.Face3(4, 10, 0),
			new THREE.Face3(0, 10, 1),
			new THREE.Face3(1, 10, 5),
			new THREE.Face3(5, 10, 4),


			//Front face
			new THREE.Face3(5, 12, 1),
			new THREE.Face3(1, 12, 3),
			new THREE.Face3(3, 12, 7),
			new THREE.Face3(7, 12, 5),


			//Back face
			new THREE.Face3(0, 2, 6),
			new THREE.Face3(6, 4, 0)

		];

		// Compute face normals
		hood_geometry.computeFaceNormals();
		hood_geometry.computeVertexNormals();

		// Create the mesh for the body.
		var hood = new THREE.Mesh(hood_geometry, hood_material);
		hood.name = "hood";

		hood.userData = { specular: hood_material.specular,
                          shininess: hood_material.shininess };

		this.car_object.add(hood);
		hood.position.set(x, y, z);
		hood.rotateY(Math.PI/2);
	}

	/*------------------------------------------------------------------------------------------------------*/

	createWindShieldSide(x, y, z) {

		'use strict';

		/* Widshield (side):  width: 5.5  (relative to camera)
		                      height: 0
		                      depth: 2
		*/

		var windshield_sideGeometry = new THREE.BoxGeometry(5.5, 2, 0);
		var windshield_sideMaterial = new THREE.MeshPhongMaterial({ color: 0xffffff,
                                                                    specular: 0xffffff,
                                                                    shininess: 40,
                                                                    wireframe: true });

		var windshield_side = new THREE.Mesh(windshield_sideGeometry, windshield_sideMaterial);

		windshield_side.userData = { specular: 0xffffff,
		                             shininess: 40};

		this.car_object.add(windshield_side);
        windshield_side.position.set(x, y, z);
	}

	/*---------------------------------------------------------------------------------------------------------*/

	createWheel(x, y, z) {

		'use strict';

		/* Wheel: radius: 2
				  tube diameter: 0.6
				  radial segments: 8
				  tubular segments: 16
				  central angle: 2π
	    */

		var wheel_material = new THREE.MeshPhongMaterial( { color: 0x333333,
                                                            specular: 0x222222,
                                                            shininess: 5,
                                                            wireframe: true } );

		// Create the custom mesh.
		var wheel_geometry = new THREE.Geometry();

		wheel_geometry.vertices = [

			// front ones (change to clockwise)
			new THREE.Vector3(0.5, 3.46, 1), 	//index 0
			new THREE.Vector3(0.5, 3.46, 3), 	//index 1
			new THREE.Vector3(0.5, 1.73, 4),	//index 2
			new THREE.Vector3(0.5, 0, 3), 		//index 3
			new THREE.Vector3(0.5, 0, 1), 		//index 4
			new THREE.Vector3(0.5, 1.73, 0),	//index 5

			//back ones
			new THREE.Vector3(0, 3.46, 1),	//index 6
			new THREE.Vector3(0, 3.46, 3),	//index 7
			new THREE.Vector3(0, 1.73, 4),	//index 8
			new THREE.Vector3(0, 0, 3),		//index 9
			new THREE.Vector3(0, 0, 1),		//index 10
			new THREE.Vector3(0, 1.73, 0),	//index 11

			//center vertexes
			new THREE.Vector3(0.25, 3.46, 1.97),	//index 12
			new THREE.Vector3(0.25, 2.59, 3.5),		//index 13
			new THREE.Vector3(0.25, 1, 3.5),		//index 14
			new THREE.Vector3(0.25, 0, 2),			//index 15
			new THREE.Vector3(0.25, 1, 0.5),		//index 16
			new THREE.Vector3(0.25, 2.58, 0.49),	//index 17
			new THREE.Vector3(0.5, 1.73, 1.98), 	//front index 18
			new THREE.Vector3(0, 1.73, 1.98)		//back index 19

		];

		// The faces
		// Array of faces.
		// The array of faces describe how each vertex in the model is connected to form faces.
		// Additionally it holds information about face and vertex normals and colors
		wheel_geometry.faces = [

			//front face
			new THREE.Face3(5, 18, 0),
			new THREE.Face3(4, 18, 5),
			new THREE.Face3(3, 18, 4),
			new THREE.Face3(2, 18, 3),
			new THREE.Face3(1, 18, 2),
			new THREE.Face3(0, 18, 1),

			//back face
			new THREE.Face3(6, 19, 11),
			new THREE.Face3(11, 19, 10),
			new THREE.Face3(10, 19, 9),
			new THREE.Face3(9, 19, 8),
			new THREE.Face3(8, 19, 7),
			new THREE.Face3(7, 19, 6),

			//face 12
			new THREE.Face3(6, 12, 0),
			new THREE.Face3(0, 12, 1),
			new THREE.Face3(1, 12, 7),
			new THREE.Face3(7, 12, 6),

			//face 13
			new THREE.Face3(7, 13, 1),
			new THREE.Face3(1, 13, 2),
			new THREE.Face3(2, 13, 8),
			new THREE.Face3(8, 13, 7),


			//face 14
			new THREE.Face3(8, 14, 2),
			new THREE.Face3(2, 14, 3),
			new THREE.Face3(3, 14, 9),
			new THREE.Face3(9, 14, 8),


			//face 15
			new THREE.Face3(9, 15, 3),
			new THREE.Face3(3, 15, 4),
			new THREE.Face3(4, 15, 10),
			new THREE.Face3(10, 15, 9),

			//face 16
			new THREE.Face3(5, 16, 11),
			new THREE.Face3(11, 16, 10),
			new THREE.Face3(10, 16, 4),
			new THREE.Face3(4, 16, 5),

			//face 17
			new THREE.Face3(0, 17, 6),
			new THREE.Face3(6, 17, 11),
			new THREE.Face3(11, 17, 5),
			new THREE.Face3(5, 17, 0)

		];

		// Compute face normals
		wheel_geometry.computeFaceNormals();
		wheel_geometry.computeVertexNormals();

		// Create the mesh for the body.
		var wheel = new THREE.Mesh(wheel_geometry, wheel_material);
		wheel.name = "wheel";

		wheel.userData = { specular: wheel_material.specular,
                           shininess: wheel_material.shininess };

		this.car_object.add(wheel);
		wheel.position.set(x, y, z);
		wheel.rotateY(Math.PI/2);
	}
	/*----------------------------------------------------------------------------------------------------------*/

    createFollowingCamera(x, y, z) {

        var aspect = window.innerWidth / window.innerHeight;

        var followingCamera = new THREE.PerspectiveCamera( 60, aspect, 1, 1000 );

		followingCamera.name = "camera";


        this.car_object.add(followingCamera);

        followingCamera.position.set(x, y, z);

        followingCamera.lookAt(this.car_object.position);

    }
    /*---------------------------------------------------------------------------------------------------------*/

    makeBounding() {

        'use strict';

        var chassis_width = 13;

        this.bounding = new THREE.Sphere(this.car_object.position, chassis_width*0.2/2);
    }

    /*---------------------------------------------------------------------------------------------------------*/

    /*****************************************Moving the car***************************************************/

	move(delta) {

		'use strict';

		var distance = this.getDisplacement(delta);

		if (this.car_object.userData.movingDirection[1]) { // Up arrow

			this.moveForward(distance, delta);

			if (this.getSpeed() !== 0) {

				this.checkDirection(0);
			}
		}

		else if (this.car_object.userData.movingDirection[3]) { // Down arrow

			this.moveBackwards(distance, delta);

			if (this.getSpeed() !== 0) {

				this.checkDirection(1);
			}
		}

		else { // If no button is pressed

			this.car_object.translateX(distance);

			if (this.getSpeed() !== 0) {

				this.slowDown(delta);
			}
		}

        this.bounding.center = this.car_object.position;
    }

	/*---------------------------------------------------------------------------------------------------------*/

 	moveForward(distance, delta) {

		'use strict';

		this.car_object.translateX(distance); // Move forward

		if (this.getSpeed() < this.getMaxSpeed()) { // If speed is not max, accelerate until it's max

			this.setSpeed(this.getSpeed() + this.getAcceleration() * delta);
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	checkDirection(backward) {

		'use strict';

		if (this.car_object.userData.movingDirection[0]) {  // Left arrow

			if (backward) {

				this.car_object.rotateY(-0.05);
            }

            else {

                this.car_object.rotateY(0.05);
            }
		}

		else if (this.car_object.userData.movingDirection[2]) { // Right arrow

            if (backward) {

                this.car_object.rotateY(0.05);
            }

            else {

                this.car_object.rotateY(-0.05);
            }
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	moveBackwards(distance, delta){

		'use strict';

		this.car_object.translateX(distance); // Do the same as up arrow, but in the opposite direction

		if (this.getSpeed() > -this.getMaxSpeed()) {

			this.setSpeed(this.getSpeed() - this.getAcceleration() * delta);
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	slowDown(delta) {

		'use strict';

		if (this.getSpeed() > 0) { // If the car is moving forward

			this.setSpeed(this.getSpeed() - this.getAcceleration() * delta); // Start slowing down

			if (this.getSpeed() < 0) { // When it reaches negative speed, stop the car

				this.setSpeed(0);
		 	}
		}

		else if (this.getSpeed() < 0) { // If the car is moving backward

			this.setSpeed(this.getSpeed() + this.getAcceleration() * delta); // Start slowing down in the opposite direction

			if (this.getSpeed() > 0) { // When it reaches positive speed, stop the car

				this.setSpeed(0);
			}
		}

		this.checkDirection();
	}

	/*-------------------------------------------------------------------------------------------*/

	/********************************Getters e setters*******************************************/
	getSpeed() {

		'use strict';

		return this.car_object.userData.speed;
	}

	/*-------------------------------------------------------------------------------------------*/
	setSpeed(speed) {

		'use strict';

		this.car_object.userData.speed = speed;
	}

	/*-------------------------------------------------------------------------------------------*/

	getMaxSpeed() {

		'use strict';

		return this.car_object.userData.maxSpeed;
	}

	/*-------------------------------------------------------------------------------------------*/

	getAcceleration() {

		'use strict';

		return this.car_object.userData.acceleration;
	}

	/*------------------------------------------------------------------------------------------------*/

	getDisplacement(delta) {

		'use strict';

		 // speed = distance / time <=> distance = speed * time
        return this.getSpeed() * delta;
	}

	getCamera() {

	    return this.car_object.getObjectByName("camera");
    }

	/*------------------------------------------------------------------------------------------------*/

	setMovingState(state) {

		'use strict';

		this.car_object.userData.moving = state;
	}

	/*-------------------------------------------------------------------------------------------------*/

	setMovingDirection(index, state) {

		'use strict';


		/* left = 0
		   up = 1
		   right = 2
		   down = 3
		*/

		this.car_object.userData.movingDirection[index] = state;
	}
}
	/*---------------------------------------------------------------------------------------------------------*/
