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

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[] = "#cbeaf5";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

constexpr int   NUMBER_OF_TILES         = 10;
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
    std::map<Direction, std::vector<int>> animationAtlas = {
        { FALLING,  {  0,  1,  2,  3,  4,  5,  6,  7 } },
        { ACCELERATING,  {  8,  9, 10, 11, 12, 13, 14, 15} }
    };

    float sizeRatio = 48.0f / 48.0f;

    gProtag = new Entity(
        { ORIGIN.x - 450.0f, ORIGIN.y - 250.0f }, // position
        { SCALE_FACTOR * sizeRatio, SCALE_FACTOR },  // scale
        "assets/spriteSheet.png",          // texture file address
        ATLAS,                           // single image or atlas?
        { 8, 8 },                        // atlas dimensions
        animationAtlas                   // actual atlas
    );

    gProtag->setColliderDimensions({
        gProtag->getScale().x / 3.0f, 
        gProtag->getScale().y / 2.0f
    });

    gProtag -> setJetpackPower(10.0f); 
    gProtag -> setFuel(INITIAL_FUEL);
    gProtag -> setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    SetTargetFPS(FPS);
}

void processInput()
{
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

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gProtag->update(deltaTime, gTiles, 0);
    if (gProtag -> getAngle() == -90 || gProtag -> getAngle() == 90){
        gProtag->setColliderDimensions({
            gProtag->getScale().y / 2.0f, 
            gProtag->getScale().x / 3.0f
        });
    }
    else {
        gProtag->setColliderDimensions({
            gProtag->getScale().x / 3.0f, 
            gProtag->getScale().y / 2.0f
        });
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gProtag-> render();
    gProtag -> displayCollider();

    EndDrawing();
}

void shutdown()
{
    // Unload the textures while the window (GL context) still exists
    delete gProtag;
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
