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
