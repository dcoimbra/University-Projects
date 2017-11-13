/******************************************Iluminacao**************************************/

//Iluminacao global (direccional)
class Sun {

    constructor(color, intensity) {

        'use strict';

        /* A light that gets emitted in a specific direction.
        This light will behave as though it is infinitely far away and the rays produced from it are all parallel.
        The common use case for this is to simulate daylight; */

        //color,
        //intensity: numeric value of the light's strength/intensity. Minimum is 0 and maximum is 1.

        this.sun_light = new THREE.DirectionalLight( color, intensity );
        scene.add(this.sun_light);
    }

    flick() {

        'use strict';

        this.sun_light.visible = !this.sun_light.visible;
    }
}
/*********************************************************************************************************************/
//Iluminacao pontual
class Candle {

    constructor(x, y, z) {

        'use strict';

        //A light that gets emitted from a single point in all directions.
        // A common use case for this is to replicate the light emitted from a bare lightbulb.
        // color,
        // intensity: numeric value of the light's strength/intensity. Default is 1.
        // distance: The distance from the light where the intensity is 0. When set to 0, then the light never stops.
        // decay: The amount the light dims along the distance of the light.
        this.candle_light = new THREE.PointLight( 0xffffff, 1, 90, 1);
        this.candle_light.position.set(x, y, z);
        scene.add(this.candle_light);


        /* candle stick*/
        var candle_geometry = new THREE.CylinderGeometry( 1, 1, y-0.5, 16 );
        var candle_material = new THREE.MeshPhongMaterial({ color: 0xaaaaaa,
                                                          specular: 0x888888,
                                                          shininess: 10,
                                                          wireframe: true });

        this.candle_object = new THREE.Mesh(candle_geometry, candle_material);

        this.candle_object.userData = { specular: candle_material.specular,
                                        shininess: candle_material.shininess };

        this.candle_object.position.set(x, 4, z);

        scene.add(this.candle_object);
        

        /*candle flame*/
        this.flame = new THREE.PointLightHelper(this.candle_light, 0.5, 0xe29822);
        scene.add(this.flame);
    }

    /*-------------------------------------------------------------------------------------------------------------*/
    flick() {

        'use strict';

        this.candle_light.visible = !this.candle_light.visible;
        this.flame.visible = !this.flame.visible;
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
function flickCandles() {

    'use strict';

    for (var i = 0; i < candles.length; i++) {

        candles[i].flick();
    }
}

/*----------------------------------------------------------------------------------------------------------------*/
function toggleShading(node) {

    'use strict';

    if (lighting_on) {

        if (node instanceof THREE.Mesh && !(node instanceof THREE.PointLightHelper) && !(node instanceof THREE.Line)) {

            var new_material;

            var parameters = {
                color: node.material.color,
                wireframe: node.material.wireframe,
                transparent: node.material.transparent,
                opacity: node.material.opacity,
            };

            if (node.name === "table" || node.name === "orange" || node.name === "butterp") {

                parameters.map = node.userData.map;
            }

            if (phong_shading) {

                parameters.specular = node.userData.specular;
                parameters.shininess = node.userData.shininess;
                new_material = new THREE.MeshPhongMaterial(parameters);

            }

            else {

                new_material = new THREE.MeshLambertMaterial(parameters);
            }

            node.material = new_material;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------*/
function toggleLighting(node) {

    'use strict';

    if (node instanceof THREE.Mesh && !(node instanceof THREE.PointLightHelper) && !(node instanceof THREE.Line)) {

        var new_material;

        var parameters = {
            color: node.material.color,
            wireframe: node.material.wireframe,
            transparent: node.material.transparent,
            opacity: node.material.opacity
        };

        if (node.material instanceof THREE.MeshPhongMaterial || node.material instanceof THREE.MeshLambertMaterial) {

            /* se o material é sombreado, cria um básico com os mesmos parametros */

            new_material = new THREE.MeshBasicMaterial(parameters);

            node.material = new_material;
        }

        else if (node.material instanceof THREE.MeshBasicMaterial) {

            /*se o material é básico, determina qual o sombreado que deve criar */

            toggleShading(node);
        }
    }
}
/**********************************************************************************************************************/