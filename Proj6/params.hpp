//
//  params.hpp
//  Proj6
//
//  Created by Matthew Meyn on 11/13/16.
//  Copyright © 2016 Matthew Meyn. All rights reserved.
//

#ifndef PARAMS_H
#define PARAMS_H

#include <stdio.h>


// title of these windows:

const char *WINDOWTITLE = { "Final Project -- Matthew Meyn" };
const char *GLUITITLE   = { "User Interface Window" };


// initial window size:

const int INIT_WINDOW_SIZE = { 900 };


// minimum allowable scale factor:

const float MINSCALE = { 0.005f };


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0.5, 0.6, 0.9, 1. };


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// file paths

const char PATH_XTR[] = "meyn1024.xtr";

const char MODELPATH_HELI[] = "heliBody_mesh.obj";
const char MODELPATH_BLADES[] = "heliBlades_mesh.obj";
const char MODELPATH_TREE[] = "pine4.obj";

char TEXPATH_HELI[] = "heli_d.bmp";
char TEXPATH_GROUND[] = "snow3_d.bmp";
char TEXPATH_TREE[] = "pine_d.bmp";



// animation params

int     MS_PER_CYCLE = 60000;

float   HeliInitAlt = 300.f;
int     HeliAnimRevs = 2;
float   HeliAnimRadius = 2500.f;

float   CAM_FOLLOW_DIST = 15.f;
float   DEFAULT_FOLLOW_SCALE = 1.;
float   HELI_LAG_FRAMES = 20.;
float   HELI_ALT_INC = 4.;

// landscape parameters

const int LANDSCAPE_RES = 1;

const int TEX_TILE_FAC = 4;

const float ELEV_BASE = -1000.;
const float LANDSCAPE_XZSCALE = 15.;
const float LANDSCAPE_YSCALE = 0.4;

const float TREE_SCALE = 0.25;



// landscape "clutter"

const int NUM_TREES = 1000;



// global light parameters

float   AmbR = 0.8, AmbG = 0.9, AmbB = 0.85;

// Light 0 parameters

float   L0x = -60.,
        L0y = 5000.,
        L0z = 60.;

float   L0r = 0.7,
        L0g = 0.7,
        L0b = 0.8;






#endif
