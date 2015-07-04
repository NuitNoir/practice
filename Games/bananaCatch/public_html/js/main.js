
monkey = new Monkey(500, 500);



function Monkey(top, left) {
//var Monkey = class {
    this.top = top;
    this.left = left;
    this.init = function(top, left) {
        this.top    = top;
        this.left   = left;

        ///// get monkey object and set initial place.
        monkey = $("#monkey");
        monkey.css("position", "absolute");
        monkey.css({top:this.top, left:this.left});

        ///// set key event to move left and right.
    }
    this.init(top, left);
    $(document).ready(function(){
        $("body").keypress(function(e){
            console.log(e.which);
            if (e.which == 39) {
//                monkey.left += 7;
                setInterval("moveLeft", 300);
            } else if (e.which == 37) {
                monkey.left -= 7;
            }
            
            $("#monkey").css({left: monkey.left});
        });
    });                
    this.moveLeft = function(e) {
        monkey.left += 7;
        $("#monkey").css({left: monkey.left});        
    }
}




