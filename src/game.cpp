/// <summary>
/// The actual game
/// </summary>

#define RAYGUI_IMPLEMENTATION
#define GLSL_VERSION 100

#include <raylib.h>
#include <raygui.h>

#include <ai.hpp>

#include <functional>
#include <iostream>
#include <queue>

#define GAME_NAME "Snake"
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 110
#define TARGET_FPS 60
#define BACKGROUND_COLOR BLACK
#define SNAKE_COLOR WHITE
#define FOOD_COLOR WHITE
#define WORLD_SIZE Vector2{20, 20}

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

/// @brief  The world renderer system
/// @details The world renderer system is responsible for rendering everything related to this particular world
class WorldRendererSystem{
    public:
        static WorldRendererSystem Instance;
        int grid_num_x, grid_num_y, cell_size_x, cell_size_y;

        WorldRendererSystem(int grid_num_x, int grid_num_y){
            this->grid_num_x = grid_num_x;
            this->grid_num_y = grid_num_y;
            this->cell_size_x = SCREEN_WIDTH / grid_num_x;
            this->cell_size_y = (SCREEN_HEIGHT - 10) / grid_num_y;
        }

        void Render(Vector2 Coordinates, Color color){
            DrawRectangle(Coordinates.x * cell_size_x, Coordinates.y * cell_size_y + 10, cell_size_x, cell_size_y, color);
        }
};

WorldRendererSystem WorldRendererSystem::Instance = WorldRendererSystem(WORLD_SIZE.x, WORLD_SIZE.y);

class Food{
    int grid_num_x, grid_num_y;
    public:
        static Food Instance;
        Vector2 position;
        Food(int grid_num_x, int grid_num_y){
            this->grid_num_x = grid_num_x;
            this->grid_num_y = grid_num_y;
            Generate();
            std::cout << "Food Generated at: " << position.x << ", " << position.y << std::endl;
        }
        void Generate(){
            position = {GetRandomValue(0, grid_num_x - 1) * 1.0f, GetRandomValue(0, grid_num_y - 1) * 1.0f};
        }
        void Render(){
            /// THis does not work?
            WorldRendererSystem::Instance.Render(position, FOOD_COLOR);
            std::cout << "Trying to render food at: " << position.x << ", " << position.y << std::endl;
        }
};

Food Food::Instance = Food(WORLD_SIZE.x, WORLD_SIZE.y);

class ScoreManager{
    public:
        static ScoreManager Instance;
        int score;
        ScoreManager(){
            score = 0;
        }

        void AddScore(int score){
            this->score += score;
        }

        void Reset(){
            score = 0;
        }
};

ScoreManager ScoreManager::Instance = ScoreManager();

class SnakeController{

    private:
        int Up, Down, Left, Right;

        // Control Function Pointers
        std::function<void()> MoveUp;
        std::function<void()> MoveDown;
        std::function<void()> MoveLeft;
        std::function<void()> MoveRight;

    public:
        SnakeController(int Up, int Down, int Left, int Right){
            this->Up = Up;
            this->Down = Down;
            this->Left = Left;
            this->Right = Right;
        }
        void ReceiveInput(){
            if(IsKeyPressed(Up)){
                MoveUp();
                return;
            }
            if(IsKeyPressed(Down)){
                MoveDown();
                return;
            }
            if(IsKeyPressed(Left)){
                MoveLeft();
                return;
            }
            if(IsKeyPressed(Right)){
                MoveRight();
                return;
            }
        }
    protected:
        void SetMoveUp(std::function<void()> MoveUp){
            this->MoveUp = MoveUp;
        }
        void SetMoveDown(std::function<void()> MoveDown){
            this->MoveDown = MoveDown;
        }
        void SetMoveLeft(std::function<void()> MoveLeft){
            this->MoveLeft = MoveLeft;
        }
        void SetMoveRight(std::function<void()> MoveRight){
            this->MoveRight = MoveRight;
        }

        friend class Snake;
        
};

class IntervalManager{
    public:
        IntervalManager(double interval, std::function<void()> callback){
            this->interval = interval;
        }
        void Update(){
            if(GetTime() - lastTime > interval){
                lastTime = GetTime();
                callback();
            }
        }
    protected:
        double interval;
        std::function<void()> callback;
        double lastTime;
};

/// @brief The snake class
/// @details The snake class is responsible for controlling the state of the snake
class Snake{

    SnakeController* controller;
    std::queue<Vector2> snakeBody;

    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    //buffer direction of the snake (used to prevent the snake from moving in the opposite direction)
    Direction bufferDirection;
    //actual direction of the snake
    Direction direction;

    double moveInterval = 0.3;
    double lastMoveTime = 0;

    public:
        Snake(SnakeController* controller){
            this->controller = controller;
            this->controller->SetMoveUp(std::bind(&Snake::MoveUp, this));
            this->controller->SetMoveDown(std::bind(&Snake::MoveDown, this));
            this->controller->SetMoveLeft(std::bind(&Snake::MoveLeft, this));
            this->controller->SetMoveRight(std::bind(&Snake::MoveRight, this));
            this->Setup();
        }

        void Setup(){
            ScoreManager::Instance.Reset();
            Food::Instance.Generate();
            direction = UP;
            bufferDirection = UP;
            snakeBody = std::queue<Vector2>();
            snakeBody.push({5, 7});
            snakeBody.push({5, 6});
            snakeBody.push({5, 5});
        }

        void Move(){
            if(GetTime() - lastMoveTime < moveInterval){
                return;
            }
            lastMoveTime = GetTime();
            Vector2 new_head = snakeBody.back();

            if(!IsOpposite(direction, bufferDirection)){
                direction = bufferDirection;
            }

            switch(direction){
                case UP:
                    new_head.y -= 1;
                    break;
                case DOWN:
                    new_head.y += 1;
                    break;
                case LEFT:
                    new_head.x -= 1;
                    break;
                case RIGHT:
                    new_head.x += 1;
                    break;
            }
            snakeBody.push(new_head);
            if(!CheckFood()){
                snakeBody.pop();
            }else{
                ScoreManager::Instance.AddScore(1);
            }
            CheckCondition();
        }

        void Render(){
            std::queue<Vector2> render_body = snakeBody;
            while(!render_body.empty()){
                WorldRendererSystem::Instance.Render(render_body.front(), SNAKE_COLOR);
                render_body.pop();
            }
        }
    private:
        void MoveUp(){
            if(direction == DOWN){
                return;
            }
            bufferDirection = UP;
            std::cout << "Move Up" << std::endl;
        }

        void MoveDown(){
            if(direction == UP){
                return;
            }
            bufferDirection = DOWN;
            std::cout << "Move Down" << std::endl;
        }

        void MoveLeft(){
            if(direction == RIGHT){
                return;
            }
            bufferDirection = LEFT;
            std::cout << "Move Left" << std::endl;
        }

        void MoveRight(){
            if(direction == LEFT){
                return;
            }
            bufferDirection = RIGHT;
            std::cout << "Move Right" << std::endl;
        }

        bool IsOpposite(Direction direction1, Direction direction2){
            return (direction1 == UP && direction2 == DOWN) || (direction1 == DOWN && direction2 == UP) || (direction1 == LEFT && direction2 == RIGHT) || (direction1 == RIGHT && direction2 == LEFT);
        }

        bool CheckFood(){
            // Check if the snake has collided with the food
            if(Food::Instance.position.x == snakeBody.back().x && Food::Instance.position.y == snakeBody.back().y){
                Food::Instance.Generate();
                return true;
            }
            return false;
        }

        void CheckCondition(){
            // Check if the snake has collided with itself
            // Check if the snake has collided with the wall
            Vector2 new_head = snakeBody.back();
            if(new_head.x < 0 || new_head.x >= WORLD_SIZE.x || new_head.y < 0 || new_head.y >= WORLD_SIZE.y){
                //restart
                Setup();
                return;
            }
            
            std::queue<Vector2> currentSnakeBody = snakeBody;
            while (!currentSnakeBody.empty())
            {
                if(currentSnakeBody.size() == 1){
                    break;
                }
                Vector2 currentHead = currentSnakeBody.front();
                if(currentHead.x == new_head.x && currentHead.y == new_head.y){
                    //restart
                    Setup();
                    return;
                }

                currentSnakeBody.pop();
            }
            
        }
};

int main(){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(TARGET_FPS);

    WorldRendererSystem wrs = WorldRendererSystem(10, 10);
    SnakeController controller = SnakeController(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
    Snake snake = Snake(&controller);

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        GuiDrawRectangle((Rectangle){0, 0, SCREEN_WIDTH, 10}, 1, Fade(RAYWHITE, 0.8f), Fade(RAYWHITE, 0.8f));
        std::string score = "Score: " + std::to_string(ScoreManager::Instance.score);
        GuiLabel((Rectangle){0, 0, SCREEN_WIDTH, 10}, score.c_str());
        //GuiDrawRectangle((Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 1, Fade(RAYWHITE, 0.8f), Fade(BLACK, 0.8f));
        snake.Move();
        snake.Render();
        Food::Instance.Render();
        EndDrawing();
        controller.ReceiveInput();
    }
    CloseWindow();
}