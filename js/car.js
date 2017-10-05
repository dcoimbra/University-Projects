/******************************************Criaçao do carro**************************************/

function createCar(x, y, z) {

	'use strict';

	car = new THREE.Object3D();

	car.userData = { speed: 0, 
					 maxSpeed: 40, 
					 acceleration: 20, 
					 isMoving: false, 
					 movingDirection: [ false, false, false, false ] 
					};

	createChassis(0, 0.25, 0);

	createHood(-4.5, 4.25, 0);

	createWindShieldFront(1.2, 3.7, 0);
	createWindShieldSide(-2.2, 3.7, -3.3); // left
	createWindShieldSide(-2.2, 3.7, 3.3); // right

	createWheel(-6, -3.5, 6.2);
	createWheel(6, -3.5, 6.2);
	createWheel(-6, -3.5, -6.2);
	createWheel(6, -3.5, -6.2);

	scene.add(car);

	car.position.set(x, y, z);

	car.scale.multiplyScalar(0.35);
}

/*-------------------------------------------------------------------------------------------*/
function createChassis(x, y, z) {

	'use strict';

	var chassis_geometry = new THREE.CubeGeometry(20, 5, 11);
	var chassis_material = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true });
	var chassis = new THREE.Mesh(chassis_geometry, chassis_material);
	chassis.position.set(x, y, z);

	car.add(chassis);
}

/*------------------------------------------------------------------------------------------*/
function createHood(x, y, z) {

	'use strict';

	var hood_geometry = new THREE.CubeGeometry(11, 3, 6);
	var hood_material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true });
	var hood = new THREE.Mesh(hood_geometry, hood_material);
	hood.position.set(x, y, z);

	car.add(hood);
}
/*-----------------------------------------------------------------------------------------------------*/

function createWindShieldFront(x, y, z) {

	'use strict';

	var windshield_frontGeometry = new THREE.CubeGeometry(0, 2, 5.5);
	var windshield_frontMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
	var windshield = new THREE.Mesh(windshield_frontGeometry, windshield_frontMaterial);
	windshield.position.set(x, y, z);

	car.add(windshield);
}
/*----------------------------------------------------------------------------------------------------*/

function createWindShieldSide(x, y, z) {

	'use strict';

	var windshield_sideGeometry = new THREE.CubeGeometry(5.5, 2, 0);
	var windshield_sideMaterial = new THREE.MeshBasicMaterial({ color: 0xffffff, wireframe: true });
	var windshield_side = new THREE.Mesh(windshield_sideGeometry, windshield_sideMaterial);
	windshield_side.position.set(x, y, z);

	car.add(windshield_side);
}
/*-------------------------------------------------------------------------------------------------------------*/

function createWheel(x, y, z) {

	'use strict';

	var wheel_geometry = new THREE.TorusGeometry(2, 0.6, 8, 16, Math.PI * 2);
	var wheel_material = new THREE.MeshBasicMaterial( { color: 0x333333, wireframe: true } );
	var wheel = new THREE.Mesh(wheel_geometry, wheel_material);

	var me = new THREE.Vector3(x, y, 5);

	wheel.position.set(x, y, z);

	wheel.lookAt(me);

	car.add(wheel);
}
/*---------------------------------------------------------------------------------------------------*/

function moveCar() {

	var delta = clock.getDelta(); //Get the seconds passed since the last time it was called.
	var distance = car.userData.speed * delta; // speed = distance / time <=> distance = speed * time

	if (car.userData.movingDirection[0]) {  // Left arrow
		moveLeft(distance, delta);

	}

	else if (car.userData.movingDirection[1]) { // Up arrow
		moveForward(distance, delta);

	}

	else if (car.userData.movingDirection[2]) { // Right arrow
		moveRight(distance, delta);

	}

	else if (car.userData.movingDirection[3]) { // Down arrow
		moveBackwards(distance, delta);

	}

	else { // If no button is pressed

		if (car.userData.speed > 0) { // If the car is moving forward
			forwardSlowingDown(distance, delta);

		}

		else if (car.userData.speed < 0) { // If the car is moving backward
			backwardsSlowingDown(distance, delta);

		}

		else { // If the speed is zero
			car.userData.isMoving = false; // Stop the car
		}
	}
}

/*---------------------------------------------------------------------------------------------------------*/

function moveForward(distance, delta){

	'use strict';

	car.translateX(distance); // Move forward

		if (car.userData.speed < car.userData.maxSpeed) { // If speed is not max, accelerate until it's max

			car.userData.speed += car.userData.acceleration*delta;
		}
}

function moveLeft(distance, delta){

	'use strict';

	car.rotateY(0.1);  // Turn left

		if (car.userData.isMoving) {   // If the car is moving, keep it moving

			car.translateX(distance);
		}
}

function moveRight(distance, delta){

	'use strict';

	car.rotateY(-0.1); // Turn right

		if (car.userData.isMoving) { // If the car is moving, keep it moving

			car.translateX(distance);
		}
}

function moveBackwards(distance, delta){

	'use strict';

	car.translateX(distance); // Do the same as up arrow, but in the opposite direction

		if (car.userData.speed < car.userData.maxSpeed) {

			car.userData.speed -= car.userData.acceleration*delta;
		}
}

function forwardSlowingDown(distance, delta){

	'use strict';

	car.translateX(distance);

			car.userData.speed -= car.userData.acceleration*delta; // Start slowing down

			if (car.userData.speed < 0) { // When it reaches negative speed, stop the car

				car.userData.speed = 0;
				car.userData.isMoving = false;
			}
}

function backwardsSlowingDown(distance, delta){

	'use strict';

	car.translateX(distance);

			car.userData.speed += car.userData.acceleration*delta; // Start slowing down in the opposite direction

			if (car.userData.speed > 0) { // When it reaches positive speed, stop the car

				car.userData.speed = 0;
				car.userData.isMoving = false;
			}
}
