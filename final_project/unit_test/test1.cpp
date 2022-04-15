#include "player.h"
#include <iostream>
#include "simplebitmap.h"


/*
* Test jumping
*/
bool moving_test_1(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");

    temp.LoadPng("../../final_project/png/level1.png");

	black_pixel = temp.CutOut(100, 100, 1, 1);

    Player p1;
    p1.x = 200;
    p1.y = 300;
    p1.dir = 0;
    p1.j_cnt = 0;
    p1.f_cnt = 80;

    //let p1 landing 
    for(int i = 0; i < 100; i++){
        p1.moving(map);
    }
    float landing_y = p1.y;
    
    //jump
    p1.j_cnt = 80;
    p1.f_cnt = 80;

    p1.moving(map);//the player should have smaller y than the landed y

    if(p1.y >= landing_y){
        std::cout << "1 wrong" <<std::endl;
        return 1;
    }else{
        return 0;
    }
    
}

bool moving_test_2(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");

    temp.LoadPng("../../final_project/png/level1.png");

	black_pixel = temp.CutOut(100, 100, 1, 1);

    Player p2;
    p2.x = 200;
    p2.y = 200;
    p2.dir = 0;
    p2.j_cnt = 80;
    p2.f_cnt = 80;

    p2.moving(map); // will jump, y will be smaller
    if(p2.y >= 200){
        std::cout << "2 wrong" <<std::endl;
        return 1;
    }else{
        return 0;
    }

    
}

bool moving_test_3(){
    
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");

    temp.LoadPng("../../final_project/png/level1.png");

	black_pixel = temp.CutOut(100, 100, 1, 1);
    
    Player p3;
    p3.x = 200;
    p3.y = 300;
    p3.dir = 0;
    p3.j_cnt = 0;
    p3.f_cnt = 80;

    p3.moving(map); //falling case, y will only be larger in value
    if(p3.y <= 300){
        std::cout << "3 wrong" <<std::endl;
        return 1;
    }else{
        return 0;
    }

    
}

bool check_room_test(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");

    temp.LoadPng("../../final_project/png/level1.png");

	black_pixel = temp.CutOut(100, 100, 1, 1);

    Player p4;
    p4.x = 200;
    p4.y = 300;
    p4.dir = 0;
    p4.j_cnt = 0;
    p4.f_cnt = 80;
    
    //should be no neighbor walls
    return p4.check_room(map) != 0;


}



int main(int argc,char *argv[]){

    return moving_test_1() || moving_test_2() || moving_test_3() || check_room_test();


}