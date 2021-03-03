//square([10,5], center=true);
base_w = 30;

union(){ //Base of pagoda
    cube([base_w,base_w,base_w/10],center=true);
    translate([0,0,base_w*0.7/10]){
       cube([base_w*0.8,base_w*0.8,base_w*0.9/10],center=true);
    }
}