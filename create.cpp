#include "render.h"
#include <fstream>

using namespace std;

obj getObj(string file_name) {
    ifstream inp(file_name);
    obj res;
    res.origin = {0, 0, 1};
    res.color = "\033[31m";

    if (!inp.is_open()) {
        return res;
    }

    string line;
    string int_str = "1234567890.";
    string num_str = "";
    float largest = 0;
    float tx, ty, tz = 0;

    vector<point> vec_all;
    vector<triangle> tri_all;
    while (getline(inp, line)) {
        if (line[0] == 'v') {
            vector<float> vec_point;
            for (int i=0; i<line.size(); i++) {
                if (int_str.find(line[i]) != string::npos)
                    num_str = num_str + line[i];
                else if (num_str.size()) {
                    vec_point.push_back(stof(num_str));
                    num_str = "";
                }
            }
            vec_point.push_back(stof(num_str));
            vec_all.push_back(point {vec_point[0], vec_point[1], vec_point[2]});
            num_str = "";
            largest = max(max(max(vec_point[0], vec_point[1]), vec_point[2]), largest);
        }

        if (line[0] == 'f') {
            vector<point> tri_point;
            for (int i=0; i<line.size(); i++) {
                if (int_str.find(line[i]) != string::npos)
                    num_str = num_str + line[i];
                else if (num_str.size()) {
                    tri_point.push_back(vec_all[stoi(num_str)-1]);
                    num_str = "";
                }
            }
            tri_point.push_back(vec_all[stoi(num_str)-1]);
            tri_all.push_back(triangle {tri_point[0], tri_point[1], tri_point[2]});
            num_str = "";
        }
    }
    
    for (triangle &tr: tri_all) {
        tr.a.x = tr.a.x/largest;
        tr.b.x = tr.b.x/largest;
        tr.c.x = tr.c.x/largest;

        tr.a.y = tr.a.y/largest;
        tr.b.y = tr.b.y/largest;
        tr.c.y = tr.c.y/largest;

        tr.a.z = tr.a.z/largest;
        tr.b.z = tr.b.z/largest;
        tr.c.z = tr.c.z/largest;
    }
    res.tris = tri_all;
    return res;
}

int main() {

    obj o1 = getObj("donut.obj");
    shiftObj(&o1, -0.75, 'y');
    shiftObj(&o1, -0.75, 'x');
    shiftObj(&o1, 0.9, 'z');
    
    for (int i=0; i < 1000; i++) {
        rotateObj(&o1, 0.05, 'y');
        rotateObj(&o1, 0.05, 'x');
        rotateObj(&o1, -0.02, 'z');
        renderObj(o1);
        system("clear");
        prntScreen();
        this_thread::sleep_for(chrono::milliseconds(45));
    }

    return 0;
}