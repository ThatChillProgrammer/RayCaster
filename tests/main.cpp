#define _USE_MATH_DEFINES

#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>


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
void save_map (int h, int w, vector<vector<int>> *img,string number) {
    string map = "../images/map000.ppm";
    map[13] = number[0];
    map[14] = number[1];
    map[15] = number[2];
    //string map = "map2.ppm";
    ofstream ofs (map, ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h;i++) { 
        for (int j=0;j<w;j++) { vector<u_int8_t> temp = hex2rgb((*img)[i][j]); ofs << temp[0] << temp[1] << temp[2]; }
    }
    ofs.close();
}
void save_map1 (int h, int w, vector<vector<int>> *img) {
    ofstream ofs ("map1.ppm", ios::binary);
    ofs << "P6\n" << h << " " << w << "\n255\n";
    for (int i=0;i<h;i++) { 
        for (int j=0;j<w;j++) { vector<u_int8_t> temp = hex2rgb((*img)[i][j]); ofs << temp[0] << temp[1] << temp[2]; }
    }
    ofs.close();
}
void draw_map (int window_height, int window_width, int map_height, int map_width, vector<vector<int>>* img, vector<string>* map) {
    unordered_map<char,int> colors = {{'0',rgb2hex(255,255,255)},{'1',rgb2hex(0,255,255)},{'2',rgb2hex(0,255,0)},{'3',rgb2hex(0,0,255)},{'4',rgb2hex(255,0,255)}};
    int scaling = window_width/map_height;     
    int white = rgb2hex(255,255,255);
    int blue = rgb2hex(0,0,255);
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
        }
    }
}
vector<vector<float>> draw_entity (float ent_x,float ent_y,float ent_a,int window_height, int window_width,vector<vector<int>>* img,vector<vector<int>>* map) {
    unordered_map<int,int> colors = {{rgb2hex(255,255,255),rgb2hex(124,124,255)},{rgb2hex(0,255,255),rgb2hex(0,255,255)},{rgb2hex(0,255,0),rgb2hex(0,255,0)},{rgb2hex(0,0,255),rgb2hex(0,0,255)},{rgb2hex(255,0,255),rgb2hex(255,0,255)}};
    float fov = 30;
    float COORDINATES_MAX = 64;
    float scaling = window_height/COORDINATES_MAX;
    vector<u_int8_t> white = {255,255,255};
    vector<u_int8_t> blue = {0,0,255};
    int color = rgb2hex(128,128,128);

    int cur_e_x = ent_x*scaling;
    int cur_e_y = ent_y*scaling;
    //cout << cur_e_x << " " << cur_e_y << endl;
    vector<float> distance = {0,0};
    float component_x = 0;
    float component_y = 0;  

    float fov_bound = ent_a+fov;
    int count=0;

    vector<vector<float>> distances;
    for (int i=0;i<window_height;i++){
        for (int j=0;j<window_width;j++){
            count++;
            if (colors[(*map)[(component_x*scaling)+cur_e_y][(component_y*scaling)+cur_e_x]] ) {
                distance[1] = colors[(*img)[(component_x*scaling)+cur_e_y][(component_y*scaling)+cur_e_x]];
                distances.push_back(distance);
                break;
            } 
            (*img)[(component_x*scaling)+cur_e_y][(component_y*scaling)+cur_e_x]=color;
            distance[0]+=0.1;
            component_x = distance[0]*sin(fov_bound * M_PI/180);
            component_y = distance[0]*cos(fov_bound * M_PI/180);
        }
        distance[0]=0;
        component_x=0;
        component_y=0;
        fov_bound-=0.117188;
    }
    return distances;
}
void draw_3d (int height, int width, vector<vector<float>>* distances, vector<vector<int>>* img) {
    int color = rgb2hex(255,0,0);
    int othCOl = rgb2hex(255,255,255);
    int reverse_list = size(*distances)-1;
    //cout << height << " " << (*distances)[reverse_list][0] << " " << reverse_list << endl;
    for (int i=0;i<width;i++) {
        int object_size = (height/(*distances)[reverse_list][0])*3;
        int start_drawing = (height-object_size)/2;
        int stop_drawing = 2;
        //cout << reverse_list << " " << object_size << " " << (*distances)[reverse_list][0] << endl;

        for (int j=0;j<height;j++) {
            if(j>=start_drawing && j<=start_drawing+object_size ){
                (*img)[j][i] = (*distances)[reverse_list][1];
            } else { (*img)[j][i] = othCOl; }
        }
        reverse_list-=1;
    }
}





int main(){

    unordered_map<int,char> int2char = {{1,'1'},{2,'2'},{3,'3'},{4,'4'},{5,'5'},{6,'6'},{7,'7'},{8,'8'},{9,'9'},{0,'0'}};

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
        "0 0000444      0",
        "0              0",
        "1111122222233000"
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
    vector<vector<int>> Map = framebuffer;

    float player_x = 15;
    float player_y = 15;
    float player_angle = 90;
    //vector<vector<float>> distances = draw_entity(player_x,player_y,player_angle,window_height,window_width,&framebuffer,&Map);
    //save_map1(window_height,window_width,&framebuffer);
    //draw_3d(window_height,window_width,&distances,&framebuffer);
    //save_map(window_height,window_width,&framebuffer,"000");



    
    string number = "";
    string pass2func = "";
    vector<vector<int>> distances;
    auto start = chrono::high_resolution_clock::now();
    for (int i=0;i<360;i++) {
        vector<vector<float>> distances = draw_entity(player_x,player_y,player_angle,window_height,window_width,&framebuffer,&Map);
        draw_3d(window_height,window_width,&distances,&framebuffer);
        if (i>=100){number += int2char[(i/100)];}
        if (i>=10){number += int2char[(i/10)%10];}
        if (i>=0){number += int2char[(i%10)];}
        if (number.size()==1){
            pass2func = "00";
            pass2func += number;
        }
        if (number.size()==2){
            pass2func = "0";
            pass2func += number;
        }
        if (number.size()==3){pass2func += number;}
        //save_map(window_height,window_width,&framebuffer,pass2func);
        //cout << number << endl;
        if (player_angle==360){player_angle=0;}
        player_angle +=1;
        number ="";
        pass2func="";
    }   

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << duration.count();
    


    

    return 0;
}