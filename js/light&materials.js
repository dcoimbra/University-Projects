/******************************************Iluminacao**************************************/

//Iluminacao global (direccional)
class Sun {

    constructor(color, intensity) {

        'use strict';
                                                        //intensity : [0, 1]
        this.sun_object = new THREE.DirectionalLight( color, intensity );

        scene.add(this.sun_object);
    }

    flick() {

        'use strict';

        this.sun_object.visible = !this.sun_object.visible;
    }
}

//Iluminacao pontual
class Candle {

    constructor(x, y, z) {

        'use strict';
                                                    /* cor, intensidade, alcance, decay */
        this.candle_object = new THREE.PointLight( 0xffcc66, 5, 250, 2);

        this.candle_object.position.set(x, y, z);

        scene.add(this.candle_object);


        this.flame = new THREE.PointLightHelper(this.candle_object, 0.5);
        scene.add(this.flame);
    }

    flick() {

        'use strict';

        this.candle_object.visible = !this.candle_object.visible;
        this.flame.visible = !this.flame.visible;
    }
}

function flickCandles() {

    for (var i = 0; i < candles.length; i++) {

        candles[i].flick();
    }
}

function toggleLighting() {

    var table_elements = table.table_object.children;
    var car_elements = car.inner_object.car_object.children;
    var toruses = border_lines[0].userData.toruses.concat(border_lines[1].userData.toruses);

    var car_child;

    if (lighting_on) {

        lighting_on = false;

        table_elements[0].material = new THREE.MeshBasicMaterial( { color: table_elements[0].material.color,
                                                                    wireframe: table_elements[0].material.wireframe} );

        for (car_child = 0; car_child < car_elements.length - 1; car_child++) {

            car_elements[car_child].material = new THREE.MeshBasicMaterial( { color: car_elements[car_child].material.color,
                                                                              wireframe: car_elements[car_child].material.wireframe } );
        }

        for (var i = 0; i < oranges.length; i++) {

            oranges[i].inner_object.orange_object.material = new THREE.MeshBasicMaterial( { color: oranges[i].inner_object.orange_object.material.color,
                                                                                            wireframe: oranges[i].inner_object.orange_object.material.wireframe,
                                                                                            opacity : 0,
                                                                                            transparent: oranges[i].inner_object.orange_object.material.transparent } );

            var stalk = oranges[i].inner_object.getStalk();

            stalk.material =  new THREE.MeshBasicMaterial( { color: stalk.material.color,
                                                             wireframe: stalk.material.wireframe,
                                                             opacity : 0,
                                                             transparent: stalk.material.transparent } );
        }

        for (var j = 0; j < butter_packages.length; j++) {

            butter_packages[j].inner_object.butterPackage_object.material = new THREE.MeshBasicMaterial( { color: butter_packages[j].inner_object.butterPackage_object.material.color,
                                                                                                           wireframe: butter_packages[j].inner_object.butterPackage_object.material.wireframe } );
        }

        for (var k = 0; k < toruses.length; k++) {

            toruses[k].inner_object.torus_object.material = new THREE.MeshBasicMaterial( { color: toruses[k].inner_object.torus_object.material.color,
                                                                                           wireframe: toruses[k].inner_object.torus_object.material.wireframe } );
        }
    }

    else {

        lighting_on = true;

        table_elements[0].material = new THREE.MeshPhongMaterial({ color: table_elements[0].material.color,
                                                                   wireframe: table_elements[0].material.wireframe});

        for (car_child = 0; car_child < car_elements.length - 1; car_child++) {

            car_elements[car_child].material = new THREE.MeshPhongMaterial( { color: car_elements[car_child].material.color,
                                                                              wireframe: car_elements[car_child].material.wireframe } );
        }

        for (var i = 0; i < oranges.length; i++) {

            oranges[i].inner_object.orange_object.material = new THREE.MeshPhongMaterial( { color: oranges[i].inner_object.orange_object.material.color,
                                                                                            wireframe: oranges[i].inner_object.orange_object.material.wireframe,
                                                                                            opacity : 0,
                                                                                            transparent: oranges[i].inner_object.orange_object.material.transparent } );

            var stalk = oranges[i].inner_object.getStalk();

            stalk.material =  new THREE.MeshPhongMaterial( { color: stalk.material.color,
                                                             wireframe: stalk.material.wireframe,
                                                             opacity : 0,
                                                             transparent: stalk.material.transparent } );
        }

        for (var j = 0; j < butter_packages.length; j++) {

            butter_packages[j].inner_object.butterPackage_object.material =  new THREE.MeshPhongMaterial( { color: butter_packages[j].inner_object.butterPackage_object.material.color,
                                                                                                            wireframe: butter_packages[j].inner_object.butterPackage_object.material.wireframe } );
        }

        for (var k = 0; k < toruses.length; k++) {

            toruses[k].inner_object.torus_object.material = new THREE.MeshPhongMaterial( { color: toruses[k].inner_object.torus_object.material.color,
                                                                                           wireframe: toruses[k].inner_object.torus_object.material.wireframe } );
        }
    }
}