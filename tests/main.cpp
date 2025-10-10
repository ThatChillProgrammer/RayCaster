#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

string map = 
"     "
" @@@ "
" @ @ "
" @@@ "
"     ";

int map_width = 5;
int map_height = 5;

u_int8_t color2_uint8 (int r, int g, int b) {

}

void draw_ppm_map (u_int16_t h, u_int16_t w, vector<u_int16_t> img, string map) {
    ofstream ofs ("map.ppm", ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    vector<uint8_t> rgb_ppm = {255,0,0}; //rgb[0]=red rgb[1]=blue rgb[2]=green
    vector<uint8_t> rgb_map_rectangles = {0,0,255};
    float adder = 0;
    int map_x=0;
    int map_y=0;
    for (int i=0;i<size(img);i++) {ofs << rgb_ppm[0] << rgb_ppm[1] << rgb_ppm[2];}
    ofs.close();
}


int main(){
    u_int16_t height = 8;
    u_int16_t width = 8;

    vector<u_int16_t> framebuffer(height*width, 0);
    cout<<size(framebuffer)<<endl;
    draw_ppm_map(height,width,framebuffer,map);
    cout<<map.size()<< " " << map_height*map_width << endl;
    
    u_int8_t color = color2_uint8(255,0,0)
    cout << 
    return 0;
}