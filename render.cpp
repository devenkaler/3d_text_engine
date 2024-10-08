#include "render.h"
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

const int fov = 90;
char screen[300][250];
string screen_color[300][250];
char ascii[71] = {' ','.', '\'', '\\', '`', '^', '"', ',', ':', ';', 'I', 'l', '!', 'i', '>', '<', '~', '+', '_', '-', '?', ']', '[', '}', 
                '{', '1', ')', '(', '|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z', 'X', 'Y', 'U', 'J', 'C', 'L', 'Q',
                 '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 'o', '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$'};
float max_dist = 3;

//get current screen size
class screen_data {
    public:
    int width;
    int height;
    screen_data() {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        height = size.ws_row/1.1;
        width = size.ws_col/1.01;
    }
};

screen_data s;
const int width = s.width;
const int height = s.height;
const float ar = 2.5*height/width;

struct point {
    float x, y, z;
};

struct triangle {
    point a, b, c;
};

struct obj {
    vector<triangle> tris;
    point origin;
    string color;
};

//clears screen array (not terminal)
void clearScreen() {
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            screen[y][x] = ascii[0];
        }
    }
}

//prints to stdout
void prntScreen() {
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            if (y == 0 || x == 0 || y == height-1 || x == width-1) {
                cout << "\033[0m.";
            } else {
                cout << screen_color[y][x] << screen[y][x];
            }
        }
        cout << endl;
    }
    clearScreen();
}

//projects point in 3d plane to 2d plane (origin around x, y plane)
bool translatePoint(point *p) {
    if (p->z < max_dist && p->z > 0.01) {
        float f = 1/tan(fov/2);
        p->x = p->x*ar*f/p->z;
        p->x = (int)round((p->x+1)*(width-1)/2);
        p->y = p->y*f/p->z;
        p->y = (int)round((-p->y+1)*(height-1)/2);
        return true;
    }
    return false;
}

int getAsciiPos(char c) {
    int ascii_len = sizeof(ascii);
    for (int i = 0; i < ascii_len; i++) {
        if (ascii[i] == c) {
            return i;
        }
    }
    return -1;
}

void drawLine(point a, point b, string color) {
    int h = (int)height;
    int w = (int)width;
    int y_err = 1;
    float err = 0;
    float dx, dy, dz, z, m;
    int x, y;
    int ascii_len = sizeof(ascii)-1;
    char ascii_val;

    if (a.x < b.x) {
        x = a.x;
        y = a.y;
        z = a.z;
        dx = b.x - x;
        dy = b.y - y;
        dz = b.z - z;
    } else {
        x = b.x;
        y = b.y;
        z = b.z;
        dx = a.x - x;
        dy = a.y - y;
        dz = a.z - z;
    }

    if (dy < 0) {
            y_err = -1;
        }
    
    if (abs(dy) > dx) {
        m = abs(dx / dy);
        for (int i = 0; i < abs(dy); i++) {
            ascii_val = ascii[(int)(ascii_len*(1-z/max_dist))];
            if (err > 1) {
                err -= 1;
                x += 1;
            }
            if (y+(y_err*i) > 0 && y+(y_err*i) < h && x > 0 && x < w &&
                getAsciiPos(screen[y+(y_err*i)][x]) < getAsciiPos(ascii_val)) {
                screen[y+(y_err*i)][x] = ascii_val;
                screen_color[y+(y_err*i)][x] = color;
            }
            err += m;
            z += dz/abs(dy);
        }
        
    } else if (dx != 0) {
        m = abs(dy / dx);
        for (int i = 0; i < dx; i++) {
            ascii_val = ascii[(int)(ascii_len*(1-z/max_dist))];
            if (err > 1) {
                err -= 1;
                y += y_err;
            }
            if (y > 0 && y <= h && x+i > 0 && x+i < w &&
                getAsciiPos(screen[y][x+i]) < getAsciiPos(ascii_val)) {
                screen[y][x+i] = ascii_val;
                screen_color[y][x+1] = color;
            }
            err += m;
            z += dz/dx;
        }
    }

}

float areaTri(float x1, float y1, float x2, float y2, float x3, float y3) {
    return abs((x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/2.0);
}

//fill triangle given 3 points based on 2d plane
void fillTri(point a, point b, point c, string color) {
    int min_x = min(a.x, min(b.x, c.x));
    int max_x = max(a.x, max(b.x, c.x));
    int min_y = min(a.y, min(b.y, c.y));
    int max_y = max(a.y, max(b.y, c.y));
    float z;
    float ascii_len = sizeof(ascii)-1;
    float ab_area, bc_area, ca_area, area;
    char ascii_val;

    for (int y = max(min_y, 0); y < min(max_y, height); y++) {
        for (int x = min(width, min_x-1); x <= max(max_x, 0); x++) {
            area = areaTri(a.x, a.y, b.x, b.y, c.x, c.y);
            ab_area = areaTri(a.x, a.y, b.x, b.y, x, y);
            bc_area = areaTri(b.x, b.y, c.x, c.y, x, y);
            ca_area = areaTri(c.x, c.y, a.x, a.y, x, y);
            if (area == ab_area + bc_area + ca_area && area > 0) {
                z = ((a.z*bc_area) + (b.z*ca_area) + (c.z*ab_area))/area;
                ascii_val = ascii[(int)(ascii_len*(1-z/max_dist))];

                if (y > 0 && y < height && x > 0 && x < width &&
                    getAsciiPos(screen[y][x]) < getAsciiPos(ascii_val)) {
                    screen[y][x] = ascii_val;
                    screen_color[y][x] = color;
                }
            }
        }

    }
}

void drawTri(triangle tri, string color="\033[0m") {
    bool a = translatePoint(&tri.a);
    bool b = translatePoint(&tri.b);
    bool c = translatePoint(&tri.c);
    if (a && b && c) {
        //drawLine(tri.a, tri.b, color);
        //drawLine(tri.b, tri.c, color);
        //drawLine(tri.c, tri.a, color);
        fillTri(tri.a, tri.b, tri.c, color);
    }
}

void renderObj(obj o) {
    for (triangle t: o.tris) {  
            drawTri(t, o.color);
        }
}

void shiftx(point *pnt, float f) {
    pnt->x += f;
}

void shifty(point *pnt, float f) {
    pnt->y += f;
}

void shiftz(point *pnt, float f) {
    pnt->z += f;
}

//around given origin
void rotatex(point *pnt, float theta, point origin) {
    float oy = pnt->y-origin.y;
    float oz = pnt->z-origin.z;
    float ny = oy*cos(theta)-oz*sin(theta);
    float nz = oy*sin(theta)+oz*cos(theta);
    pnt->y = ny+origin.y;
    pnt->z = nz+origin.z;
}

//around given origin
void rotatey(point *pnt, float theta, point origin) {
    float ox = pnt->x-origin.x;
    float oz = pnt->z-origin.z;
    float nx = ox*cos(theta)+oz*sin(theta);
    float nz = oz*cos(theta)-ox*sin(theta);
    pnt->x = nx+origin.x;
    pnt->z = nz+origin.z;    
}

//around given origin
void rotatez(point *pnt, float theta, point origin) {
    float ox = pnt->x-origin.x;
    float oy = pnt->y-origin.y;
    float nx = ox*cos(theta)-oy*sin(theta);
    float ny = ox*sin(theta)+oy*cos(theta);
    pnt->x = nx+origin.x;
    pnt->y = ny+origin.y;
}

//rotate object requires type is x, y, z
void rotateObj(obj *o, float theta, char type) {
    for (triangle &t: o->tris) {
        if (type == 'x') {
            rotatex(&t.a, theta, o->origin);
            rotatex(&t.b, theta, o->origin);
            rotatex(&t.c, theta, o->origin);
        } else if (type == 'y') {
            rotatey(&t.a, theta, o->origin);
            rotatey(&t.b, theta, o->origin);
            rotatey(&t.c, theta, o->origin);
        } else if (type == 'z') {
            rotatez(&t.a, theta, o->origin);
            rotatez(&t.b, theta, o->origin);
            rotatez(&t.c, theta, o->origin);
        }
    }
}

//shifts object given type x, y, z
void shiftObj(obj *o, float f, char type) {
    for (triangle &t: o->tris) {
        if (type == 'x') {
            shiftx(&t.a, f);
            shiftx(&t.b, f);
            shiftx(&t.c, f);
        } else if (type == 'y') {
            shifty(&t.a, f);
            shifty(&t.b, f);
            shifty(&t.c, f);
        } else if (type == 'z') {
            shiftz(&t.a, f);
            shiftz(&t.b, f);
            shiftz(&t.c, f);
        }
    }
}