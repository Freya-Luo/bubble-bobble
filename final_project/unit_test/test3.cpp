#include "bubble.h"
#include <iostream>
#include "simplebitmap.h"

bool direction_test(){
    Bubble b;
    b.dir = 0;
    for(int i = 0; i < 200; i++){
        b.moving();
        if(b.dir == 1){
            return true;
        }
    }
    return false;
}

bool dash_distance_test(){
    Bubble b;
    b.x = 100;
    b.y = 100;
    b.dir = 0;
    for(int i = 0; i < 200; i++){
        b.moving();
    }
    if(b.x != 176){
        return true;
    }else{
        return false;
    }
}

int main(int argc,char *argv[]){
    return direction_test() || dash_distance_test();
}