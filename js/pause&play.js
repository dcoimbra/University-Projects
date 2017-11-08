function checkAnimation() {

    'use strict';

    paused = !paused; //pauses or unpauses game

    if (!paused) {  //if the game is not paused

        clock.start(); //Starts clock and sets the startTime and oldTime to the current time.
        animate(); //restart animation
    }
}