#include "render.h"

using namespace std;


int main() {
    obj cube = {{triangle {point {0, 0, 1}, point {0, 1, 1}, point {1, 0, 1}}, triangle {point {1, 1, 1}, point {0, 1, 1}, point {1, 0, 1}},
                          triangle {point {0, 0, 2}, point {0, 1, 2}, point {1, 0, 2}}, triangle {point {1, 1, 2}, point {0, 1, 2}, point {1, 0, 2}},
                          triangle {point {0, 0, 1}, point {0, 0, 2}, point {0, 1, 1}}, triangle {point {0, 0, 2}, point {0, 1, 1}, point {0, 1, 2}},
                          triangle {point {1, 0, 1}, point {1, 0, 2}, point {1, 1, 1}}, triangle {point {1, 0, 2}, point {1, 1, 1}, point {1, 1, 2}},
                          triangle {point {0, 0, 1}, point {0, 0, 2}, point {1, 0, 1}}, triangle {point {0, 0, 2}, point {1, 0, 1}, point {1, 0, 2}},
                          triangle {point {0, 1, 1}, point {0, 1, 2}, point {1, 1, 1}}, triangle {point {0, 1, 2}, point {1, 1, 1}, point {1, 1, 2}}},
                          
                          {0.5, 0.5, 1.5},
                          
                          "\033[31m"};

    obj cube2 = cube;
    cube2.color = "\033[33m";

    for (int i=0; i < 1000; i++) {
        rotateObj(&cube, 0.01, 'y');
        rotateObj(&cube, 0.01, 'x');
        rotateObj(&cube2, -0.01, 'x');
        rotateObj(&cube2, -0.01, 'y');
        shiftObj(&cube2, -0.01, 'z');
        shiftObj(&cube2, -0.01, 'y');
        renderObj(cube);
        renderObj(cube2);
        system("clear");
        prntScreen();
        this_thread::sleep_for(chrono::milliseconds(50));
        
        
    }

    return 0;
}