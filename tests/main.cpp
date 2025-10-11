#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>

using namespace std;


int rgb2hex (u_int8_t r,u_int8_t g,u_int8_t b) {
    return  int((r<<16) + (g<<8) + (b));
}
vector<u_int8_t> hex2rgb (int hexCode) {
    vector<u_int8_t> ret_vector(3);
    ret_vector[0] = u_int8_t((hexCode)>>16);
    ret_vector[1] = u_int8_t((hexCode - (ret_vector[0]<<16))>>8);
    ret_vector[2] = u_int8_t(hexCode - ((ret_vector[0]<<16) + ((ret_vector[1]<<8))));
    return ret_vector;
}
void save_map (int h, int w, vector<vector<int>> *img) {
    ofstream ofs ("map.ppm", ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h;i++) { 
        for (int j=0;j<w;j++) { vector<u_int8_t> temp = hex2rgb((*img)[i][j]); ofs << temp[0] << temp[1] << temp[2]; }
    }
    ofs.close();
}
void draw_map (int window_height, int window_width, int map_height, int map_width, vector<vector<int>>* img, vector<string>* map) {
    int scaling = window_width/map_height;     //PROBLEM: window and height should have diff scacling value 
    for(int row=0;row<map_height;row++) {
        for(int col=0;col<map_width;col++) {
            if ((*map)[row][col] != ' ') {
                (*img)[row*scaling][col*scaling] = rgb2hex(255,255,255);
                for(int i=0;i<scaling;i++) {
                    for(int j=0;j<scaling;j++) {
                        (*img)[row*scaling+i][col*scaling+j] = rgb2hex(255,255,255);
                    }
                }
            }
        }
    }
}



void draw_entity (float ent_x,float ent_y,int window_height, int window_width,vector<vector<int>>* img) {
    float COORDINATES_MIN = 0;
    float COORDINATES_MAX = 64;
    float scaling = window_height/COORDINATES_MAX;
    float scale_down = 1-COORDINATES_MAX/float(window_height);
    //for(int row=0;row<COORDINATES_MAX;row++) {for(int col=0;col<COORDINATES_MAX;col++) {}}

    (*img)[ent_x*scaling][ent_y*scaling]=rgb2hex(0,0,0);



    

}





int main(){

    unordered_map<int,char> int2char = {{1,'1'},{2,'2'},{3,'3'},{4,'4'},{5,'5'},{6,'6'},{7,'7'},{8,'8'},{9,'9'},{0,'0'}};

    vector<string> vectorMap = {
        "0000222222220000",
        "1              0",
        "1      11111   0",
        "1     0        0",
        "0     0  1110000",
        "0     3        0",
        "0   10000      0",
        "0   0   11100  0",
        "0   0   0      0",
        "0   0   1  00000",
        "0       1      0",
        "2       1      0",
        "0       0      0",
        "0 0000000      0",
        "0              0",
        "0002222222200000"
    };
    int map_height = size(vectorMap);
    int map_width = vectorMap[0].size();
    int window_height = 512;
    int window_width = 512;
    vector<int> innerList(window_width,0);
    vector<vector<int>> framebuffer(window_height,innerList);

    for (int row=0;row<(window_height);row++) {
        for (int col=0;col<(window_width);col++) { framebuffer[row][col] = rgb2hex(255,0,0); }
    }
    draw_map(window_height,window_width,map_height,map_width,&framebuffer,&vectorMap);

    int player_x = 0;
    int player_y = 0;
   
    for (float player_pos_y=0;player_pos_y<64;player_pos_y+=0.25){
        for (float player_pos_x=0;player_pos_x<64;player_pos_x+=0.25){
            draw_entity(player_pos_x,player_pos_y,window_height,window_width,&framebuffer);
        }
    }

    save_map(window_height,window_width,&framebuffer);
    
    

    return 0;
}