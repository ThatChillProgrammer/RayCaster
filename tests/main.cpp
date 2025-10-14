#define _USE_MATH_DEFINES

#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "./headerfiles/stb_image.h"

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
vector<vector<int>> textureLoad(char *filename) {
    int x,y,n;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
    vector<int> colors(x*y);
    int count = 0;
    for (int i=0;i<(x*y)*3;i+=3) {
        colors[count] = rgb2hex(static_cast<int>(data[i]),static_cast<int>(data[1+i]),static_cast<int>(data[2+i]));
        count++;
    } 
    vector<int> innerList(x,0);
    vector<vector<int>> texture(y,innerList);
    for (int i=0;i<y;i++) {
        for (int j=0;j<x;j++) { texture[i][j] = colors[(i*y)+j]; }
    }
    return texture;
} 

char T_filepath[] = {"./textures/texture001.png"};
vector<vector<int>> brickwall_t = textureLoad(T_filepath);

void save_map (int h, int w, vector<vector<int>> *img) {
    //string map = "../images/map000.ppm";
    //map[13] = number[0];
    //map[14] = number[1];
    //map[15] = number[2];
    string map = "./images/map.ppm";
    ofstream ofs (map, ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h;i++) { 
        for (int j=0;j<w;j++) { vector<u_int8_t> temp = hex2rgb((*img)[i][j]); ofs << temp[0] << temp[1] << temp[2]; }
    }
    ofs.close();
}
void save_map1 (int h, int w, vector<vector<int>> *img) {
    ofstream ofs ("./images/3dview.ppm", ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h;i++) { 
        for (int j=0;j<w;j++) { vector<u_int8_t> temp = hex2rgb((*img)[i][j]); ofs << temp[0] << temp[1] << temp[2]; }
    }
    ofs.close();
}
void draw_map (int window_height, int window_width, int map_height, int map_width, vector<vector<int>>* img, vector<string>* map) {
    unordered_map<char,int> colors = {{'0',rgb2hex(255,255,255)},{'1',rgb2hex(0,255,255)},{'2',rgb2hex(0,255,0)},{'3',rgb2hex(0,0,255)},{'4',rgb2hex(255,0,255)},{'4',rgb2hex(255,0,255)}};
    int scaling = window_width/map_height;     
    for(int row=0;row<map_height;row++) {
        for(int col=0;col<map_width;col++) {
            if (colors[(*map)[row][col]]) {
                (*img)[row*scaling][col*scaling] = colors[(*map)[row][col]];
                for(int i=0;i<scaling;i++) {
                    for(int j=0;j<scaling;j++) {
                        (*img)[row*scaling+i][col*scaling+j] = colors[(*map)[row][col]];
                    }
                }
            }
            (*img)[row*scaling][col*scaling] = colors['5'];
        }
    }
}
vector<vector<float>> draw_entity (float ent_x,float ent_y,float ent_a,int window_height, int window_width,vector<vector<int>>* img,vector<vector<int>>* map) {
    vector<string> vectorMap = {
        "0000222222220000",
        "1              0",
        "1     111111   0",
        "1     0        0",
        "0     0  1110000",
        "0     3        0",
        "0   11100      0",
        "0   0   11100  0",
        "0   0   0      0",
        "0   0   1  00000",
        "0       1      0",
        "2       1      0",
        "0       0      0",
        "0 3333333      0",
        "0              0",
        "1111122222233000"
    };
    unordered_map<int,int> colors = {{rgb2hex(255,255,255),brickwall_t[0][0]},{rgb2hex(0,255,255),rgb2hex(0,255,255)},{rgb2hex(0,255,0),rgb2hex(0,255,0)},{rgb2hex(0,0,255),rgb2hex(0,0,255)},{rgb2hex(255,0,255),rgb2hex(255,0,255)}};
    float scaling = window_height/64;
    float fov = 30;
    float decrement = (fov*2)/window_height;
    int cur_e_x = ent_x*scaling;
    int cur_e_y = ent_y*scaling;
    vector<float> distance = {0,0};
    float component_x = 0;
    float component_y = 0;  
    float fov_bound = ent_a+fov;
    vector<vector<float>> distances;
    for (int i=0;i<window_height;i++){
        for (int j=0;j<window_width;j++){
            float c_x = (component_x*scaling)+cur_e_y;
            float c_y = (component_y*scaling)+cur_e_x;
            if (colors[(*map)[c_x][c_y]] ) {


                cout << 
                    "vectormap: " << vectorMap[(c_x)/(window_height/16)][(c_y)/(window_width/16)] << 
                    "            x: " << int((c_y)/(window_width/16))*32  <<
                    "            y: " << int((c_x)/(window_height/16))*32 << 
                    "            x: " << c_y <<
                    "            y: " << c_x <<
                    "      texture: " << c_x <<
                endl;
                
                
                
                distance[0] = distance[0]*cos((ent_a - fov_bound) * M_PI/180);
                distance[1] = colors[(*map)[c_x][c_y]];
                distances.push_back(distance);
                break;
            } 
            (*img)[c_x][c_y]=8421504;
            distance[0]+=decrement;
            component_x = distance[0]*sin(fov_bound * M_PI/180);
            component_y = distance[0]*cos(fov_bound * M_PI/180);
        }
        distance[0]=0;
        component_x=0;
        component_y=0;
        fov_bound-=decrement;
    }
    return distances;
}
void draw_3d (int height, int width, vector<vector<float>>* distances, vector<vector<int>>* img) {
    int color = rgb2hex(255,0,0);
    int othCOl = rgb2hex(255,255,255);
    int reverse_list = size(*distances)-1;
    for (int i=0;i<width;i++) {
        float object_size = (height/((*distances)[reverse_list][0]))*3;
        int start_drawing = (height-object_size)/2;
        int stop_drawing = 2;
        for (int j=0;j<(height);j++) {
            if(j>=start_drawing && j<=start_drawing+object_size ){
                (*img)[j][i] = (*distances)[reverse_list][1];
            } else { (*img)[j][i] = othCOl; }
        }
        reverse_list-=1;
    }
}





int main(){
    cout << int(hex2rgb(brickwall_t[0][0])[0]) << " " << int(hex2rgb(brickwall_t[0][0])[1]) << " " <<  int(hex2rgb(brickwall_t[0][0])[2])<< endl;


    vector<string> vectorMap = {
        "0000222222220000",
        "1              0",
        "1     111111   0",
        "1     0        0",
        "0     0  1110000",
        "0     3        0",
        "0   11100      0",
        "0   0   11100  0",
        "0   0   0      0",
        "0   0   1  00000",
        "0       1      0",
        "2       1      0",
        "0       0      0",
        "0 3333333      0",
        "0              0",
        "1111122222233000"
    };
    
    int map_height = size(vectorMap);
    int map_width = vectorMap[0].size();
    int window_height = 512;
    int window_width = 512;
    vector<int> innerList(window_width,16711680);
    vector<vector<int>> framebuffer(window_height,innerList);

    draw_map(window_height,window_width,map_height,map_width,&framebuffer,&vectorMap);
    vector<vector<int>> Map = framebuffer;

    float player_x = 15;//0-64
    float player_y = 15;//0-64
    float player_angle = 90; //0-360
    
    string number = "";
    string pass2func = "";
    vector<vector<float>> distances = draw_entity(player_x,player_y,player_angle,window_height,window_width,&framebuffer,&Map);
    save_map(window_height,window_width,&framebuffer);
    draw_3d(window_height,window_width,&distances,&framebuffer);
    save_map1(window_height,window_width,&framebuffer);

    return 0;
}