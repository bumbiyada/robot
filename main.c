// i = x
// j = y
// k = params: 
// k = 0 val = 1 - clear, 2 - wall, 3 - plant, 4 - ready_for_plant, 5 - base
// k = 1 val = vawe iteration
// k = 2 val = 1/0 if was allready scanned
int points[201][201][3];

// i - for each scanned plant base
// j : 0 - is_scanned (0,1) 1 - x, 2 - y, 3 - is_planted
int plants_found;
int plants[1000][4];

// base params
int b_x, b_y;
// robot main params in space
int r_x, r_y, r_compas;

// debug var for debug messages in std.out
// set debug = TRUE to enable debug message
boolean debug;

int main {
    // init

    // config
   plants_found = 0;
   debug = true;


    // spiral
    spiral();
    // wave search algo each plants

    // wave search algo move to base

    return 1;
}

// move spiraly each iteration increase squere size by 1 until every squere will be checked
void spiral {
    int squereSize = 1;
    int scannedPoints = 0;
    while (true) {
        scannedPoints = scanSquere(squreSize);

        if (scannedPoints == 0) {
            break;
        } 
        squereSize = squereSize + 1;
    }
}


int scanSquere (int squereSize) {

    boolean is_wall_right_before;
    int squereWallsVisited;
    int scannedNewSectorsTotal;
    int scndSectors;
    // deside which way to go
    while (true)
    {
        // case to break from cycle
        if (squereWallsVisited > 4) {
            break;
        }
        // scan sectors
        scannedNewSectorsTotal = scannedNewSectorsTotal + scanSectorsAround();
        // if is_wall_right == false and is_wall_right_before = true -> mright() 
        if (is_wall_right_before & !справа_стена & is_right_sector_fits_size(squereSize)) {
            is_wall_right_before = false;
            mright();
            if (справа_стена) {
                is_wall_right_before = true;
            }
            continue;
        }
        if (справа_стена) {
                is_wall_right_before = true;
            }
        // if no wall forward and there is no limit by squere size -> mfwd()
        if (is_fwd_sector_fits_size(squereSize) & впереди_свободно) {
            mfwd();
            continue;
        }
        // else mleft()
        if (!is_fwd_sector_fits_size(squereSize)){
            squereWallsVisited = squereWallsVisited + 1;
            mleft();
            continue;
        }
        if (впереди_стена) {
            is_wall_right_before = true;
            mleft();
            continue;
        }
    }
    return scannedNewSectorsTotal;

}
// scan sectors around robot and current robot position
// add info to global array of items
// returns count of newly scanned sectors
int scanSectorsAround(){
    int scannedSectors;
    boolean isScanned;
    if (compas() % 360 == 0) {
        // N
        isScanned = scanSingleSector(0,r_x, r_y+1);
        if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(90,r_x+1, r_y);
        if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(180,r_x, r_y-1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(270,r_x-1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
    }
    if (compas() % 360 == 90) {
        // 
        isScanned = scanSingleSector(0,r_x+1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(90,r_x, r_y-1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(180,r_x-1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(270,r_x, r_y+1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
    }
    if (compas() % 360 == 180) {
        // S
        isScanned = scanSingleSector(0,r_x, r_y-1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(90,r_x-1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(180,r_x, r_y+1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(270,r_x+1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
    }
    if (compas() % 360 == 270) {
        //
        isScanned = scanSingleSector(0,r_x-1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(90,r_x, r_y+1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(180,r_x+1, r_y);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
        isScanned = scanSingleSector(270,r_x, r_y-1);
         if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
    }
    // scan current position
    isScanned = scanSingleSector(360, r_x, r_y);
     if (isScanned == false) {
            scannedSectors = scannedSectors + 1;
        }
    return scannedSectors;
}
// scan single sector in x,y coords in direction (0 fwd,90 right,180 back,270 left, 360 - current robot position )
// it automaticly converts x,y into array indixes
// returns false if it`s newly scanned sector
// return true if sector was allready scanned previously
boolean scanSingleSector(int direction, int x, int y) {
    int sectorInfo[3];
    sectorInfo = points[x+100][y+100];
    if (sectorInfo[2] == 0) {
        // it wasnt scanned prev, we will scan sector and save info to points array and return false
        if (direction == 0) {
            // front 
            if (впереди_стена) {
                sectorInfo[0] = 2;
                sectorInfo[2] = 1;
            }
        }
        if (direction == 90) {
            //right
             if (справа_стена) {
                sectorInfo[0] = 2;
                sectorInfo[2] = 1;
            }
        }
        if (direction == 180) {
            //back
             if (сзади_стена) {
                sectorInfo[0] = 2;
                sectorInfo[2] = 1;
            }
        }
        if (direction == 270) {
            //left
             if (слева_стена) {
                sectorInfo[0] = 2;
                sectorInfo[2] = 1;
            }
        } 
        if (direction == 360) {
            //curent pos
            if (база){
                sectorInfo[0] = 5; //base
                sectorInfo[2] = 1; 
                b_x = x;
                b_y = y;
            }
            if (грядка) {
                sectorInfo[0] = 4;
                sectorInfo[2] = 1;
                int pInfo[4] = plants[plants_found]
                pInfo[0] = 1; // scanned
                pInfo[1] = r_x; // x
                pInfo[2] = r_y; // y
                pInfo[3] = 0; // is planted
                plants[plants_found] = pInfo;
                plants_found = plants_found + 1;
            }
            if (!база & !грядка) {
                sectorInfo[0] = 1; //clear
                sectorInfo[2] = 1; 
            }
        } 
        points[x+100][y+100] = sectorInfo;
        return false;
    } else {
        // sector was allready scanned, we will not make changes into sector and return true
        return true;
    }
}
// scan forward sector
// return true if forward sector is in squere size
// return false if robot fwd move goes out of the squere size
// EX: if size == 1, robot allready set on x=1 y=1 and want`s to move to x=2 y=1, the target
// goes out of squere size and function will return false 
boolean is_fwd_sector_fits_size(int size) {
    if (compas() % 360 == 0 & r_y < size) {
        return true;
    }
    if (compas() % 360 == 90 & r_x < size) {
        return true;
    }
    if (compas() % 360 == 180 & r_y > -size) {
        return true;
    }
    if (compas() % 360 == 270 & r_x > -size) {
        return true;
    }
    return false;
}

boolean is_right_sector_fits_size(int size) {
    if (compas() % 360 == 0 & r_x < size) {
        return true;
    }
    if (compas() % 360 == 90 & r_y > -size) {
        return true;
    }
    if (compas() % 360 == 180 & r_x > -size) {
        return true;
    }
    if (compas() % 360 == 270 & r_y < size) {
        return true;
    }
    return false;
}


// calibrate compas
int compas {
    if (r_compas < 0) {
        return r_compas + 360;
    }
    if (r_compas > 360) {
        return r_compas - 360;
    }
    return r_compas;
}


// move forward
void mfwd{
    if (debug == true) {
        println("*forward");
    } 
    вперед;
    if (compas() % 360 == 0) {
        offsetRXY(0,1);
    }
    if (compas() % 360 == 90) {
        offsetRXY(1,0);
    }
    if (compas() % 360 == 180) {
        offsetRXY(0,-1);
    }
    if (compas() % 360 == 270) {
        offsetRXY(-1,0);
    }
}
// move left
void mlft{
    left();
    mfwd();
}
// move right
void mright{
    right();
    mfwd();
}
// tirn left
void left{
    if (debug == true) {
        println("*tirn left");
    } 
    налево;
    r_compas = r_compas - 90;
}
// tirn right
void right{
    if (debug == true) {
        println("*tirn right");
    } 
    направо;
    r_compas = r_compas + 90;
}
// set robot offset x,y
void offsetRXY(int x, int y) {
    r_x = r_x + x;
    r_y = r_y + y;
    if (debug == true) {
        cout << "*offset robot x=" << r_x << " y=" << r_y; println("");
    } 
}