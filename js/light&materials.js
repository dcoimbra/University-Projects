/******************************************Iluminacao**************************************/

//Iluminacao global (direccional)
class Sun {

    constructor(color, intensity) {

        this.sun_object = new THREE.DirectionalLight( color, intensity );

        scene.add(this.sun_object);
    }

    flick() {

        this.sun_object.visible = !this.sun_object.visible;
    }
}

//Iluminacao pontual
class Candle {

    constructor(x, y, z) {

        this.candle_object = new THREE.PointLight( 0xffffff, 7, 250, 2);

        this.candle_object.position.set(x, y, z);

        scene.add(this.candle_object);

        var pointLightHelper = new THREE.PointLightHelper(this.candle_object, 1);
        scene.add( pointLightHelper );
    }

    flick() {

        this.candle_object.visible = !this.candle_object.visible;
    }
}