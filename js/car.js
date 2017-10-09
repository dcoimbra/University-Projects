/******************************************Criaçao do carro**************************************/

// Classe carro
class Car {

	constructor(x, y, z) {

		'use strict';

		this.car_object = new THREE.Object3D();

		this.car_object.userData = { speed: 0,
					 			     maxSpeed: 40,
					 			     acceleration: 20,
					 			     moving: false,
					 		         movingDirection: [ false, false, false, false ] // [left, up, right, down]
							   	   };

		this.car_object.position.set(x, y, z);

		this.build(x, y, z);
	}

	/*---------------------------------------------------------------------------*/
	build(x, y, z) {

		'use strict';

		//Elements are instantiated relative to the car's position
		this.createChassis(x, y + 0.25, z);

		this.createHood(x - 4.5, y + 4.25, z);

		this.createWindShieldFront(x + 1.2, y + 3.7, z);
		this.createWindShieldSide(x - 2.2, y + 3.7, z - 3.3); // left
		this.createWindShieldSide(x - 2.2, y + 3.7, z + 3.3); // right

		this.createWheel(x - 6, y - 3.5, z + 6.2);
		this.createWheel(x + 6, y - 3.5, z + 6.2);
		this.createWheel(x - 6, y - 3.5, z - 6.2);
		this.createWheel(x + 6, y - 3.5, z - 6.2);

		scene.add(this.car_object);

		//Scale down size of car
		this.car_object.scale.multiplyScalar(0.2);
	}

	/*-------------------------------------------------------------------------------------------*/

	createChassis(x, y, z) {

		'use strict';

		/* Chassis: width: 20  (relative to camera)
		            height: 11
		            depth: 5
		*/

		var chassis_geometry = new THREE.BoxGeometry(20, 5, 11);
		var chassis_material = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true });
		var chassis = new THREE.Mesh(chassis_geometry, chassis_material);
		chassis.position.set(x, y, z);

		this.car_object.add(chassis);
	}

	/*-------------------------------------------------------------------------------------------*/

	createHood(x, y, z) {

		'use strict';

		/* Hood:    width: 11  (relative to camera)
		            height: 6
		            depth: 3
		*/

		var hood_geometry = new THREE.BoxGeometry(11, 3, 6);
		var hood_material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true });
		var hood = new THREE.Mesh(hood_geometry, hood_material);
		hood.position.set(x, y, z);

		this.car_object.add(hood);
	}

	/*-------------------------------------------------------------------------------------------*/

	createWindShieldFront(x, y, z) {

		'use strict';

		/* Widshield (front): width: 0  (relative to camera)
		                      height: 5.5
		                      depth: 2
		*/

		var windshield_frontGeometry = new THREE.BoxGeometry(0, 2, 5.5);
		var windshield_frontMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
		var windshield = new THREE.Mesh(windshield_frontGeometry, windshield_frontMaterial);
		windshield.position.set(x, y, z);

		this.car_object.add(windshield);
	}

	/*-------------------------------------------------------------------------------------------*/

	createWindShieldSide(x, y, z) {

		'use strict';

		/* Widshield (side):  width: 5.5  (relative to camera)
		                      height: 0
		                      depth: 2
		*/

		var windshield_sideGeometry = new THREE.BoxGeometry(5.5, 2, 0);
		var windshield_sideMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
		var windshield_side = new THREE.Mesh(windshield_sideGeometry, windshield_sideMaterial);
		windshield_side.position.set(x, y, z);

		this.car_object.add(windshield_side);
	}

	/*-------------------------------------------------------------------------------------------*/

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

		wheel.position.set(x, y, z);

		this.car_object.add(wheel);
	}
	/*-------------------------------------------------------------------------------------------*/


	/*****************************************Moving the car*************************************/

	move() {

		'use strict';

		var delta = clock.getDelta(); //Get the seconds passed since the last time it was called.
		var distance = this.getDisplacement(delta);
        var speed = this.getSpeed();

		if (this.car_object.userData.movingDirection[0]) {  // Left arrow

			this.moveLeft(distance, delta);
		}

		else if (this.car_object.userData.movingDirection[1]) { // Up arrow

			this.moveForward(distance, delta);
		}

		else if (this.car_object.userData.movingDirection[2]) { // Right arrow

			this.moveRight(distance, delta);
		}

		else if (this.car_object.userData.movingDirection[3]) { // Down arrow

			this.moveBackwards(distance, delta);
		}

		else { // If no button is pressed

			this.slowDown(distance, speed, delta);
		}
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

	moveLeft(distance, delta) {

		'use strict';

		if (this.isMoving()) {   // If the car is moving, keep it moving

			this.car_object.rotateY(0.1);	 // Turn left
			this.slowDown(distance, this.getSpeed(), delta);
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	moveRight(distance, delta){

		'use strict';

		if (this.isMoving()) { // If the car is moving, keep it moving

			this.car_object.rotateY(-0.1);	// Turn right
			this.slowDown(distance, this.getSpeed(), delta);
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	moveBackwards(distance, delta){

		'use strict';

		this.car_object.translateX(distance); // Do the same as up arrow, but in the opposite direction

		if (this.getSpeed() < this.getMaxSpeed()) {

			this.setSpeed(this.getSpeed() - this.getAcceleration() * delta);
		}
	}

	/*---------------------------------------------------------------------------------------------------------*/

	slowDown(distance, speed, delta) {

		'use strict';

		this.car_object.translateX(distance);

		if (this.getSpeed() > 0) { // If the car is moving forward

			this.setSpeed(this.getSpeed() - this.getAcceleration() * delta); // Start slowing down

			if (this.getSpeed() < 0) { // When it reaches negative speed, stop the car

				this.setSpeed(0);
				this.setMovingState(false);
		 	}
		}

		else if (this.getSpeed() < 0) { // If the car is moving backward

			this.setSpeed(this.getSpeed() + this.getAcceleration() * delta); // Start slowing down in the opposite direction

			if (this.getSpeed() > 0) { // When it reaches positive speed, stop the car

				this.setSpeed(0);
				this.setMovingState(false);
			}
		}
	}
/*-------------------------------------------------------------------------------------------*/

/********************************Getters e setters*******************************************/
	getSpeed() {

		'use strict';

		return this.car_object.userData.speed;
	}

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

	/*-------------------------------------------------------------------------------------------*/

	getDisplacement(delta) {

		'use strict';

		var distance = this.getSpeed() * delta; // speed = distance / time <=> distance = speed * time

		return distance;
	}

	/*-------------------------------------------------------------------------------------------*/

	isMoving() {

		'use strict';

		return this.car_object.userData.moving;
	}

	/*-------------------------------------------------------------------------------------------*/

	setMovingState(state) {

		'use strict';

		this.car_object.userData.moving = state;
	}

	/*-------------------------------------------------------------------------------------------*/

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
