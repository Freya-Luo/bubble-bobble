#include "simplebitmap.h"


class Player {       
  public:            
	float  x, y;
	bool jumped = false;
	bool falling = false;
	int dir = 0; //0 is right, 1 is left;
	int j_cnt, f_cnt ;     
	bool undefeated = false;
	int undefeat_cnt = 0;
	int walk_state = 0;
	int state = 0;
	SimpleBitmap player_bmp_l1;
	SimpleBitmap player_bmp_l2;
	SimpleBitmap player_bmp_l3;
	SimpleBitmap player_bmp_r1;
	SimpleBitmap player_bmp_r2;
	SimpleBitmap player_bmp_r3;
  // Player(float born_x, float born_y); // constructor
	Player(float born_x, float born_y) {
			x = born_x;
			y = born_y;
			player_bmp_r1.LoadPng("../final_project/png/Bubble_walk2.png");
			player_bmp_r2.LoadPng("../final_project/png/Bubble_walk1.png");
			player_bmp_l1.LoadPng("../final_project/png/Bubble_left.png");
			player_bmp_l2.LoadPng("../final_project/png/Bubble_walk3.png");
		}
	
  void move(int action);
  SimpleBitmap player_bmp(void);	
	void moving(SimpleBitmap bmp);
	int check_room(SimpleBitmap bmp);
};


void Player::move(int action) {
	// 0: move right, 1: move left, 2: jump, 3: falling
	if (action == 0) {
		// move right
		if (state == 0) {
			// same direction
			walk_state = (walk_state + 1) % 2;
			x += 10;
			dir = 0;
			state = action;
		} else {
			// different direction
			walk_state = 0;
			x += 10;
			dir = 0;
			state = action;
		}
	} else if (action == 1) {
		// move left
		if (state == 0) {
			// different direction
			walk_state = 0;
			x -= 10;
			dir = 1;
			state = action;
		} else {
			// same direction
			walk_state = (walk_state + 1) % 3;
			x -= 10;
			dir = 1;
			state = action;
		}
	}
}

SimpleBitmap Player::player_bmp(void) {
	if (state == 0) {
		if (walk_state == 0) {
			return player_bmp_r1;
		} else {
			return player_bmp_r2;
		} 
	} else {
		if (walk_state == 0) {
			return player_bmp_l1;
		} else {
			return player_bmp_l2;
		} 
	}
}


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
	if(undefeated){
		undefeat_cnt += 1;
		if(undefeat_cnt == 300){
			std::cout << "Undefeated state after reborn ends! Be careful!" << std::endl;
			undefeat_cnt = 0;
			undefeated = false;
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
		
	if(y < float(20)){
		y = float(20);	
	}

	SimpleBitmap loc_d;
	loc_d = bmp.CutOut(int(x)+16, int(y)+32, 1, 1);;

	if(black_pixel.operator==(loc_d) && (!jumped)){
		y += 1.4f;
		f_cnt = 0;
		falling = true;
	}else{
		falling = false;
		jumped = false;
	}

}
