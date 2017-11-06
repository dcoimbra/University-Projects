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
        this.candle_object = new THREE.PointLight( 0xffffff, 3, 250, 1);

        this.candle_object.position.set(x, y, z);
        scene.add(this.candle_object);

        //Adicionado helper
        this.flame = new THREE.PointLightHelper(this.candle_object, 0.5);
        scene.add(this.flame);
    }

    /*-------------------------------------------------------------------------------------------------------------*/
    flick() {

        'use strict';

        this.candle_object.visible = !this.candle_object.visible;
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
                opacity: node.material.opacity
            };

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