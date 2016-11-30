//
//  xtrRead.cpp
//  FinalProject
//
//  Created by Matthew Meyn on 11/24/16.
//  adapted from Prof. Mike Bailey's XTR file code
//

#include "xtrRead.hpp"
#include <cstring>

float* ReadXtr(int* OutNumLong, int* OutNumLat,
               float* OutHeightMin, float* OutHeightMax, FILE* XtrFile) {
    
    if(!XtrFile) {
        return nullptr;
    }
    
    const char FORM_FEED =	0x0c;
    
    float MinLng, MaxLng,  MinLat, MaxLat;
    
    fscanf( XtrFile, "%f %f %d", &MinLng, &MaxLng, OutNumLong );
    fscanf( XtrFile, "%f %f %d", &MinLat, &MaxLat, OutNumLat );
    
    int c;
    do {
        c = fgetc( XtrFile );
    } while( c != FORM_FEED );
    
    c = fgetc( XtrFile );			// the '\n'
    if( c != '\n' ) {
        fprintf( stderr, "Should have found a '\\n' -- but found '%c'\n", c );
    }
    
    int numLong = *OutNumLong, numLat = *OutNumLat;
    
    float* elevations = new float [ (numLong) * (numLat) ];
    memset(elevations, 0, (numLong) * (numLat) * sizeof(float));
    
    for( int lat = 0; lat < numLat; lat++ )
    {
        int offset = lat * (numLong);
        fread( (void *)&elevations[offset], sizeof(float), numLong, XtrFile );
        
        // find min and max elevations
        
        for(int lng = 0; lng < numLong; lng++) {
            
            if(elevations[offset + lng] < *OutHeightMin) {
                *OutHeightMin = elevations[offset + lng];
            }
            
            if(elevations[offset + lng] > *OutHeightMax) {
                *OutHeightMax = elevations[offset + lng];
            }
        }
    }
    
    printf("Read XTR file.\nMin Lat, Long: (%f, %f)\nMax Lat, Long: (%f, %f)\n",
           MinLat, MinLng, MaxLat, MaxLng);
    printf("Min Height: %f\nMax Height: %f\n\n", *OutHeightMin, *OutHeightMax);
    
    return elevations;
}
