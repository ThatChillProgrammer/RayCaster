#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string map = 
"                        "
"  @@@@@@@@@@@@@@@@@@@@  "
"  @                  @  "
"  @                  @  "
"  @                  @  "
"  @                  @  "
"  @                  @  "
"  @                  @  "
"  @@@@@@@@@@@@@@@@@@@@  "
"                        ";

int map_width = 24;
int map_height = 10;

void draw_ppm (u_int16_t h, u_int16_t w, vector<u_int16_t> img) {
    ofstream ofs ("map.ppm", std::ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    float adder = 0;
    vector<uint8_t> rgb = {0,0,255}; //rgb[0]=red rgb[1]=blue rgb[2]=green
    for (int i=0;i<size(img);i++) {
        ofs << rgb[0] << rgb[1] << rgb[2];  
        if (adder >= 1) {
            if (rgb[0]>255){rgb[0]=255;} else {rgb[0]+=1;}
            if (rgb[1]>255){rgb[1]=255;} else {rgb[1]+=0;}
            if (rgb[2]>255){rgb[2]=255;} else {rgb[2]-=1;}
            adder = 0;
        } else {adder += (255/float(size(img)));}
    }
    ofs.close();
}

int main(){
    u_int16_t height = 512;
    u_int16_t width = 512;

    vector<u_int16_t> framebuffer(height*width, 0);
    draw_ppm(height,width,framebuffer);
    return 0;
}