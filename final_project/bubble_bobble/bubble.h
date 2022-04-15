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