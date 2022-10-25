//sphere(10);
//cube([20, 20, 20]);




hoogte = 15;

//module zandloper(het){
//    
//cylinder(r1=10, r2=5, h=het);
//translate([0, 0, 5])
//    cylinder(r1=5, r2=10, h=het);
//
//}
//translate([-20, 0, 0])  
//    zandloper(30);
//
//translate([20, 0, 0])  
//    zandloper(5);

module snaarwiel(doorsnede, het, gat){
    difference(){
        union(){        
            cylinder(r1=doorsnede/2+3, r2=doorsnede/2, h=het);
            translate([0, 0, het])
                cylinder(r1=doorsnede/2, r2=doorsnede/2+3, h=het);
        }
        cylinder(r=gat/2, h=hoogte*2);
    }

}
//snaarwiel(25, 5, 5, $fn=100);

//cylinder(r=10, h=8, $fn=8);

cube([20, 20,20], center=true);



