//
//  setLight.hpp
//  Proj4
//


#ifndef setLight_hpp
#define setLight_hpp

#include <stdio.h>


void SetMaterial( float r, float g, float b,  float shininess );

void SetPointLight( int ilight, float x, float y, float z,  float r, float g, float b );

void SetSpotLight(
                  int ilight,
                  float x, float y, float z,
                  float xdir, float ydir, float zdir,
                  float ang,
                  float r, float g, float b
                  );

float *Array3( float a, float b, float c );

float *Array4( float a, float b, float c, float d );

float *BlendArray3( float factor, float array0[3], float array1[3] );

float *MulArray3( float factor, float array0[3] );

#endif /* setLight_hpp */
