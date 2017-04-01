// ============================================
// The header file is for module "city landscape"
// This module is provdied in C style for new programmers in C
// 2014 Gatech ECE2035
//=============================================
#ifndef CITY_LANDSCAPE_PRIVATE_H
#define CITY_LANDSCAPE_PRIVATE_H

#include "mbed.h"
#include "globals.h"
#include "city_landscape_public.h"

//==== [private type] ====
// N/A


//==== [private function] ====
// N/A


//==== [private macros] ====
// The bottom of the screen => y=127
// Gut the landscape grow up from the bottom of the screen. It is awkward.
// Thus, we use a macro to reverse the coordinate for convenience.
#define REVERSE_Y(x) (SIZE_Y-(x))

//==== [private settings] ====
// You could modify these settings, but try to keep them be used only inside city_landscape.cpp
// Here are the settings to define the looking of your city landscape
#define CITY_TO_SCREEN_MARGIN 25 // pixel on the screen
#define CITY_WIDTH 10 // pixel on the screen
#define BUILDING_WIDTH 2 // pixel on the screen
#define NUM_BUILDING (CITY_WIDTH/BUILDING_WIDTH)
#define BUILDING_COLOR 0x00FF00
#define LANDSCAPE_COLOR 0xCCAA00





#endif //CITY_LANDSCAPE_PRIVATE_H