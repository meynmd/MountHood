//
//  xtrRead.hpp
//  FinalProject
//
//  Created by Matthew Meyn on 11/24/16.
//

#ifndef xtrRead_hpp
#define xtrRead_hpp

#include <stdio.h>

float* ReadXtr(int* OutNumLong, int* OutNumLat,
               float* OutHeightMin, float* OutHeightMax, FILE* XtrFile);

#endif /* xtrRead_hpp */
