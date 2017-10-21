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

		this.build(x, y, z);
	}

	/*----------------------------------------------------------------------------------------------------------*/
	build(x, y, z) {

		'use strict';

		//Elements are instantiated relative to the car's position
		this.createChassis(0, 0.25, 0);

		this.createHood( -4.5, 4.25, 0);

		this.createWindShieldFront( +1.2,  +3.7, 0);
		this.createWindShieldSide( -2.2,  +3.7,  -3.3); // left
		this.createWindShieldSide( -2.2,  +3.7,  +3.3); // right

		this.createWheel( -6,  -3.5,  6.2); // back-left
		this.createWheel( 6, -3.5, 6.2); // front-left
		this.createWheel( -6,  -3.5, -6.2); // back-right
		this.createWheel( 6,  -3.5,  -6.2); // front-right

		this.makeBounding();

        this.createFollowingCamera(-80, 80, 0);

		scene.add(this.car_object);

		//Scale down size of car
		this.car_object.scale.multiplyScalar(0.2);
	}

	/*----------------------------------------------------------------------------------------------------------*/

	createChassis(x, y, z) {

		'use strict';

		/* Chassis: width: 20  (relative to camera)
		            height: 11
		            depth: 5
		*/

		var chassis_geometry = new THREE.BoxGeometry(20, 5, 11);
		var chassis_material = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true });
		var chassis = new THREE.Mesh(chassis_geometry, chassis_material);

		chassis.name = "chassis";

		this.car_object.add(chassis);
        chassis.position.set(x, y, z);
	}

	/*----------------------------------------------------------------------------------------------------------*/

	createHood(x, y, z) {

		'use strict';

		/* Hood:    width: 11  (relative to camera)
		            height: 6
		            depth: 3
		*/

		var hood_geometry = new THREE.BoxGeometry(11, 3, 6);
		var hood_material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true });
		var hood = new THREE.Mesh(hood_geometry, hood_material);

		this.car_object.add(hood);
        hood.position.set(x, y, z);
	}

	/*-----------------------------------------------------------------------------------------------------------*/

	createWindShieldFront(x, y, z) {

		'use strict';

		/* Windshield (front): width: 0  (relative to camera)
		                      height: 5.5
		                      depth: 2
		*/

		var windshield_frontGeometry = new THREE.BoxGeometry(0, 2, 5.5);
		var windshield_frontMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
		var windshield = new THREE.Mesh(windshield_frontGeometry, windshield_frontMaterial);

		this.car_object.add(windshield);
        windshield.position.set(x, y, z);
	}

	/*------------------------------------------------------------------------------------------------------*/

	createWindShieldSide(x, y, z) {

		'use strict';

		/* Widshield (side):  width: 5.5  (relative to camera)
		                      height: 0
		                      depth: 2
		*/

		var windshield_sideGeometry = new THREE.BoxGeometry(5.5, 2, 0);
		var windshield_sideMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
		var windshield_side = new THREE.Mesh(windshield_sideGeometry, windshield_sideMaterial);

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

		var wheel_geometry = new THREE.TorusGeometry(2, 0.6, 8, 16, Math.PI * 2);
		var wheel_material = new THREE.MeshBasicMaterial( { color: 0x333333, wireframe: true } );
		var wheel = new THREE.Mesh(wheel_geometry, wheel_material);

		this.car_object.add(wheel);
        wheel.position.set(x, y, z);
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

	/*****************************************Moving the car***************************************************/

	move(delta) {

		'use strict';

		var distance = this.getDisplacement(delta);

		if (this.car_object.userData.movingDirection[1]) { // Up arrow

			this.moveForward(distance, delta);

			if (this.getSpeed() !== 0) {

				this.checkDirection();
			}
		}

		else if (this.car_object.userData.movingDirection[3]) { // Down arrow

			this.moveBackwards(distance, delta);

			if (this.getSpeed() !== 0) {

				this.checkDirection();
			}
		}

		else { // If no button is pressed

			this.car_object.translateX(distance);

			if (this.getSpeed() !== 0) {

				this.slowDown(delta);
			}
		}

		this.bounding.center = this.car_object.getWorldPosition();
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

	checkDirection() {

		'use strict';

		if (this.car_object.userData.movingDirection[0]) {  // Left arrow

			this.car_object.rotateY(0.05);
		}

		else if (this.car_object.userData.movingDirection[2]) { // Right arrow

			this.car_object.rotateY(-0.05);
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

	makeBounding() {

		'use strict';

		var chassis = this.car_object.getObjectByName("chassis");

		var bounding = new THREE.Sphere(this.car_object.getWorldPosition(), chassis.geometry.parameters.width/2 + 3.5);

		this.bounding = bounding;
	}

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

		var distance = this.getSpeed() * delta; // speed = distance / time <=> distance = speed * time

		return distance;
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
