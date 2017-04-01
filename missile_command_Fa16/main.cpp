// External libs
#include "mbed.h"
#include "wave_player.h"
#include "MMA8452.h"

// Projet includes
#include "globals.h"
#include "city_landscape_public.h"
#include "missile_public.h"
#include "player_public.h"
#include "testbench.h"

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))

// Helper function declarations
void playSound(char* wav);

void checkMissileCollision(int& playerScore, double threshold);
void checkCityCollision(int city_num);
void checkPlayerCollision();
void explosion(int x, int y);
void explosion2(int x, int y);

double distance(double x1, double x2, double y1, double y2);
void startLevel(int& missile_speed, int& missile_interval, int playerLevel, int city_num, double& threshold);
int checkLevel(int playerScore);

// Console output
Serial pc(USBTX,USBRX);

// Accelerometer
MMA8452 accel(p28, p27, 100000);

// Control buttons
DigitalIn left_pb(p21);
DigitalIn right_pb(p22); 
DigitalIn fire_pb(p23);
DigitalIn pause_pb(p24);

// Screen
uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;

// Speaker
AnalogOut DACout(p18);
PwmOut speaker(p25);
wave_player waver(&DACout);

// SD Card
SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs

// ===User implementations start===
int main()
{
    // Replace <outfile> with your student ID
    test_dlinkedlist(
        "/sd/tests/dll_test.txt",
        "/sd/903096443.txt"
    );

    // Test the speaker
    playSound("/sd/wavfiles/BUZZER.wav");
    
    //Initialize hardware buttons
    left_pb.mode(PullUp); 
    right_pb.mode(PullUp);
    fire_pb.mode(PullUp);
    pause_pb.mode(PullUp);
    
    
    // ===User implementations start===    
    // Initialization goes here
    // Initialize variables
    int city_num = 4;
    int missile_speed;
    int missile_interval;
    double threshold; // intercept radius
    enum button_t {LEFT, RIGHT, FIRE, PAUSE, IDLE};
    enum button_t button = IDLE;
    
    // Initialize player data
    int playerScore = 0;
    int playerHighScore = 0;
    int playerLevel = -1;
    
    // Initialize accelerometer
    accel.activate();
    double xG;
    double yG;
    double zG;
    
    //********************************************
    // Main game menu loop
    //********************************************
MainGameMenu:
    uLCD.cls();
    bool flag = 1;
    while(flag)
    {
        // Display game title
        uLCD.color(GREEN);
        uLCD.rectangle(1, 30, 125, 42, 0x00FF00);
        uLCD.locate(1,4);
        uLCD.printf("Missile Command");
        // Display options
        uLCD.locate(1,6);
        uLCD.printf("1. Play Game");
        uLCD.locate(1,8);
        uLCD.printf("2. Instructions");
        uLCD.locate(1,10);
        uLCD.printf("3. Level Select");
        uLCD.locate(1,12);
        uLCD.printf("4. Bloopers");
        // User selection
        if (left_pb == 0) {
            button = LEFT;
        } else if (right_pb == 0) {
            button = RIGHT;
        } else if (fire_pb == 0) {
            button = FIRE;
        } else if (pause_pb == 0) {
            button = PAUSE;
        } else {
            button = IDLE;
        }
        // Check what the user selected
        switch (button) {
        case LEFT :
            // Play game
            playSound("/sd/wavfiles/beep.wav");
            flag = 0;
            break;
        case RIGHT :
            // Give user instructions
            // Go back to main menu if button is pressed
            playSound("/sd/wavfiles/beep.wav");
            uLCD.cls();
            uLCD.locate(0,0);
            uLCD.printf("Protect Cities!");
            uLCD.locate(0,1);
            uLCD.printf("Protect Yourself!");
            uLCD.locate(0,2);
            uLCD.printf("Fight for Glory!");
            uLCD.locate(0,4);
            uLCD.printf("Controls:");
            uLCD.locate(0,5);
            uLCD.printf("PB1: Move Left");
            uLCD.locate(0,6);
            uLCD.printf("PB2: Move Right");
            uLCD.locate(0,7);
            uLCD.printf("PB3: Fire");
            uLCD.locate(0,8);
            uLCD.printf("PB4: Pause");
            while(1) {
                // check for stop condition
                if (left_pb == 0 || right_pb == 0 || fire_pb == 0 || pause_pb == 0) {
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    break;
                }
            }
            break;
        case FIRE :
            playSound("/sd/wavfiles/beep.wav");
            uLCD.cls();
            // prompt user for level difficulty
            uLCD.locate(0,0);
            uLCD.printf("Select Level:");
            uLCD.locate(0,2);
            uLCD.printf("Push Button");
            uLCD.locate(0,3);
            uLCD.printf("to Select");
            uLCD.locate(0,5);
            uLCD.printf("1. Level 1");
            uLCD.locate(0,7);
            uLCD.printf("2. Level 2");
            uLCD.locate(0,9);
            uLCD.printf("3. Level 3");
            uLCD.locate(0,11);
            uLCD.printf("4. Level 4");
            // initialize starting score based on user input
            while (1) {
                if (left_pb == 0) {
                    playerScore = 0;
                    playerLevel = -1;
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    flag = 0;
                    break;
                } else if (right_pb == 0) {
                    playerScore = 10;
                    playerLevel = 0;
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    flag = 0;
                    break;
                } else if (fire_pb == 0) {
                    playerScore = 20;
                    playerLevel = 1;
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    flag = 0;
                    break;
                } else if (pause_pb == 0) {
                    playerScore = 30;
                    playerLevel = 2;
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    flag = 0;
                    break;
                }
            }
            break;
        case PAUSE :
        playSound("/sd/wavfiles/beep.wav");
        //Bouncing Ball Demo
            float fx=50.0,fy=21.0,vx=1.0,vy=0.4;
            int x=50,y=21,radius=4;
            uLCD.background_color(BLACK);
            uLCD.cls();
            //draw walls
            uLCD.line(0, 0, 127, 0, WHITE);
            uLCD.line(127, 0, 127, 127, WHITE);
            uLCD.line(127, 127, 0, 127, WHITE);
            uLCD.line(0, 127, 0, 0, WHITE);
            for (int i=0; i<1500; i++) {
                //draw ball
                uLCD.filled_circle(x, y, radius, RED);
                //bounce off edge walls and slow down a bit?
                if ((x<=radius+1) || (x>=126-radius)) vx = -.90*vx;
                if ((y<=radius+1) || (y>=126-radius)) vy = -.90*vy;
                //erase old ball location
                uLCD.filled_circle(x, y, radius, BLACK);
                //move ball
                fx=fx+vx;
                fy=fy+vy;
                x=(int)fx;
                y=(int)fy;
                if (left_pb == 0 || right_pb == 0 || fire_pb == 0 || pause_pb == 0) {
                    uLCD.cls();
                    playSound("/sd/wavfiles/beep.wav");
                    break;
                }
            }
        default :
            break;
    }
}
    //*********************************************
    // Main game loop
    //*********************************************
    // Reset button to idle
    button = IDLE;
    while(1)
    {
        // 0. Check if player should advance to next level
        if (playerLevel != checkLevel(playerScore)) {
            playerLevel++;
            startLevel(missile_speed, missile_interval, playerLevel, city_num, threshold);
        }
        // 1. Update missiles
        player_missile_draw();
        missile_generator();
        
        // 2. Read input
        // Check push button and accelerometer inputs
        // Override game and go to next level
        accel.readXYZGravity(&xG, &yG, &zG);
        if (left_pb == 0 && fire_pb == 0) {
            playerScore += 10;
        }
        if (left_pb == 0 || xG > 0.5) {
            button = LEFT;
        } else if (right_pb == 0 || xG < -0.5) {
            button = RIGHT;
        } else if (fire_pb == 0 || yG < -0.5) {
            button = FIRE;
        } else if (pause_pb == 0) {
            button = PAUSE;
        } else {
            button = IDLE;
        }
        
        // 3. Update player position
        switch (button) {
        case LEFT :
            player_moveLeft();
            playSound("/sd/wavfiles/beep.wav");
            break;
        case RIGHT :
            player_moveRight();
            playSound("/sd/wavfiles/beep.wav");
            break;
        case FIRE :
            player_fire();
            playSound("/sd/wavfiles/newpewpew.wav");
            break;
        case PAUSE :
            uLCD.cls();
            uLCD.printf("GAME IS PAUSED");
            while(1) {
                if (left_pb == 0 || right_pb == 0 || fire_pb == 0 || pause_pb == 0) {
                    uLCD.cls();
                    player_draw(BLUE);
                    break;
                }
            }
        default :
            break;
        }
        
        // 4. Check for collisions
        checkMissileCollision(playerScore, threshold);
        checkCityCollision(city_num);
        checkPlayerCollision();
        
        // 5. Redraw city landscape
        draw_landscape();
        draw_cities();
        // Display current score
        uLCD.color(BLUE);
        uLCD.locate(0,0);
        uLCD.printf("%d", playerScore);
        
        // 6. Check for endgame
        bool endGame = 0;
        for (int i = 0; i < city_num; i++) {
            if (city_get_info(i).status != DESTORIED)
            {
                endGame = 0;
                break;
            }
            endGame = 1;
        }
        if (player_get_info().status == DESTROYED || endGame) {
            // Game over screen: print score
            // Different sound and text with high score
            uLCD.cls();
            uLCD.color(RED);
            uLCD.locate(0,3);
            uLCD.printf("GAME OVER \n ");
            uLCD.locate(0,9);
            uLCD.printf("Press Button");
            uLCD.locate(0,11);
            uLCD.printf("to Play Again");
            if (playerScore > playerHighScore) {
                playerHighScore = playerScore;
                uLCD.locate(0,5);
                uLCD.color(GREEN);
                uLCD.printf("NEW HIGH SCORE: %d", playerScore);
                playSound("/sd/wavfiles/newhighscore.wav");
            } else {
                uLCD.locate(0,5);
                uLCD.printf("SCORE: %d", playerScore);
                uLCD.locate(0,7);
                uLCD.printf("HIGH SCORE: %d", playerHighScore);
                playSound("/sd/wavfiles/die.wav");
            }
            
            // Loop until push button is pressed to restart game
            while (1) {
                if (left_pb == 0 || right_pb == 0 || fire_pb == 0 || pause_pb == 0) {
                    uLCD.cls();
                    playerScore = 0;
                    playerLevel = -1;
                    // please dont take off points for this
                    goto MainGameMenu;
                    //break;
                }
            }
        }
    }
    
    // ===User implementations end===
    
}

// ===User implementations start===
// Put helper functions here

// checks if enemy missiles collide with player missiles
void checkMissileCollision(int& playerScore, double threshold)
{
    // checks if two missiles collide within a certain range
    double x1, x2, y1, y2;
    DLinkedList* missileDLL = get_missile_list();
    DLinkedList* p_missileDLL = player_get_info().playerMissiles;
    MISSILE* enemy = (MISSILE*) getHead(missileDLL);
    PLAYER_MISSILE* player;
    // nested for loop to check every enemy/player missile
    for (int i = 0; i < getSize(missileDLL); i++) {
        player = (PLAYER_MISSILE*) getHead(p_missileDLL);
        x1 = enemy -> x;
        y1 = enemy -> y;
        for (int j = 0; j < getSize(p_missileDLL); j++) {
            x2 = player -> x;
            y2 = player -> y;
            // if missiles are within threshold:
            // destroy missiles, increase score, animate, make sound
            if (distance(x1, y1, x2, y2) < threshold) {
                enemy -> status = MISSILE_EXPLODED;
                player -> status = PMISSILE_EXPLODED;
                playerScore++;
                explosion(x1, y1);
                playSound("/sd/wavfiles/destroymissile.wav");
            }
            // get next player missile
            player = (PLAYER_MISSILE*) getNext(p_missileDLL);
        }
        enemy = (MISSILE*) getNext(missileDLL);
    }
}

// Checks if enemy missiles collide with city
void checkCityCollision(int city_num)
{
    double x1, x2, y1, y2;
    CITY city;
    DLinkedList* missileDLL = get_missile_list();
    MISSILE* enemy = (MISSILE*) getHead(missileDLL);
    // nested loop to check every enemy missile and city
    while (enemy) {
        x1 = enemy -> x;
        y1 = enemy -> y;
        for (int i = 0; i < city_num; i++) {
            city = city_get_info(i);
            // bottom left corner of city
            x2 = city.x;
            y2 = city.y;
            // if collision occurs, destroy city and enemy missile
            if ((x2-x1)<1 && (x1-x2)<14 && (y2-y1)<10 && (y1-y2)<1) {
                if (city.status == EXIST) {
                    enemy -> status = MISSILE_EXPLODED;
                    city_destory(i);
                    explosion2(x2+3, y2-2);
                    playSound("/sd/wavfiles/destroymissile.wav");
                    break;
                }
                
            }
        }
        enemy = (MISSILE*) getNext(missileDLL);
    }
}

// Checks if missile collides with player
void checkPlayerCollision()
{
    // Get DLL and x, y coordinates
    DLinkedList* missileDLL = get_missile_list();
    MISSILE* enemy = (MISSILE*) getHead(missileDLL);
    double x1, x2, y1, y2;
    while (enemy) {
        x1 = enemy -> x;
        y1 = enemy -> y;
        x2 = player_get_info().x;
        y2 = player_get_info().y;
        // Calculate rectangle in which missile kills player
        // pixels are adjustable 
        if ((x1 - x2) < 12 && (x2 - x1) < 1 && (y2 - y1) < 1 && (y1 - y2) < 5) {
            player_destroy();
            break; 
        }
        enemy = (MISSILE*) getNext(missileDLL);
    }
}

// Explosion animation when something is destroyed
void explosion(int x, int y) {
    float hold = 0.05;
    // flash red and white
    uLCD.filled_circle(x, y, 10, RED);
    wait(hold);
    uLCD.filled_circle(x, y, 10, WHITE);
    wait(hold);
    uLCD.filled_circle(x, y, 10, RED);
    wait(hold);
    uLCD.filled_circle(x, y, 10, WHITE);
    wait(hold);
    // erase circle
    uLCD.filled_circle(x, y, 10, BLACK);
}

// Explosion animation 2 when something is destroyed
void explosion2(int x, int y) {
    float hold = 0.05;
    // multiple circles flash red and white
    uLCD.filled_circle(x, y, 5, BLUE);
    wait(hold);
    uLCD.filled_circle(x, y, 5, WHITE);
    wait(hold);
    uLCD.filled_circle(x+1, y-1, 5, BLUE);
    wait(hold);
    
    uLCD.filled_circle(x+2, y+2, 5, WHITE);
    wait(hold);
    uLCD.filled_circle(x+2, y+2, 5, BLUE);
    wait(hold);
    uLCD.filled_circle(x+1, y-1, 5, WHITE);
    wait(hold);
    
    uLCD.filled_circle(x+1, y-1, 5, BLUE);
    wait(hold);
    uLCD.filled_circle(x, y, 5, BLUE);
    wait(hold);
    uLCD.filled_circle(x+2, y+2, 5, BLUE);
    wait(hold);
    
    // erase circles
    uLCD.filled_circle(x, y, 5, BLACK);
    uLCD.filled_circle(x+2, y+2, 5, BLACK);
    uLCD.filled_circle(x+1, y-1, 5, BLACK);
}

// returns an int telling the user what level they are on based on score
int checkLevel(int score)
{
    return (int) score / 10;
}

// Initializes level based on playerLevel
// 4 levels
void startLevel(int& missile_speed, int& missile_interval, int playerLevel, int city_num, double& threshold) {
    switch (playerLevel) {
    case 0:
        // level 1
        uLCD.cls();
        uLCD.color(GREEN);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.printf("LEVEL 1");
        wait(3);
        uLCD.cls();
        // Initialize missile data
        missile_speed = 8;
        missile_interval = 60;
        threshold = 100;
        missile_init();
        set_missile_speed(missile_speed);
        set_missile_interval(missile_interval);
        // Initialize player and landscape
        player_init();
        city_landscape_init(city_num);
        break;
    case 1:
        // level 2
        uLCD.cls();
        uLCD.color(GREEN);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.printf("LEVEL 2");
        wait(3);
        uLCD.cls();
        // Initialize missile data
        missile_speed = 6;
        missile_interval = 50;
        threshold = 75;
        missile_init();
        set_missile_speed(missile_speed);
        set_missile_interval(missile_interval);
        // Initialize player and landscape
        player_init();
        city_landscape_init(city_num);
        break;
    case 2:
        // level 3
        uLCD.cls();
        uLCD.color(GREEN);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.printf("LEVEL 3");
        wait(3);
        uLCD.cls();
        // Initialize missile data
        missile_speed = 4;
        missile_interval = 40;
        threshold = 50;
        missile_init();
        set_missile_speed(missile_speed);
        set_missile_interval(missile_interval);
        // Initialize player and landscape
        player_init();
        city_landscape_init(city_num);
        break;
    default:
        // level 4, but set at default to avoid any errors
        uLCD.cls();
        uLCD.color(GREEN);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.printf("LEVEL 4");
        wait(3);
        uLCD.cls();
        // Initialize missile data
        missile_speed = 2;
        missile_interval = 30;
        threshold = 25;
        missile_init();
        set_missile_speed(missile_speed);
        set_missile_interval(missile_interval);
        // Initialize player and landscape
        player_init();
        city_landscape_init(city_num);
        break;
    }
}
// calculates distance squared between 2 points
double distance(double x1, double y1, double x2, double y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}
// ===User implementations end===

// Plays a wavfile
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
    return;
}
