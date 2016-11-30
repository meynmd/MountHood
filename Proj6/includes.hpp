//
//  includes.hpp
//  Proj6
//
//  Created by Matthew Meyn on 11/13/16.
//

#ifndef includes_h
#define includes_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "GLUT/glut.h"
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#endif

#endif /* includes_h */
