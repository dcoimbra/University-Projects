/******************************************Iluminacao**************************************/

//Iluminacao global (direccional)
class GlobalLight {

    constructor(color, intensity) {

        this.light_object = new THREE.DirectionalLight( color, intensity );

        scene.add(this.light_object);
    }

    flick() {

        this.light_object.visible = !this.light_object.visible;
    }
}