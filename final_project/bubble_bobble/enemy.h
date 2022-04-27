#include "simplebitmap.h"


SimpleBitmap temp;

class Enemy{
	public:
		float x, y;
		bool live;
		bool jumped = false;
		bool falling = false;
		int j_cnt, f_cnt;  
		

    	SimpleBitmap enemy_bmp_l1;
    	SimpleBitmap enemy_bmp_l2;
    	SimpleBitmap enemy_bmp_r1;
    	SimpleBitmap enemy_bmp_r2;
		SimpleBitmap fruit;
		int walk_state = 0;

		int dir = 0; //0 is right, 1 is left;
		int pre_state = 0;
		Enemy(float born_x, float born_y) {
			x = born_x;
			y = born_y;
			enemy_bmp_l1.LoadPng("../final_project/png/mob_walking_left.png");
			enemy_bmp_l2.LoadPng("../final_project/png/mob_walking_left2.png");
			enemy_bmp_r1.LoadPng("../final_project/png/mob_walking_right.png");
			enemy_bmp_r2.LoadPng("../final_project/png/mob_walking_right2.png");
			fruit.LoadPng("../final_project/png/banana.png");
		}
		void moving(SimpleBitmap bmp, float player_y, bool player_moving);
		int check_room(SimpleBitmap bmp);
		SimpleBitmap Enemy::enemy_bmp(void);

};

 
SimpleBitmap Enemy::enemy_bmp(void) {
	if(!live) {
		return fruit;
	}

	if (dir == 0) {
		if (walk_state < 15) {
			return enemy_bmp_r1;
		} else {
			return enemy_bmp_r2;
		} 
	} else {
		if (walk_state < 15) {
			return enemy_bmp_l1;
		} else {
			return enemy_bmp_l2;
		} 
	}
}


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

void Enemy::moving(SimpleBitmap bmp, float player_y, bool player_moving){
	int room = check_room(bmp);
	
	if(live != 0){
		if(player_y < y - 20 && j_cnt == 0 && !player_moving){
			if(j_cnt == 0 && f_cnt == 0 && (!falling)){
				j_cnt = 80;
				f_cnt = 80;
				return;
			}
		}
		if(dir == 0 && room !=2){
				x += 0.5f;
				walk_state = (walk_state + 1) % 31;

		}else if(dir == 1 && room != 1){
				x -= 0.5f;
				walk_state = (walk_state + 1) % 31;
		}
		if(room != 0 && j_cnt == 0){
			dir = 1 - dir;
			if(dir == 1){
				x -= 0.5f;
				walk_state = 0;
			}else{
				x += 0.5f;
				walk_state = 0;
			}
		}
	}
	
	if(j_cnt > 0){
		jumped = true;
		y -= 1.4f;
		j_cnt -= 1;
	}
	if(f_cnt > 0 && j_cnt == 0){
		jumped = false;
		y += 1.4f;
	}

	if(y+32 >= 480){
		y = 0;
	}

	if(y < 20){
		y = float(20);	
	}

	SimpleBitmap player_loc = bmp.CutOut(int(x)+16, int(y)+32, 1, 1);
	if(black_pixel.operator==(player_loc) && (!jumped)){
		y += 1.2;
		f_cnt = 0;
		falling = true;
	}else{
		falling = false;
	}
}
