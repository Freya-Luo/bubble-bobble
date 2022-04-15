#include "enemy.h"
#include <iostream>
#include "simplebitmap.h"

bool moving_test_1(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");
    temp.LoadPng("../../final_project/png/level1.png");
	black_pixel = temp.CutOut(100, 100, 1, 1);

     //enemy position lower than player
    Enemy e1;
    e1.x = 200;
    e1.y = 400;
    e1.dir = 0;
    e1.j_cnt = 0;
    e1.f_cnt = 0;
    e1.live = true;
    e1.falling = false;
    float player_y = 200.0f;
    bool player_moving = false;

    //landing before able to jumps
    e1.moving(map, player_y, player_moving);

    return e1.j_cnt == 0 || e1.f_cnt == 0; //f and j count should not stay 0

}

bool moving_test_2(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");
    temp.LoadPng("../../final_project/png/level1.png");
	black_pixel = temp.CutOut(100, 100, 1, 1);

     //enemy position lower than player but already a fruit
    Enemy e2;
    e2.x = 200;
    e2.y = 400;
    e2.dir = 0;
    e2.j_cnt = 0;
    e2.f_cnt = 0;
    e2.live = false;
    e2.falling = false;
    float player_y = 200.0f;
    bool player_moving = false;

    e2.moving(map, player_y, player_moving);

    return e2.j_cnt != 0 || e2.f_cnt != 0;
}

bool moving_test_3(){
    SimpleBitmap map;
    map.LoadPng("../../final_project/png/level1.png");
    temp.LoadPng("../../final_project/png/level1.png");
	black_pixel = temp.CutOut(100, 100, 1, 1);

     //player still in progress of jumping or falling
    Enemy e3;
    e3.x = 200;
    e3.y = 400;
    e3.dir = 0;
    e3.j_cnt = 0;
    e3.f_cnt = 0;
    e3.live = true;
    e3.falling = false;
    float player_y = 200.0f;
    bool player_moving = true;

    e3.moving(map, player_y, player_moving);

    return e3.j_cnt != 0 || e3.f_cnt != 0;
}

int main(int argc,char *argv[]){

   return moving_test_1() || moving_test_2() || moving_test_3();

}