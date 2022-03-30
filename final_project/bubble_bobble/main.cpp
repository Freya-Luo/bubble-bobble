#include <iostream>
#include "simplebitmap.h"
#include "fssimplewindow.h"
#include <vector>

SimpleBitmap temp;

class Enemy{
	public:
		float x, y;
		void moving();
};

class Bubble{
	public:
		float x, y;
		void moving();
};

class Player {       
  public:            
	float  x, y;
	bool jumped = false;
	bool falling = false;
	bool walk1= false;
	bool walk2 = false;
	int dir = 0; //0 is right, 1 is left;
	int j_cnt, f_cnt ;     

	void moving(SimpleBitmap bmp);
	SimpleBitmap drawing();
};

SimpleBitmap Player::drawing(){
    SimpleBitmap player_bmp;
	if(dir == 0){
		if(!(walk1 || walk2)){
			player_bmp.LoadPng("../final_project/png/Bubble.png");
		}else if(walk1){
			player_bmp.LoadPng("../final_project/png/Bubble_walk1.png");
		}else{
			player_bmp.LoadPng("../final_project/png/Bubble_walk2.png");
		}
	}else{
		player_bmp.LoadPng("../final_project/png/Bubble_left.png");
	}
	return player_bmp;
}

void Player::moving(SimpleBitmap bmp){
	if(j_cnt > 0){
		jumped = true;
		y -= 0.3;
		j_cnt -= 1;
	}
	if(f_cnt > 0 && j_cnt == 0){
		jumped = false;
		y += 0.3;
		f_cnt -= 1;
	}
	if(x > bmp.GetWidth()-20){
		x = bmp.GetWidth()-20;	
	}
	if(x < 20){
		x = 20;
	}
		
	if(y < 20){
		y = 20;	
	}

	SimpleBitmap black = temp.CutOut(100, 100, 1, 1);
	SimpleBitmap player_loc = bmp.CutOut(int(x)+16, int(y)+32, 1, 1);
	if(black.operator==(player_loc) && (!jumped)){
		y += 0.3;
		f_cnt = 0;
		falling = true;
	}else{
		falling = false;
	}
}

void run(){
	
    temp.LoadPng("../final_project/png/level1.png");

    SimpleBitmap bmp;
    bmp.LoadPng("../final_project/png/level1.png");
	FsOpenWindow(0,0,bmp.GetWidth(),bmp.GetHeight(),1);
	

	Player player;
	std::vector<Bubble> bubbles;
	std::vector<Bubble> enemies;
	player.x = 100;
	player.y = 440;
	player.j_cnt = 0;
	player.f_cnt = 0;

	for(;;)
	{

		FsPollDevice();
		auto key=FsInkey();

		if(FSKEY_ESC==key)
		{
			break;
		}

		if(FSKEY_LEFT==key)
		{
			player.x -= 10;
			player.dir = 1;
		}else if(FSKEY_RIGHT==key)
		{
			player.dir = 0;
			player.x += 10;
			if(player.walk1 == false){
				player.walk1 = true;
				player.walk2 = false;
			}else{
				player.walk2 = true;
				player.walk1 = false;
			}
		}

		if(FSKEY_UP==key)
		{
			if(player.j_cnt == 0 && player.f_cnt == 0 && (!player.falling)){
				player.j_cnt = 400;
				player.f_cnt = 400;
			}
		}
		if(FSKEY_SPACE==key)
		{
			Bubble bubble;
			bubble.x = player.x;
			bubble.y = player.y;
			bubbles.push_back(bubble);
		}


		player.moving(bmp);
		SimpleBitmap player_bmp = player.drawing();
		

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(0, 0);
		glDrawPixels(bmp.GetWidth(),bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmp.GetBitmapPointer());
		
		
		glRasterPos2i(int(player.x), int(player.y));
		glDrawPixels(player_bmp.GetWidth(),player_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,player_bmp.GetBitmapPointer());
		

		glPixelZoom(1.0,-1.0);
		FsSwapBuffers();
		
	}

}



int main(int argc,char *argv[]){
	std::cout << "Press left and right to move, up to jump" << std::endl;
    run();
    return 0;
}