
//vars
wheelThickness = 7.5;
barDiameter = 5;
barLenght = 50;
// alterable 
wheelRadius = 100;
barAmount = 30;

// base
standWidth = wheelRadius*2;
standDepth = barLenght*1.5;
standHeight = 10;
// stand Bar
standBarLenght = wheelRadius * 1.5;
standBarDia = barDiameter;

// horizontal screw
scewLength = standDepth;
screwDia = barDiameter;

// plates to connect screw
plateWidth = barDiameter;
plateLength = wheelRadius * 1.8;
plateDepth = 2;





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
    translate([0, 0, wheelRadius])
        rotate([90, 0, 0])
            cylinder(scewLength, d=screwDia, center=true);
            
    // Plates to attach to screw and wheel
    translate([0, -(standDepth/2 - plateDepth*3), wheelRadius])
        color([1, 0, 0])cube([plateWidth, plateDepth, plateLength], center=true);
    translate([0, (standDepth/2 - plateDepth*3), wheelRadius])
        color([1, 0, 0])cube([plateWidth, plateDepth, plateLength], center=true);
    
}

stand();