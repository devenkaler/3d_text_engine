# 3d_text_engine

Displays 3d object (obj) which is made of multiple triangles which are defined as 3 points in 3d space. 

EX: obj m = {triangle {point {0, 0, 1}, point {0, 1, 1}, point {1, 0, 1}}, triangle {point {1, 1, 1}, point {0, 1, 1}, point {1, 0, 1}},
                          triangle {point {0, 0, 2}, point {0, 1, 2}, point {1, 0, 2}}, triangle {point {1, 1, 2}, point {0, 1, 2}, point {1, 0, 2}},
                          triangle {point {0, 0, 1}, point {0, 0, 2}, point {0, 1, 1}}, triangle {point {0, 0, 2}, point {0, 1, 1}, point {0, 1, 2}},
                          triangle {point {1, 0, 1}, point {1, 0, 2}, point {1, 1, 1}}, triangle {point {1, 0, 2}, point {1, 1, 1}, point {1, 1, 2}},
                          triangle {point {0, 0, 1}, point {0, 0, 2}, point {1, 0, 1}}, triangle {point {0, 0, 2}, point {1, 0, 1}, point {1, 0, 2}},
                          triangle {point {0, 1, 1}, point {0, 1, 2}, point {1, 1, 1}}, triangle {point {0, 1, 2}, point {1, 1, 1}, point {1, 1, 2}}},
                          
                          point {0.5, 0.5, 1.5}}

This would display a cube, which has an origin of {0.5, 0.5, 1.5}. The object can be moved and rotated in relation to the origin. Multiple Objects can be created

Currently only supports Linux 
