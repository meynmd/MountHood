//
//  array.cpp
//  Proj2
//
//  Created by Matthew Meyn on 10/23/16.
//  Copyright © 2016 Matthew Meyn. All rights reserved.
//

#include "array.hpp"

// utility to create an array from 3 separate values:

float *
Array3( float a, float b, float c ) {
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}


// utility to create an array from a multiplier and an array:

float *
MulArray3( float factor, float array0[3] ) {
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}
