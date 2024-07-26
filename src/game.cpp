/// <summary>
/// The actual game
/// </summary>

#define RAYGUI_IMPLEMENTATION

#include <raylib.h>
#include <raygui.h>

#include <ai.hpp>

#define GAME_NAME "Snake"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define TARGET_FPS 60

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
        ClearBackground(RAYWHITE);
        DrawGrid(20, 0);
        //GuiDrawRectangle((Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 1, Fade(RAYWHITE, 0.8f), Fade(BLACK, 0.8f));
        EndDrawing();
    }
    CloseWindow();
}