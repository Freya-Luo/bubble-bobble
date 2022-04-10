#include <iostream>
#include "simplebitmap.h"
#include "fssimplewindow.h"
#include <vector>

SimpleBitmap temp;
SimpleBitmap black_pixel;
SimpleBitmap bubble_bmp;
float born_x = 100;
float born_y = 400;

int life = 3;

class Bubble{
	public:
		float x, y;
		int dir;//0 is right, 1 is left;
		int dash_cnt = 75;
		void moving();
};

void Bubble::moving(){
	if(dash_cnt >= 0){
		if(dir == 0){
			x += 1;
		}else{
			x -= 1;
		}
		dash_cnt -= 1;
	}else{
		y -= 1;
	}
}


class Enemy{
	public:
		float x, y;
		bool live;
		bool jumped = false;
		bool falling = false;
		int j_cnt, f_cnt;  
		
    	SimpleBitmap enemy_bmp_l;
    	SimpleBitmap enemy_bmp_r;
		SimpleBitmap fruit;
		int dir = 0; //0 is right, 1 is left;
		void moving(SimpleBitmap bmp);
		int check_room(SimpleBitmap bmp);
};

int Enemy::check_room(SimpleBitmap bmp){
	SimpleBitmap loc_l, loc_r, loc_u, loc_d;
	
	loc_l = bmp.CutOut(int(x), int(y)+16, 1, 1);;
	loc_r = bmp.CutOut(int(x)+32, int(y)+16, 1, 1);

	if(!loc_l.operator==(black_pixel)){//left side wall
		return 1;
	}else if(!loc_r.operator==(black_pixel)){//right side wall
		return 2;
	}else{
		return 0; //no wall
	}
	
}

void Enemy::moving(SimpleBitmap bmp){
	int room = check_room(bmp);
	if(live != 0){
		if(dir == 0 && room !=2){
				x += 0.4;
			}else if(dir == 1 && room != 1){
				x -= 0.4;
			}
			if(room != 0){
				dir = 1 - dir;
				if(dir == 1){
					x -= 0.4;
				}else{
					x += 0.4;
				}
			}

	}
	
	if(j_cnt > 0){
		jumped = true;
		y -= 0.8f;
		j_cnt -= 1;
	}
	if(f_cnt > 0 && j_cnt == 0){
		jumped = false;
		y += 0.8f;
		f_cnt -= 1;
	}

	if(y+32 >= 480){
		y = 0;
	}

	if(y < 20){
		y = float(20);	
	}

	SimpleBitmap player_loc = bmp.CutOut(int(x)+16, int(y)+32, 1, 1);
	if(black_pixel.operator==(player_loc) && (!jumped)){
		y += 0.8;
		f_cnt = 0;
		falling = true;
	}else{
		falling = false;
	}
}


class Player {       
  public:            
	float  x, y;
	bool jumped = false;
	bool falling = false;
	int dir = 0; //0 is right, 1 is left;
	int j_cnt, f_cnt ;     

    SimpleBitmap player_bmp_l;
    SimpleBitmap player_bmp_r;
	void moving(SimpleBitmap bmp);
	int check_room(SimpleBitmap bmp);
};

int Player::check_room(SimpleBitmap bmp){
	SimpleBitmap loc_l, loc_r, loc_u, loc_d;
	
	loc_l = bmp.CutOut(int(x), int(y)+16, 1, 1);;
	loc_r = bmp.CutOut(int(x)+32, int(y)+16, 1, 1);

	if(!loc_l.operator==(black_pixel)){
		return 1;
	}else if(!loc_r.operator==(black_pixel)){
		return 2;
	}else{
		return 0;
	}
	
}

void Player::moving(SimpleBitmap bmp){
	if(j_cnt > 0){
		jumped = true;
		y -= 1;
		j_cnt -= 1;
	}
	if(f_cnt > 0 && j_cnt == 0){
		jumped = false;
		y += 1;
		f_cnt -= 1;
	}

	if(y+32 >= 480){
		y = 0;
	}
		
	if(y < float(20)){
		y = float(20);	
	}

	SimpleBitmap loc_d;
	loc_d = bmp.CutOut(int(x)+16, int(y)+32, 1, 1);;

	if(black_pixel.operator==(loc_d) && (!jumped)){
		y += 0.8;
		f_cnt = 0;
		falling = true;
	}else{
		falling = false;
	}

}


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
			Enemy en;
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			en.fruit.LoadPng("../final_project/png/banana.png");
			en.x = 300;
			en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}else if(level == 2){
		for (int i =0; i<en_cnt; i++){
			Enemy en;
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			en.fruit.LoadPng("../final_project/png/banana.png");
			en.x = 400;
			en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}else{
		for (int i =0; i<en_cnt; i++){
			Enemy en;
			en.j_cnt = 0;
			en.f_cnt = 0;
			en.dir = rand()%2;
			en.live = true;
			en.enemy_bmp_l.LoadPng("../final_project/png/mob_walking_left.png");
			en.enemy_bmp_r.LoadPng("../final_project/png/mob_walking_right.png");
			en.fruit.LoadPng("../final_project/png/banana.png");
			en.x = 200;
			en.y = i*150 + 100;
			enemies->push_back(en);
		}
	}
	
}
	

int run_single_level(int level, int score, SimpleBitmap bmp){
	Player player;
	
	std::vector<Bubble> bubbles;
	std::vector<Enemy> enemies;
	bubble_bmp.LoadPng("../final_project/png/Bubble_ammo.png");

	assign_monster_loc(level, &bubbles, &enemies);

	player.x = born_x;
	player.y = born_y;
	player.player_bmp_r.LoadPng("../final_project/png/Bubble.png");
	player.player_bmp_l.LoadPng("../final_project/png/Bubble_left.png");
	player.j_cnt = 0;
	player.f_cnt = 0;

	for(;;)
	{

		FsPollDevice();
		auto key=FsInkey();
		
		if(FSKEY_ESC==key){
			exit(0);
		}


		if(FSKEY_LEFT==key){
			if(player.check_room(bmp) != 1){
				player.x -= 10;
				player.dir = 1;
			}
		}else if(FSKEY_RIGHT==key){
			if(player.check_room(bmp) != 2){
				player.dir = 0;
				player.x += 10;
			}
		}

		if(FSKEY_UP==key)
		{
			if(player.j_cnt == 0 && player.f_cnt == 0 && (!player.falling)){
				player.j_cnt = 120;
				player.f_cnt = 120;
			}
		}
		
		if(FSKEY_SPACE==key)
		{
			Bubble bubble;
			if(player.dir == 0){
				bubble.x = player.x + bubble_bmp.GetWidth();
				bubble.y = player.y;
			}else{
				bubble.x = player.x - bubble_bmp.GetWidth();
				bubble.y = player.y;
			}
			bubble.dir = player.dir;
			bubbles.push_back(bubble);
		}


		player.moving(bmp);		

		int type = 0;
		if(check_collision(&player, &enemies, &type)){
			if(type == 1){
				player.x = born_x;
				player.y = born_y;
				player.j_cnt = 0;
				life -= 1;
				std::cout << "Attacked by enemy! Reborn! Life remaining: " << life << std::endl;
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
		glDrawPixels(bmp.GetWidth(),bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bmp.GetBitmapPointer());
		
		
		glRasterPos2i(int(player.x), int(player.y));
		if(player.dir == 1){
			glDrawPixels(player.player_bmp_l.GetWidth(),player.player_bmp_l.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,player.player_bmp_l.GetBitmapPointer());
		}else{
			glDrawPixels(player.player_bmp_r.GetWidth(),player.player_bmp_r.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,player.player_bmp_r.GetBitmapPointer());
		}

		check_hit(&bubbles, &enemies);

		for(int i =0; i<enemies.size(); i++){
			enemies.at(i).moving(bmp);
			glRasterPos2i(int(enemies.at(i).x), int(enemies.at(i).y));
			if(enemies.at(i).live){
				if(enemies.at(i).dir == 1){
					glDrawPixels(enemies.at(i).enemy_bmp_l.GetWidth(),enemies.at(i).enemy_bmp_l.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,enemies.at(i).enemy_bmp_l.GetBitmapPointer());
				}else{
					glDrawPixels(enemies.at(i).enemy_bmp_r.GetWidth(),enemies.at(i).enemy_bmp_r.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,enemies.at(i).enemy_bmp_r.GetBitmapPointer());		
				}
			}else{
					glDrawPixels(enemies.at(i).fruit.GetWidth(),enemies.at(i).fruit.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,enemies.at(i).fruit.GetBitmapPointer());
			}
		}

		for(int i = 0; i<bubbles.size(); i++){
			bubbles.at(i).moving();
			glRasterPos2i(int(bubbles.at(i).x), int(bubbles.at(i).y));
			glDrawPixels(bubble_bmp.GetWidth(),bubble_bmp.GetHeight(),GL_RGBA,GL_UNSIGNED_BYTE,bubble_bmp.GetBitmapPointer());
		}

		need_terminate();

		glPixelZoom(1.0,-1.0);
		FsSwapBuffers();
		
	}

}


void run(){
	
	temp.LoadPng("../final_project/png/level1.png");
	black_pixel = temp.CutOut(100, 100, 1, 1);

    SimpleBitmap bmp;
	int level;

	
	FsOpenWindow(0,0,640,480,1);

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