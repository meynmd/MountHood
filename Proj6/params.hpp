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

const char PATH_XTR[] = "meyn1024.xtr";

const char MODELPATH_HELI[] = "heliBody_mesh.obj";
const char MODELPATH_BLADES[] = "heliBlades_mesh.obj";
const char MODELPATH_TREE[] = "pine.obj";

char TEXPATH_HELI[] = "heli_d.bmp";
char TEXPATH_SNOW[] = "snow3_d.bmp";
char TEXPATH_GROUND[] = "dirt_d.bmp";
char TEXPATH_TREE[] = "myLeaves.bmp";



// animation params

int     MS_PER_CYCLE = 300000;

float   HeliInitAlt = 300.f;
int     HeliAnimInitRevs = 10;
int     HeliAnimRevs = 10;
int     HeliBladeRevsFac = 800;
float   HeliAnimRadius = 2500.f;
float   HeliAnimRadiusMin = 1000.f;
float   HeliAnimRadiusMax = 3000.f;

const float   CAM_FOLLOW_DIST = 15.f;
const float   DEFAULT_FOLLOW_SCALE = 1.;
const int     HELI_LAG_FRAMES = 20;

const float   HELI_CLIMB_FAC = 0.05;
const float   HELI_CLIMB_MAX = 100.;
const float   HELI_TURN_FAC = 0.025;
const float   HELI_TURN_MAX = 125.;

const float   TILT_X_FAC = 5.;
const float   TILT_Y_FAC = 5.;
const float   TILT_Z_FAC = 3.5;
const float   TILT_Z_AUTO_MAX = 10.;

// landscape parameters

const int LANDSCAPE_RES = 2;

const int TEX_TILES = 2;

const float ELEV_BASE = -1500.;

const float LANDSCAPE_XSCALE = 10.56;
const float LANDSCAPE_ZSCALE = 15.;
const float LANDSCAPE_YSCALE = 0.4;

const float TREE_SCALE = 1.;

const float MIN_SNOW_LEVEL = 10.;

bool UseNormals = true;


// landscape "clutter"

const int NUM_TREES = 1000;
const float TREE_ELEV_MAX = 500.;


// global light parameters

float   AmbR = 0.3, AmbG = 0.3, AmbB = 0.3;

// Light 0 parameters

float   L0x = 0.,
        L0y = 5000.,
        L0z = 500000.;

float   L0r = 0.425,
        L0g = 0.475,
        L0b = 0.475;


// Light 1 parameters

float   L1x = 0.,
        L1y = 5000.,
        L1z = -500000.;

float   L1r = 0.375,
        L1g = 0.425,
        L1b = 0.45;


// Light 2 parameters

float   L2x = 500000.,
        L2y = 5000.,
        L2z = 0.;

float   L2r = 0.4,
        L2g = 0.4,
        L2b = 0.4;


// Light 3 parameters

float   L3x = -500000.,
        L3y = 5000.,
        L3z = 0.;

float   L3r = 0.375,
        L3g = 0.375,
        L3b = 0.35;


// Light 4 parameters

float   L4x = 0.,
        L4y = 5000.,
        L4z = 0.;

float   L4r = 0.35,
        L4g = 0.35,
        L4b = 0.35;

#endif
