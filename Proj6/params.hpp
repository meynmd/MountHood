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

const int INIT_WINDOW_SIZE_W = { 1600 };
const int INIT_WINDOW_SIZE_H = { 900 };


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

const char PATH_XTR[] = "meyn1200.xtr";

const char MODELPATH_HELI[] = "heliBody_mesh.obj";
const char MODELPATH_BLADES[] = "heliBlades_mesh.obj";
const char MODELPATH_TREE[] = "pine_mesh.obj";

char TEXPATH_HELI[] = "heli_d.bmp";
char TEXPATH_SNOW[] = "snow3_d.bmp";
char TEXPATH_ICE[] = "ice_d.bmp";
char TEXPATH_GROUND[] = "dirt_d.bmp";
char TEXPATH_TREE[] = "pine_d.bmp";



// animation params

int     MS_PER_CYCLE = 300000;

float   HeliInitAlt = 300.f;
int     HeliAnimInitRevs = 10;
int     HeliAnimRevs = 10;
int     HeliBladeRevsFac = 800;
float   HeliAnimRadius = 2500.f;
float   HeliAnimRadiusMin = 300.f;
float   HeliAnimRadiusMax = 4000.f;

const float   CAM_FOLLOW_DIST = 15.f;
const float   DEFAULT_FOLLOW_SCALE = 1.;
const int     HELI_LAG_FRAMES = 20;

//const float   HELI_CLIMB_FAC = 0.05;
const float   HELI_CLIMB_FAC = 0.015;
const float   HELI_CLIMB_MAX = 100.;
const float   HELI_TURN_FAC = 0.025;
const float   HELI_TURN_MAX = 125.;

const float   TILT_X_FAC = 5.;
//const float   TILT_Y_FAC = 7.;
//const float   TILT_Z_FAC = 3.5;
const float   TILT_Y_FAC = 10.;
const float   TILT_Z_FAC = 2.;
const float   TILT_Z_AUTO_MAX = 20.;

// landscape parameters

const float WATER_LEVEL = -575.;

const int LANDSCAPE_RES = 1;

const int TEX_TILES = 32;

const float ELEV_BASE = -1500.;

const float LANDSCAPE_XSCALE = 10.56;
const float LANDSCAPE_ZSCALE = 15.;
const float LANDSCAPE_YSCALE = 0.4;

const float TREE_SCALE = 1.;

const float MIN_SNOW_LEVEL = 400.;

bool UseNormals = true;


// landscape "clutter"

const int NUM_TREES = 3000;
const float TREE_ELEV_MAX = 500.;
const float TREE_ELEV_MIN = -500.;

const float TREE_DELTA_ELEV_MAX = 1.;


// global light parameters

float   AmbR = 0.4, AmbG = 0.5, AmbB = 0.6;

// Light 0 parameters (out in positive z)

float   L0x = 0.,
        L0y = 5000.,
        L0z = 500000.;

float   L0r = 0.8,
        L0g = 0.8,
        L0b = 0.8;


// Light 1 parameters (negative z)

float   L1x = 0.,
        L1y = 5000.,
        L1z = -500000.;

float   L1r = 0.65,
        L1g = 0.75,
        L1b = 0.8;


// Light 2 parameters (positive x)

float   L2x = 500000.,
        L2y = 5000.,
        L2z = 0.;

float   L2r = 0.7,
        L2g = 0.7,
        L2b = 0.65;


// Light 3 parameters (negative x)

float   L3x = -500000.,
        L3y = 5000.,
        L3z = 0.;

float   L3r = 0.525,
        L3g = 0.525,
        L3b = 0.5;


// Light 4 parameters

float   L4x = 0.,
        L4y = 1000.,
        L4z = 0.;

float   L4r = 0.75,
        L4g = 0.75,
        L4b = 0.75;

#endif
