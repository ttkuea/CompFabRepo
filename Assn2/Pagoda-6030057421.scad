// Adjust base width
base_w = 30;

// Adjust level no.
lvl_no = 3;

module prism(l, w, h){
    polyhedron(
        points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
        faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]
    );
}

module pagoda_base(){
    b1_w = base_w;
    b2_w = base_w*0.7;
    b3_w = base_w*0.65;
    
    st_w = (b1_w - b3_w) / 2;
    st_l = b2_w * 0.25;
    st_h = b1_w *1.05 / 10;
    
    tx = [-1, 5.7, 1, -5.7];
    ty = [-1, -0.17, 1, 0.17];
    
    union(){
        //Base 1
        cube([base_w,base_w,base_w/10],center=true);
        
        //Base 2
        translate([0,0,base_w*0.7/10]){
            cube([b2_w,b2_w,base_w*0.8/10],center=true);
        }
        
        //Base 3
        translate([0,0,2* base_w*0.6/10]){
            cube([b3_w,b3_w,base_w*0.7/10],center=true);
        }
        
        //Stairs
        for (i = [0:3]) {
            translate([tx[i] * st_w/2, ty[i] * b1_w/2, b1_w/20]){
                rotate(a = 90  * i) prism(st_l,st_w,st_h);
            }
        }
    }
}

module pagoda_body(b_w){
    body_w = b_w * 0.6;
    body_h = body_w * 0.7; 
    
    roof_w = b_w * 0.95;
    thicc = roof_w*0.1;
    union(){
        
        difference(){
            cube([body_w, body_w, body_h],center = true);
            cube([body_w*1.1, body_w * 0.2, body_h * 0.6],center=true);
            cube([body_w*0.2, body_w * 1.1, body_h * 0.6],center=true);
        };
        
        
        translate([0,0,body_h*0.39]){
            thicc = thicc * 0.2;
            union(){
            for (i = [1:5]){
                translate([0,0, thicc*i]){
                    cube([roof_w * (1-(0.05*i)),roof_w * (1-(0.05*i)),thicc],center=true);
                }
            } 
            }
        }

    }  
}

module pagoda_top(b_w){
    body_w = b_w * 0.1;
    body_h = body_w * 0.1; 
    
    roof_w = b_w * 0.4;
    thicc = roof_w*0.1;
    translate([0,0,body_h*0.9]){
            thicc = thicc * 0.4;
            union(){
            for (i = [1:6]){
                translate([0,0, thicc*i]){
                    cube([roof_w * (1-(0.1*i)),roof_w * (1-(0.1*i)),thicc],center=true);
                }
            } 
            }
            translate([0,0,thicc*20]) cube([roof_w * 0.2,roof_w * 0.2,b_w*0.9],center=true);
    }
    
    
}

module pagoda_recur_body(b_w,b_h){
    body_w = b_w * 0.7;
    body_h = body_w * 0.4; 
    
    roof_w = b_w * 0.98;
    thicc = roof_w*0.1;
    union(){
        cube([body_w, body_w, body_h * 0.07],center=true);
        
        translate([0,0,body_h/2])
            cube([body_w * 0.8,body_w * 0.8, body_h], center=true);
        
        translate([0,0,body_h*0.9]){
            thicc = thicc * 0.2;
            union(){
            for (i = [1:5]){
                translate([0,0, thicc*i]){
                    cube([roof_w * (1-(0.1*i)),roof_w * (1-(0.1*i)),thicc],center=true);
                }
            } 
            }
        }
        
    }
}


// RECURSIVE
module pagoda_recur(lvl){
    recur_w = base_w * (1 - (0.05 * (lvl_no - lvl)));
    recur_h = base_w * 0.6;
    if (lvl) {
        pagoda_recur_body(recur_w);
        translate([0,0,recur_h* (0.5 - (0.01 * (lvl_no-lvl)))]) pagoda_recur(lvl-1);
    } else {
        translate([0,0,recur_h*(0.03 - (0.01 * lvl_no))])pagoda_top(recur_w);
    }
}


// MAIN
module pagodadadadada(){
    offset_body_base = (base_w *1.05 / 10) + (base_w/10);
    offset_recur_base = offset_body_base + (base_w*0.6*0.4);
    union(){
        pagoda_base();
    
        translate([0,0,offset_body_base]) pagoda_body(base_w);
    
        translate([0,0,offset_recur_base]) pagoda_recur(lvl_no);
    }
}


pagodadadadada();