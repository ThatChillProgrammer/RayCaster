#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>

using namespace std;


string rgb2hex (vector <int> rgb) {
    vector<char> hex = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    string hexCode;
    for (int i=0;i<3;i++){ hexCode += hex[int(rgb[i]/16)]; hexCode += hex[(rgb[i]%16)]; }
    return hexCode;
}
vector<u_int8_t> hex2rgb (string hexCode) {
    unordered_map<char,u_int8_t> hex = {{'0',0},{'1',1},{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},
    {'7',7},{'8',8},{'9',9},{'a',10},{'b',11},{'c',12},{'d',13},{'e',14},{'f',15}};
    vector<u_int8_t> rgb(3);
    for (int i=0;i<3;i++) { rgb[i] = u_int8_t((hex[hexCode[i*2]]*16)+hex[hexCode[i*2+1]]); }
    return rgb;
}
void save_map (int h, int w, vector<vector<u_int8_t>> *img) {
    ofstream ofs ("map.ppm", ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h*w;i++) { ofs << (*img)[i][0] << (*img)[i][1] << (*img)[i][2]; }
    ofs.close();
}

void draw_map (int window_height, int window_width, int map_height, int map_width, vector<vector<u_int8_t>>* img, string* map) {
    vector<u_int8_t> map_color = {255,255,255};
    int pixel_size;
    if (window_width%map_width==0 || window_width%map_width<5) {pixel_size=window_width/map_width;} 
    else {pixel_size=window_width/map_width+1;}

    int pixelsinpixel = pixel_size*pixel_size;

    int map_iterator = 0;
    int current_y = 0;
    int current_x = pixel_size;

    for (int i=0;i<map_height;i++) {
        for (int j=0;j<map_width;j++) {
            if ((*map)[map_iterator] == '@'){
                int current_verticy = current_x+current_y;
                for(int pixel_x=0;pixel_x<pixel_size;pixel_x++) {
                    int color_y=current_verticy + pixel_x;
                    (*img)[current_verticy + pixel_x] = map_color;
                    for(int pixel_y=0;pixel_y<pixel_size-1;pixel_y++) {
                        (*img)[color_y+window_width] = map_color;
                        color_y += window_width;
                    }

                }
            }
            current_x += pixel_size;
            map_iterator++;
        }
        current_x = 0;
        current_y += window_width*pixel_size;
    }
}
  

int main(){
    string map = 
        "          "
        " @@@@@@@@ "
        " @  @     "
        " @  @     "
        " @  @     "
        " @  @     "
        " @@@     @"
        "          "
        "          "
        "          "
        " @@@     @"
        "          "
        "          ";
    int map_width = 10;
    int map_height = 10;


    int window_height = 10;
    int window_width = 10;
    vector<vector<u_int8_t>> framebuffer(window_height*window_width, {255,0,0});
    draw_map(window_height,window_width,map_height,map_width,&framebuffer,&map);
    save_map(window_height,window_width,&framebuffer);

    return 0;
}