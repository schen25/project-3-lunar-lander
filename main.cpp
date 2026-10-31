/**

* Author: Stacey Chen

* Assignment: Lunar Lander!

* Date due: 10/30/26

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/Entity.h"
// many OOP principles were violated in the process... sorry in advance

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[] = "#cbeaf5";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr int   NUMBER_OF_LEFT_GRASS    = 23,
                NUMBER_OF_RIGHT_GRASS   = 10,
                NUMBER_OF_WATER         = 10,
                NUMBER_OF_LEFT_COLS     = 5,
                NUMBER_OF_RIGHT_COLS    = 5;

constexpr float TILE_DIMENSION          = 50.0f,
                // in m/ms², since delta time is in ms
                ACCELERATION_OF_GRAVITY = 10.0f,
                SCALE_FACTOR = 100.0f,
                INITIAL_FUEL = 750.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;

Entity *gProtag = nullptr;
Entity *gTiles  = nullptr;
bool gGameOver = false;
bool gWinStatus = false;

std::vector<Entity*> gMapBlocks;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Action, std::vector<int>> animationAtlas = {
        { PLAYER_FALLING,  {  0,  1,  2,  3,  4,  5,  6,  7 } },
        { PLAYER_ACCELERATING,  {  8,  9, 10, 11, 12, 13, 14, 15} },
        { WATER_MOVING, {24, 25, 26, 27, 28, 29, 30, 31}},
        { GRASS_MOVING, {32, 33, 34, 35, 36, 37, 38, 39}},
        { CLOUD_MOVING, {40, 41, 42, 43, 44, 45, 46, 47}},
        { NO_ACTION, {63}}
    };

    float sizeRatio = 48.0f / 48.0f;

    gProtag = new Entity(
        { ORIGIN.x - 450.0f, ORIGIN.y - 250.0f }, // position
        { SCALE_FACTOR * sizeRatio, SCALE_FACTOR },  // scale
        "assets/spriteSheet.png",          // texture file address
        ATLAS,                           // single image or atlas?
        { 8, 8 },                        // atlas dimensions
        animationAtlas,                   // actual atlas
        PLAYER
    );

    gProtag->setColliderDimensions({
        gProtag->getScale().x / 4.25f, 
        gProtag->getScale().y / 2.0f
    });

    gProtag -> fall();
    gProtag -> setJetpackPower(10.0f); 
    gProtag -> setFuel(INITIAL_FUEL);

    Vector2 protagVelocity = {static_cast<float>(gProtag -> getSpeed()), 0.0f};
    gProtag -> setVelocity(protagVelocity);
    gProtag -> setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    /*
        ----------- TILES -----------
    */

    float leftMostX = TILE_DIMENSION / 2.0f;
    float bottomMostY = SCREEN_HEIGHT - TILE_DIMENSION / 2.0f;

    for (size_t row = 0; row < NUMBER_OF_LEFT_GRASS / NUMBER_OF_LEFT_COLS + 1; row++){
        size_t colsForRow = std::min(static_cast<std::size_t>(NUMBER_OF_LEFT_COLS), NUMBER_OF_LEFT_GRASS-row*5);
        for (size_t col = 0; col < colsForRow; col++){
            gMapBlocks.push_back(new Entity(
                {leftMostX + col * TILE_DIMENSION,
                bottomMostY - row * TILE_DIMENSION},
                {TILE_DIMENSION, TILE_DIMENSION},
                "assets/spriteSheet.png",
                ATLAS,
                {8, 8},
                animationAtlas,
                GRASS
            ));

            gMapBlocks[gMapBlocks.size()-1] -> grassMove();
        }
    }

    leftMostX = TILE_DIMENSION*NUMBER_OF_LEFT_COLS+TILE_DIMENSION/2.0f;

    
    for (size_t i = 0; i < NUMBER_OF_WATER; i++){
         gMapBlocks.push_back(new Entity(
            {leftMostX + i * TILE_DIMENSION,
            bottomMostY},
            {TILE_DIMENSION, TILE_DIMENSION},
            "assets/spriteSheet.png",
            ATLAS,
            {8, 8},
            animationAtlas,
            WATER
        ));

         gMapBlocks[gMapBlocks.size()-1] -> waterMove();
         gMapBlocks[gMapBlocks.size()-1]-> setColliderDimensions({
            gMapBlocks[gMapBlocks.size()-1]->getScale().x, 
            gMapBlocks[gMapBlocks.size()-1]->getScale().y / 10.0f
        });
    }

    gMapBlocks.push_back(new Entity{
        {leftMostX, bottomMostY - 2*TILE_DIMENSION},
        {TILE_DIMENSION, TILE_DIMENSION},
        "assets/spriteSheet.png",
        ATLAS,
        {8,8},
        animationAtlas,
        CLOUD
    });

    gMapBlocks[gMapBlocks.size()-1] -> cloudMove();
    gMapBlocks[gMapBlocks.size()-1]-> setColliderDimensions({
        gMapBlocks[gMapBlocks.size()-1]->getScale().x / 1.1f, 
        gMapBlocks[gMapBlocks.size()-1]->getScale().y / 2.25f
    });
    gMapBlocks[gMapBlocks.size()-1] -> setMovement({1.0f, 0.0f});

    leftMostX = TILE_DIMENSION*(5+NUMBER_OF_WATER)+TILE_DIMENSION/2.0f;
    for (size_t row = 0; row < NUMBER_OF_RIGHT_GRASS / NUMBER_OF_RIGHT_COLS + 1; row++){
        size_t colsForRow = std::min(static_cast<std::size_t>(NUMBER_OF_RIGHT_COLS),
                                      NUMBER_OF_LEFT_GRASS + NUMBER_OF_RIGHT_GRASS-row*3);
        for (size_t col = 0; col < colsForRow; col++){
            gMapBlocks.push_back(new Entity(
                {leftMostX + col * TILE_DIMENSION,
                bottomMostY - row * TILE_DIMENSION},
                {TILE_DIMENSION, TILE_DIMENSION},
                "assets/spriteSheet.png",
                ATLAS,
                {8, 8},
                animationAtlas,
                GRASS
            ));

            gMapBlocks[gMapBlocks.size()-1] -> grassMove();
        }
    }



    SetTargetFPS(FPS);
}

void processInput()
{
    if (!gGameOver){
        gProtag->resetMovement();

        if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && gProtag -> getFuel()) gProtag-> useJetpack();
        else gProtag -> fall();

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
            gProtag -> rotateLeft(); 
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
            gProtag -> rotateRight();
        }

        if (GetLength(gProtag->getMovement()) > 1.0f)
            gProtag->normaliseMovement();
    }
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    if (!gGameOver) {
        float ticks     = static_cast<float>(GetTime());
        float deltaTime = ticks - gPreviousTicks;
        gPreviousTicks  = ticks;

        gProtag->update(deltaTime,  gMapBlocks, static_cast<int>(gMapBlocks.size()));
        if (gProtag -> getAngle() == -90 || gProtag -> getAngle() == 90){
            gProtag->setColliderDimensions({
                gProtag->getScale().y / 2.0f, 
                gProtag->getScale().x / 3.0f
            });
        }
        else {
            gProtag->setColliderDimensions({
                gProtag->getScale().x / 4.25f, 
                gProtag->getScale().y / 2.0f
            });
        }

        if (gProtag -> isCollidingLeft() || gProtag -> isCollidingRight() 
            || gProtag -> hasCollidedWaterTop() ||
             (gProtag -> isCollidingBottom() && (gProtag -> getAngle() == -90.0f || gProtag -> getAngle() == 90.0f))){
            gGameOver = true;
            gWinStatus = false;
        }
        else if (gProtag -> HasCollidedGrassOrCloudTop()){
            gGameOver = true;
            gWinStatus = true;
        }


        for (size_t i = 0; i <  static_cast<int>(gMapBlocks.size()); i++){
            gMapBlocks[i] -> update(deltaTime, gMapBlocks, 0);
        }
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    DrawText(TextFormat("Fuel: %04i", static_cast<int>(gProtag -> getFuel())), 20, 20, 20, ColorFromHex("#437871"));

    if (gGameOver){
        if (gWinStatus) DrawText("Mission success!", 150, 70, 75, ColorFromHex("#2ad48d"));
        else DrawText("Mission failed :(", 170, 70, 75, ColorFromHex("#d44c2a"));
    }

    for (size_t i = 0; i <  static_cast<int>(gMapBlocks.size()); i++){
         gMapBlocks[i] -> render();
    }

    gProtag-> render();

    EndDrawing();
}

void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gProtag;

    for (size_t i = 0; i <  static_cast<int>(gMapBlocks.size()); i++){
        delete  gMapBlocks[i];
    }

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
