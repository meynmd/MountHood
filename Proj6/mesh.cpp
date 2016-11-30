//
//  loader.cpp
//  Proj2
//
//  Created by Matthew Meyn on 10/10/16.
//  Copyright © 2016 Matthew Meyn. All rights reserved.
//

#include "mesh.hpp"
#include <cstdlib>

bool Mesh::LoadPoints(){
    if(!objFile) {
        return false;
    }
    
    // parse through file from beginning to find vertices
    
    fseek( objFile, 0, SEEK_SET );
    size_t bufSize = 256;
    char* buffer = new char[bufSize];
    size_t charsRead = 0;
    bool ok = true;
    
    while( (charsRead = getline( &buffer, &bufSize, objFile ) ) ) {
        char* tok = strtok( buffer, " " );
        if(!tok) {
            break;
        }
        
        // find out what info is on this line
        
        if( strcmp( tok, "v" ) == 0 ) {
            
            // vertex location: get vertex coords
            
            point p;
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                p.x = atof( tok );
            } else ok = false;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                p.y = atof( tok );
            } else ok = false;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                p.z = atof( tok );
            } else ok = false;
            
            // add this point to points
            
            if(ok) {
                points.push_back( p );
            } else {
                fprintf(stderr, "Vertex with fewer than 3 components!\n");
            }
        }
        else if(strcmp(tok, "vt") == 0) {
            
            // texture coords
            
            texCoord tc;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                tc.s = atof( tok );
            } else ok = false;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                tc.t = atof( tok );
            } else ok = false;
            
            // add coords to tex coords
            
            if(ok) {
                texCoords.push_back(tc);
            } else {
                fprintf(stderr, "Invalid texture coordinates!\n");
            }
        }
        else if(strcmp(tok, "vn") == 0) {
            
            // normal coords
            
            normal n;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                n.x = atof( tok );
            } else ok = false;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                n.y = atof( tok );
            } else ok = false;
            
            if( ( tok = strtok( NULL, " " ) ) != NULL ) {
                n.z = atof( tok );
            } else ok = false;
            
            if(ok) {
                normals.push_back(n);
            } else {
                fprintf(stderr, "Invalid normal coordinates!\n");
            }
        }
    }
    if(ok) {
        return true;
    } else {
        return false;
    }
}

bool Mesh::LoadPolys() {
    if(!objFile) {
        return false;
    }
    
    // parse through file from beginning to find polys
    fseek( objFile, 0, SEEK_SET );
    size_t bufSize = 256;
    char* buffer = new char[bufSize];
    size_t charsRead = 0;
    bool ok = true;
    
    while( (charsRead = getline( &buffer, &bufSize, objFile ) ) ) {
        tri t;
        char* lineCtxt;
        char* tok = strtok_r(buffer, " ", &lineCtxt); //strtok( buffer, " " );
        
        if(!tok) {
            break;
        }
        
        // if it's not a tri, move on...
        if( strcmp( tok, "f" ) != 0 ) {
            continue;
        }
        
        // get tri points
        char* pointCtxt;
        // point 0
        if( ( tok = strtok_r( NULL, " ", &lineCtxt ) ) != NULL ) {
            // get point index
            char* vTok = strtok_r(tok, "/", &pointCtxt);
            t.p0 = atoi( vTok ) - 1;
            
            // get texture coord index;
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.tc0= atoi( vTok );
            
            // get normal index
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.n0 = atoi( vTok );
            
        } else ok = false;
        
        // point 1
        if( ( tok = strtok_r( NULL, " ", &lineCtxt ) ) != NULL ) {
            // get point index
            char* vTok = strtok_r(tok, "/", &pointCtxt);
            t.p1 = atoi( vTok ) - 1;
            
            // get texture coord index;
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.tc1= atoi( vTok );
            
            // get normal index
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.n1 = atoi( vTok );
            
        } else ok = false;
        
        // point 2
        if( ( tok = strtok_r( NULL, " ", &lineCtxt ) ) != NULL ) {
            // get point index
            char* vTok = strtok_r(tok, "/", &pointCtxt);
            t.p2 = atoi( vTok ) - 1;
            
            // get texture coord index;
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.tc2= atoi( vTok );
            
            // get normal index
            vTok = strtok_r(NULL, "/", &pointCtxt);
            t.n2 = atoi( vTok );
            
        } else ok = false;
        
        // add this tri to tris
        if(ok) {
            tris.push_back( t );
        } else {
            fprintf(stderr, "Tri with fewer than 3 points!\n");
        }
    }
    
    printf("\n\n\n");
    
    if(ok) {
        return true;
    } else return false;
}


Mesh::Mesh() {
    objFile = nullptr;
    objPath = nullptr;
}

bool Mesh::Load(const char path[]) {
    objFile = fopen( path, "r" );
    bool ok = true;
    
    if(!objFile) {
        return false;
    }
    
    ok = LoadPoints();
    if( !LoadPolys() ) {
        ok = false;
    }

    if(ok) {
        printf( "Loaded OBJ file %s.\n", path );
    } else {
        printf( "Failed to load OBJ file.\n" );
    }
    return ok;
}

int Mesh::GetNumPolys() {
    return tris.size();
}

std::vector<tri> Mesh::GetPolys() {
    return tris;
}

std::vector<point> Mesh::GetPoints() {
    return points;
}

std::vector<normal> Mesh::GetNormals() {
    return normals;
}

std::vector<texCoord> Mesh::GetTexCoords() {
    return texCoords;
}
