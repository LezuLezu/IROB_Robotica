//vars
wheelThickness = 7.5;
barDiameter = 5;
barLenght = 50;
// alterable 
wheelRadius = 100;
barAmount = 30;

barSpacing = wheelRadius / barAmount;

module wheel(){
//    make wheel bases
    difference(){
        cylinder(wheelThickness, wheelRadius, wheelRadius, center=true);
        cylinder(wheelThickness, (wheelRadius - barDiameter), (wheelRadius - barDiameter));
    }
    translate([0, 0, barLenght])
        difference(){
        cylinder(wheelThickness, wheelRadius, wheelRadius);
        cylinder(wheelThickness, (wheelRadius - barDiameter), (wheelRadius - barDiameter));
    }
    
//    add wheel bars
    union(){
     //making holders
        make_ring_of(wheelRadius, barAmount){ 
            cylinder(barLenght,d=barDiameter);
        }        
    }
}

rotate([90, 0, 0]){
    wheel();
}


module make_ring_of(wheelRadius, barAmount){
    for (a = [0 : barAmount - 1]) {
        angle = a * 360 / barAmount;
        translate((wheelRadius - barDiameter/2) * [cos(angle), -sin(angle), 0])
                children();
    }
}