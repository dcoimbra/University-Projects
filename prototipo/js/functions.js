function goBack() {
    
    window.history.back();
}

                        
function move() {
                var elem = document.getElementById("myBar");
                var width = 0;
                var id = setInterval(frame, 10);
                function frame() {
                    if (width == 100) {
                        clearInterval(id);
                        window.location="pessoal.html";
                    } else {
                        width++;
                        elem.style.width = width + '%';
                    }
                }
            }