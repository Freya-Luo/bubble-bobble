
SimpleBitmap black_pixel;

class Bubble{
	public:
		float x, y;
		int dir;//0 is right, 1 is left;
		int dash_cnt;
		void moving();
};

int get_dash_cnt(SimpleBitmap bmp, Bubble b){
	SimpleBitmap loc_l, loc_r;
	int max_cnt = 75;
	if(b.dir == 1){
		for(int i = 0; i <max_cnt; i++){
			loc_l = bmp.CutOut(int(b.x-8)-i, int(b.y)+16, 1, 1);
			if(loc_l.operator==(black_pixel)){
				continue;
			}else{
				return i;
			}
		}
	}else{
		for(int i = 0; i <max_cnt; i++){
			loc_r = bmp.CutOut(int(b.x)+32+i, int(b.y)+16, 1, 1);
			if(loc_r.operator==(black_pixel)){
				continue;
			}else{
				return i;
			}
		}
	}
	return max_cnt;
}

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