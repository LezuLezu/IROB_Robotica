//vars
//WHEEL
wheelThickness = 7.5;
barDiameter = 5;
barLength = 50;
wheelRadius = 100;
// alterable 
barAmount = 60;

barSpacing = wheelRadius / barAmount;


// base
standWidth = wheelRadius*2;
standDepth = barLength*1.5;
standHeight = 10;
// stand Bar
standBarLenght = wheelRadius * 1.5;
standBarDia = barDiameter;

// horizontal screw
scewLength = standDepth;
screwDia = barDiameter;

// plates to connect screw
plateWidth = barDiameter;
plateLength = (wheelRadius * 1.95) ;
plateDepth = 2;



module wheel(){
//    make wheel bases
    difference(){
        cylinder(wheelThickness, wheelRadius, wheelRadius, center=true);
        cylinder(wheelThickness, (wheelRadius - barDiameter), (wheelRadius - barDiameter), center=true);
    }
    translate([0, 0, barLength])
        difference(){
        cylinder(wheelThickness, wheelRadius, wheelRadius, center=true);
        cylinder(wheelThickness, (wheelRadius - barDiameter), (wheelRadius - barDiameter), center=true);
    }
    
//    add wheel bars
    union(){
     //making holders
        make_ring_of(wheelRadius, barAmount){ 
            cylinder(barLength,d=barDiameter);
        }        
    }
}
translate([0, (barLength/2), wheelRadius*1.1])
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

module stand(){
    // stand base
    cube([standWidth, standDepth, standHeight], center=true);
    
    // poles back
    translate([(standWidth/2 - standBarDia/2), (standDepth/2 - standBarDia/2), 0])
        rotate([0, -45, 0,])
            color([0, 0, 1])cylinder(h=standBarLenght, d=standBarDia, center=false);
    translate([-(standWidth/2 - standBarDia/2), (standDepth/2 - standBarDia/2), 0])
        rotate([0, 45, 0,])
            color([0, 1, 0])cylinder(h=standBarLenght, d=standBarDia, center=false);
    
    // poles front
        translate([(standWidth/2 - standBarDia/2), -(standDepth/2 - standBarDia/2), 0])
        rotate([0, -45, 0,])
            color([0, 0, 1])cylinder(h=standBarLenght, d=standBarDia, center=false);
    translate([-(standWidth/2 - standBarDia/2), -(standDepth/2 - standBarDia/2), 0])
        rotate([0, 45, 0,])
            color([0, 1, 0])cylinder(h=standBarLenght, d=standBarDia, center=false);
    
    // horizontal screw 
    translate([0, 0, wheelRadius-screwDia/2])
        rotate([90, 0, 0])
            cylinder(scewLength, d=screwDia, center=true);
            
    // Plates to attach to screw and wheel
    translate([0, barLength/2, wheelRadius+wheelThickness])
        color([1, 0, 0])cube([plateWidth, plateDepth, plateLength], center=true);
    translate([0, -barLength/2, wheelRadius+wheelThickness])
        color([1, 0, 0])cube([plateWidth, plateDepth, plateLength], center=true);
    
}

stand();