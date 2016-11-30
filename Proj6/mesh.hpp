//
//  loader.hpp
//  Proj2
//
//  Created by Matthew Meyn on 10/10/16.
//  Copyright © 2016 Matthew Meyn. All rights reserved.
//


#ifndef mesh_hpp
#define mesh_hpp


#include <stdio.h>
#include <vector>


struct point {
    float x, y, z,
    r, g, b;
};


typedef point normal;


struct texCoord {
    float s, t;
};


struct edge {
    int p0, p1;
};


struct tri {
    int p0, p1, p2;
    int n0, n1, n2;
    int tc0, tc1, tc2;
};


class Mesh {
private:
    FILE* objFile;
    char* objPath;
    std::vector<point> points;
    std::vector<normal> normals;
    std::vector<texCoord> texCoords;
    std::vector<edge> edges;
    std::vector<tri> tris;
    
    bool LoadPoints();
    bool LoadPolys();
    
public:
    Mesh();
    
    bool Load(const char path[]);
    int GetNumPolys();
    std::vector<tri> GetPolys();
    std::vector<point> GetPoints();
    std::vector<normal> GetNormals();
    std::vector<texCoord> GetTexCoords();
};



#endif
