/// <summary>
/// The actual game
/// </summary>

#define RAYGUI_IMPLEMENTATION
#define GLSL_VERSION 100

#include <raylib.h>
#include <raygui.h>

#include <ai.hpp>

#define GAME_NAME "Snake"
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 110
#define TARGET_FPS 60

Shader gridShader;

class GameManager{
    enum class GameState{
        MENU,
        PLAYING,
        GAME_OVER
    };
    public:
        GameState gameState;
        GameManager(){
            gameState = GameState::MENU;
        }
};

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(TARGET_FPS);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        GuiDrawRectangle((Rectangle){0, 0, SCREEN_WIDTH, 10}, 1, Fade(RAYWHITE, 0.8f), Fade(RAYWHITE, 0.8f));
        GuiLabel((Rectangle){0, 0, SCREEN_WIDTH, 10}, "Score: 10");
        //GuiDrawRectangle((Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 1, Fade(RAYWHITE, 0.8f), Fade(BLACK, 0.8f));
        EndDrawing();
    }
    CloseWindow();
}