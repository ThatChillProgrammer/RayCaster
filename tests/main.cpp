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


//fix
void draw_map (int window_height, int window_width, int map_height, int map_width, vector<vector<u_int8_t>>* img, string* map) {
    vector<u_char> map_color = {255,255,255};
    int pixel_size = window_width/map_width;

    int counter =0;
    for (int i=0;i<map_height;i++) {

        for (int j=0;j<map_width;j++) {

            if ((*map)[counter] == '@'){
                cout << "@: " << (window_height*pixel_size + i*pixel_size) <<  endl;
                (*img)[(window_height*pixel_size) + i] = map_color;

            }
            counter++;
        }

    }
}
  
/*
for (int i=0;i<size(*img);i+=pixel_size) {
        if ((*map)[count] == '@'){
            if((*img)[i] != map_color) {
                (*img)[i] = map_color;
                (*img)[i+1] = map_color;
                (*img)[i+map_width] = map_color;
                (*img)[i+map_width+1] = map_color;
            }
        }       
        count++;    
    }
*/

int main(){
    string map = 
        "     "
        " @@@ "
        " @ @ "
        " @@@ "
        "     ";
    int map_width = 5;
    int map_height = 5;


    int window_height = 10;
    int window_width = 10;
    vector<vector<u_int8_t>> framebuffer(window_height*window_width, {255,0,0});
    draw_map(window_height,window_width,map_height,map_width,&framebuffer,&map);
    save_map(window_height,window_width,&framebuffer);

    return 0;
}