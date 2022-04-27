#include <iostream>
#include "simplebitmap.h"
#include "fssimplewindow.h"
#include <vector>
#include "bubble.h"
#include "enemy.h"
#include <algorithm>    // std::min
#include "player.h"
#include "mmlplayer.h"
#include "yssimplesound.h"

SimpleBitmap bubble_bmp;

float born_x = 100;
float born_y = 400;

int life = 3;

YsSoundPlayer mmlplayer;
YsSoundPlayer::SoundData nextWave;
YsSoundPlayer::Stream waveStream;
MMLSegmentPlayer mml;

bool check_intersect(float e_x, float e_y, float b_x, float b_y){
	if((b_x + 32 > e_x) && (b_x < e_x + 32) && ((b_y+32 > e_y) && (b_y < e_y+32))){
		return true;
	}
	return false;
}

bool check_collision(Player *player,  std::vector<Enemy> *enemies, int *type){
	for(int i = 0; i < enemies->size(); i++){
			float e_x = enemies->at(i).x;
			float e_y = enemies->at(i).y;
			bool reached = check_intersect(e_x, e_y, player->x, player->y);
		if(reached){
			if(enemies->at(i).live){
				*type = 1;
				return true;
			}else{
				enemies->erase(enemies->begin() + i);
				*type = 2;
				return true;
			}
		}
		
	}
	return false;
}

void check_hit(	std::vector<Bubble> *bubbles, std::vector<Enemy> *enemies){
	for(int i = 0; i < bubbles->size(); i++){
		if(bubbles->at(i).dash_cnt <= 0){
			continue;
		}
		if(bubbles->at(i).y <= 0){
			bubbles->erase(bubbles->begin() + i);
			continue;
		}
		for(int j = 0; j < enemies->size(); j++){
			if(!enemies->at(j).live){
				continue;
			}
			float e_x = enemies->at(j).x;
			float e_y = enemies->at(j).y;
			float b_x = bubbles->at(i).x;
			float b_y = bubbles->at(i).y;
			if(check_intersect(e_x, e_y, b_x, b_y)){
				enemies->at(j).live = false;
				bubbles->erase(bubbles->begin() + i);
				break;
			}
		}
	}
}

void need_terminate(){
	if(life == 0){
		std::cout << "All 3 life used! Failed!" << std::endl;
		exit(0);
	}
}

void assign_monster_loc(int level, std::vector<Bubble> *bubbles, std::vector<Enemy> *enemies){
	int en_cnt = 3;
	if(level == 1){
		for (int i =0; i<en_cnt; i++){
			Enemy en(300, i*150 + 100);
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			// en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			// en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			// en.fruit.LoadPng("../final_project/png/banana.png");
			// en.x = 300;
			// en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}else if(level == 2){
		for (int i =0; i<en_cnt; i++){
			Enemy en(400, i*150 + 100);
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			// en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			// en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			// en.fruit.LoadPng("../final_project/png/banana.png");
			// en.x = 400;
			// en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}else{
		for (int i =0; i<en_cnt; i++){
			Enemy en(200, i*150 + 100);
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			// en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			// en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			// en.fruit.LoadPng("../final_project/png/banana.png");
			// en.x = 200;
			// en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}
	
}
	

int run_single_level(int level, int score, SimpleBitmap bmp){

	Player player(born_x, born_y);

	std::vector<Bubble> bubbles;
	std::vector<Enemy> enemies;
	bubble_bmp.LoadPng("../final_project/png/Bubble_ammo.png");

	assign_monster_loc(level, &bubbles, &enemies);

	player.j_cnt = 0;
	player.f_cnt = 0;

	for(;;)
	{

		FsPollDevice();
		auto key=FsInkey();
		
		mmlplayer.KeepPlaying();  
		if(mmlplayer.StreamPlayerReadyToAcceptNextSegment(waveStream,nextWave))
		{
			mmlplayer.AddNextStreamingSegment(waveStream,nextWave);
			auto rawWave=mml.GenerateWave(100);
			nextWave.CreateFromSigned16bitStereo(YM2612::WAVE_SAMPLING_RATE,rawWave);

			if(0!=mml.GetLastErrorCode())
			{
				for(auto msg : mml.GetLastError().Format())
				{
					std::cout << msg << std::endl;
				}
			}
		}


		if(FSKEY_ESC==key){
			exit(0);
		}


		if(FSKEY_LEFT==key){
			if(player.check_room(bmp) != 1){
				player.move(1);
			}
		}else if(FSKEY_RIGHT==key){
			if(player.check_room(bmp) != 2){
				player.move(0);
			}
		}

		if(FSKEY_UP==key)
		{
			if(player.j_cnt == 0 && player.f_cnt == 0 && (!player.falling)){
				player.j_cnt = 80;
				player.f_cnt = 80;
			}
		}
		
		if(FSKEY_SPACE==key)
		{
			Bubble bubble;
			if(player.dir == 0){
				bubble.x = std::min(int(player.x + bubble_bmp.GetWidth()), int(640-2*bubble_bmp.GetWidth()));
				bubble.y = player.y;
			}else{
				bubble.x = std::max(int(player.x - bubble_bmp.GetWidth()), int(bubble_bmp.GetWidth()));;
				bubble.y = player.y;
			}
			bubble.dir = player.dir;
			bubble.dash_cnt = get_dash_cnt(bmp, bubble);
			bubbles.push_back(bubble);
		}


		player.moving(bmp);		

		int type = 0;
		if(check_collision(&player, &enemies, &type)){
			if(type == 1){
				if(!player.undefeated){
					player.x = born_x;
					player.y = born_y;
					player.j_cnt = 0;
					life -= 1;
					player.undefeated = true;
					std::cout << "Attacked by enemy! Reborn! Life remaining: " << life << std::endl;
					std::cout << "Undefeated state begin now!" << std::endl;
				}
			}else{
				score += 100;
				std::cout << "Score: " << score << std::endl;
			}
		}

		if(enemies.size() == 0){
			return score;
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(0, 0);
		glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glDrawPixels(bmp.GetWidth(),bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmp.GetBitmapPointer());
		
		
		glRasterPos2i(int(player.x), int(player.y));
		SimpleBitmap player_bmp = player.player_bmp();
		glDrawPixels(player_bmp.GetWidth(),player_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,player_bmp.GetBitmapPointer());

		check_hit(&bubbles, &enemies);

		for(int i =0; i<enemies.size(); i++){
			enemies.at(i).moving(bmp, player.y, player.jumped || player.falling);
			glRasterPos2i(int(enemies.at(i).x), int(enemies.at(i).y));
			SimpleBitmap enemy_bmp = enemies.at(i).enemy_bmp();
			glDrawPixels(enemy_bmp.GetWidth(),enemy_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,enemy_bmp.GetBitmapPointer());
		}

		for(int i = 0; i<bubbles.size(); i++){
			bubbles.at(i).moving();
			if(bubbles.at(i).y <= 0){
				bubbles.erase(bubbles.begin() + i);
				continue;
			}
			glRasterPos2i(int(bubbles.at(i).x), int(bubbles.at(i).y));
			glDrawPixels(bubble_bmp.GetWidth(),bubble_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bubble_bmp.GetBitmapPointer());
		}

		need_terminate();

		glPixelZoom(1.0,-1.0);
		FsSwapBuffers();
		
	}

}

void menu(){
	SimpleBitmap menu_bmp;
	menu_bmp.LoadPng("../final_project/png/menu.png");
	for(;;){
		FsPollDevice();
		auto key=FsInkey();

		if(FSKEY_SPACE==key)
		{
			break;
		}
		if(FSKEY_ESC==key)
		{
			exit(0);
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glRasterPos2i(0, 0);
		glDrawPixels(menu_bmp.GetWidth(),menu_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,menu_bmp.GetBitmapPointer());
		
		glPixelZoom(1.0,-1.0);
		FsSwapBuffers();
	}
}

void run(){
	
	temp.LoadPng("../final_project/png/level1.png");
	black_pixel = temp.CutOut(100, 100, 1, 1);

    SimpleBitmap bmp;
	int level;

	mmlplayer.Start();
	mmlplayer.StartStreaming(waveStream);
	mml.AddSegment(
		"T180O4L4",
		"T180O4L4",
		"T180O4L4"
	);
	for (int i = 0; i < 100; i++){
		mml.AddSegment(
		"@2T90L12V12",
		"@2T90L2S0M6000",
		"@2T90L4V12"
		);

		mml.AddSegment(
		"O3G+O4C+EO3G+O4C+EO3G+O4C+EO3G+O4C+E",
		"O3C+1",
		"O2C+1"
	);
	}


	
	FsOpenWindow(0,0,640,480,1);
	
	menu();

	int score = 0;
	
    bmp.LoadPng("../final_project/png/level1.png");
    score = run_single_level(1, score, bmp);
    bmp.LoadPng("../final_project/png/level2.png");
	score = run_single_level(2, score, bmp);
	bmp.LoadPng("../final_project/png/level3.png");
	score = run_single_level(3, score, bmp);

	
}



int main(int argc,char *argv[]){
	std::cout << "Press LEFT and RIGHT to move, UP to jump, and SPACE to attack" << std::endl;
	run();
    return 0;
}