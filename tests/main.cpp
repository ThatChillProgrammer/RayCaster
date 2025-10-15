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
        "0 1201333      0",
        "0              0",
        "1111122222233000"
};

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

char T_filepathB[] = {"./textures/rudy.jpg"};
vector<vector<int>> t_brickwall = textureLoad(T_filepathB);

unordered_map<char,vector<vector<int>>> LoadAllTextures (vector<string> filepaths) {
    unordered_map<char,vector<vector<int>>> Loaded_Textures;
    string characters = "0123456789ABCDEF";
    string T_filepathB = filepaths[0];

    for (int i=0;i<size(filepaths);i++) {
        if (i>characters.size()) {cout << "TO MANY TEXTURES" << endl; break;}
        char *cstr = (filepaths[i]).data();
        char test[] = {*cstr};
        Loaded_Textures[characters[i]] = textureLoad(cstr);
    }

    return Loaded_Textures;    
}

void save_map (int h, int w, vector<vector<int>> *img) {
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
    unordered_map<char,int> colors = {{'0',rgb2hex(255,255,255)},{'1',rgb2hex(0,255,255)},{'2',rgb2hex(0,255,0)},{'3',rgb2hex(0,0,255)},{'4',rgb2hex(255,0,255)},{'4',rgb2hex(0,0,0)}};
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
            //(*img)[row*scaling][col*scaling] = colors['5'];
        }
    }
}
void getTextureSlice(float ent_a, float fov_bound, float c_x, float c_y, int current_block_x, int current_block_y, int last_block_y,vector<float> &distance, int window_height, int window_width, vector<vector<float>> &distances)  {
    if (last_block_y == current_block_y) {distance[1] = ((c_y-current_block_y)*(64/(window_height/size(vectorMap))));
    } else {  distance[1] = ((c_x-current_block_x)*(64/(window_width/size(vectorMap[0]))));}
    distance[0] = distance[0]*cos((ent_a - fov_bound) * M_PI/180);
    distances.push_back(distance);
}
void zeroAngleVars(vector<float> &distance, vector<float> &ray_components) {
    distance[0]=0;
    ray_components[0]=0;
    ray_components[1]=0;
}

vector<vector<float>> compute_player_view (float ent_x,float ent_y,float ent_a,int window_height, int window_width,vector<vector<int>>* img,vector<vector<int>>* map) {
    unordered_map<int,int> colors = {{rgb2hex(255,255,255),1},{rgb2hex(0,255,255),2},{rgb2hex(0,255,0),3},{rgb2hex(0,0,255),rgb2hex(0,0,255)},{rgb2hex(255,0,255),rgb2hex(255,0,255)}};
    vector<float> distance = {0,0};
    vector<vector<float>> distances;

    float scaling = window_height/64;

    float fov = 30;
    float fov_bound = ent_a+fov;
    float decrement = (fov*2)/window_height;

    int player_pos_x = ent_x*scaling;
    int player_pos_y = ent_y*scaling;
    
    vector<float> ray_components = {0,0}; // (ray_x,ray_y)
    
    int last_block_y=0;

    for (int i=0;i<window_height;i++){

        float sinCalculation = sin(fov_bound * M_PI/180);
        float cosCalculation = cos(fov_bound * M_PI/180);

        for (int j=0;j<window_width;j++){

            float c_y = (ray_components[0]*scaling)+player_pos_y;
            float c_x = (ray_components[1]*scaling)+player_pos_x;

            int current_block_y = int((c_y)/(window_height/size(vectorMap)))*(window_height/size(vectorMap[0]));

            if (colors[(*map)[c_y][c_x]]) { 
                getTextureSlice(ent_a,fov_bound,c_x, c_y, int((c_x)/(window_width/size(vectorMap[0])))*(window_width/size(vectorMap)), current_block_y, last_block_y,distance, window_height,window_width,distances);
                break;
            } 

            (*img)[c_y][c_x]=8421504;

            distance[0]+=decrement;
            ray_components[0] = distance[0]*sinCalculation;
            ray_components[1] = distance[0]*cosCalculation;
            last_block_y = current_block_y;
        }
        zeroAngleVars(distance, ray_components);
        fov_bound-=decrement;
    }
    return distances;
}

void draw_player_view (int height, int width, vector<vector<float>>* distances, vector<vector<int>>* img) {
    int color = rgb2hex(100,100,100);
    int othCOl = rgb2hex(128,0,32);
    int reverse_list = size(*distances)-1;
    for (int i=0;i<width;i++) {
        float object_size = (height/((*distances)[reverse_list][0]))*3;
        int start_drawing = (height-object_size)/2;
        float scaling = 64/((start_drawing+object_size)-start_drawing);
        for (int j=0;j<height;j++) {
            if(j>=start_drawing && j<=start_drawing+object_size ){
                (*img)[j][i] = t_brickwall[int(-(scaling*((start_drawing)-j)))][(*distances)[reverse_list][1]];
            } 
            else if (j>=start_drawing) { (*img)[j][i] = othCOl; }
            else if(j<=start_drawing+object_size ){(*img)[j][i] = color;}
        }
        reverse_list-=1;
    }
}

int main(){
    vector<string> filepaths = {"./textures/texture001.png","./textures/ivan.jpg","./textures/rudy.jpg","./textures/quent.jpg"};
    unordered_map<char,vector<vector<int>>> textures = LoadAllTextures(filepaths);

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

    vector<vector<float>> distances = compute_player_view(player_x,player_y,player_angle,window_height,window_width,&framebuffer,&Map);
    save_map(window_height,window_width,&framebuffer);
    draw_player_view(window_height,window_width,&distances,&framebuffer);
    save_map1(window_height,window_width,&framebuffer);

    return 0;
}