#include <iostream> 
#include <cmath>
#include <vector> 
#include <unistd.h> 

using namespace std;;

float A, B, C;
float cw = 20; 
int w = 160, h = 44;


float z_inverse;
int cam_dist =100;
float k1 = 44;

float fps = 0.6; 
float x, y, z; 
int xprime, yprime; 
int index; 


float calcx(int i, int j, int k){
	return j*sin(A)*sin(B)*cos(C) - k*cos(A)*sin(B)*cos(C) + j*cos(A)*sin(C) + k*sin(A)*sin(C) + i*cos(B)*cos(C);
}

float calcy(int i, int j, int k){
	return j*cos(A)*cos(C) + k*sin(A)*cos(C) - j*sin(A)*sin(B)*sin(C) + k*cos(A)*sin(B)*sin(C) - i*cos(B)*sin(C); 
}

float calcz(int i, int j, int k){
	return k*cos(A)*cos(B) - j*sin(A)*cos(B) + i*sin(B);
}

void calc_surface(float cx, float cy, float cz, std::vector<float> &zbuf, std::vector<char> &buf, int c){
	x = calcx(cx, cy, cz); 
	y = calcy(cx, cy, cz);
	z = calcz(cx, cy, cz) + cam_dist;

	z_inverse = 1/z;
	xprime = (int)(k1 * x * z_inverse + w/2);
	yprime = (int)(k1 * y * z_inverse + h/2);

	index = xprime + yprime * w;
	if(index >= 0 and index < w*h){
		if(z_inverse > zbuf[index]){
			zbuf[index] = z_inverse; 
			buf[index] = c;
		}
	}
}


int main(void) {
	std::vector<float> zbuf(160*44*6); 
	std::vector<char> buf(160*60);
	std::cout << "\x1b[2J";
	while(true){
		std::fill(zbuf.begin(), zbuf.end(), 0); 
		std::fill(buf.begin(), buf.end(), ' ');

		for( float cx = -cw; cx<cw; cx+=fps){
			for(float cy = -cw; cy<cw; cy+=fps){
				calc_surface(cx, cy, -cw, zbuf, buf, '$');
				calc_surface(cx,-cw, cy,  zbuf, buf, '.');
				calc_surface(-cw, cy, cx,  zbuf, buf, '~');
				calc_surface(cy,-cx,cw,zbuf, buf, '@');
				calc_surface(cw, cy,-cx,zbuf, buf, '?');
				calc_surface(cx,cw,-cy,zbuf, buf, '#');
			}
		}
		std::cout << "\x1b[H";

		for(int i = 0; i< w*h; ++i){
			if(i%w){
				std::cout << buf[i];
			}
			else{
				std::cout << "\n";
			}
		}

		A += 0.05; 
		B +=0.05;
		C += 0.01;
		usleep(1000*40);
	}

	return 0;
}

