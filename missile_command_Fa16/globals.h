// ============================================
// The header file for general settings for the project
// Fall 2016 Gatech ECE2035
//=============================================
#ifndef GLOBAL_H
#define GLOBAL_H

#include "uLCD_4DGL.h"
#include "SDFileSystem.h"

// === [global object] ===
extern uLCD_4DGL uLCD;
extern SDFileSystem sd;


// === [global settings] ===
#define BACKGROUND_COLOR 0x000000 //black
#define LANDSCAPE_HEIGHT 4        // number of pixel on the screen
#define MAX_BUILDING_HEIGHT 10    // number of pixel on the screen


// === [define the macro of error heandle function] ===
// when the condition (c) is not true, assert the program and show error code
#define ASSERT_P(c,e) do { \
    if(!(c)){ \
        uLCD.printf("\nERROR:%d\n",e); \
        while(1); \
    } \
} while (0)


// === [error code] ===
#define ERROR_MISSILE_INDEX_GET_INFO      -1  // make sure your code give the valid index for missile_get_info()
#define ERROR_MISSILE_INDEX_UPDATE_STATUS -2  // make sure your code give the valid index for missile_update_status()
#define ERROR_MISSILE_SPEED               -3  // missile speed has to be between 1 and 8
#define ERROR_MISSILE_INTERVAL            -4  // missile interval has to be between 1 and 100
// other missile error code ...
#define ERROR_CITY_NUMBER                 -11 // num_city in city_landscape_init() is larger than MAX_NUM_CITY
#define ERROR_CITY_INDEX_GET_INFO         -12 // make sure your code give the valid index for city_get_info()
#define ERROR_CITY_INDEX_DESTROY          -13 // make sure your code give the valid index for city_destory()
// other anti-missile error code ...

#endif //GLOBAL_H