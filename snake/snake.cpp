#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

// Colors
Color green = {173, 204, 96, 255};
Color darkgreen = {45, 56, 27, 255};

// Cell size and count
int cellsize = 30;
int cellcount = 25;
int offset =75;

double last_update_time = 0;

bool eventtriggered(double interval) {
    double current_time = GetTime();
    if (current_time - last_update_time >= interval) {
        last_update_time = current_time;
        return true;
    }
    return false;
}

bool element_in_deque(Vector2 element, deque<Vector2> deque) {
    for (int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], element)) {
            return true;
        }
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};
    bool add_segment = false;

    void Draw() {
        for (int i = 0; i < body.size(); i++) {
            int x = body[i].x;
            int y = body[i].y;
            DrawRectangle(offset+x * cellsize, offset+y * cellsize, cellsize, cellsize, darkgreen);
        }
    }

    void update() {
        if (add_segment == true) {
            body.push_front(Vector2Add(body[0], direction));
            add_segment = false;
        } else {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
    }
    void reset(){
        body ={Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food {
public:
    Vector2 position;

    Food(deque<Vector2> snakebody) {
        position = GenerateRandomPos(snakebody);
    }

    void Draw() {
        DrawRectangle(offset+position.x * cellsize,offset+ position.y * cellsize, cellsize, cellsize, darkgreen);
    }

    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellcount - 1);
        float y = GetRandomValue(0, cellcount - 1);
        return Vector2{x, y}; 
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakebody) {
        Vector2 position = GenerateRandomCell();
        while (element_in_deque(position, snakebody)) {
            position = GenerateRandomCell(); 
        }
        return position;
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    bool running = true;
    int score=0;
    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update() {
        if(running){
        snake.update();
        check_collision_food();
        check_collision_with_edges();
        collision_tail();
        }
    }

    void check_collision_food() {
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.GenerateRandomPos(snake.body);
            snake.add_segment = true;
            score++;
        }
    }

    void check_collision_with_edges(){
        if(snake.body[0].x==cellcount||snake.body[0].x==-1){
            game_over();
        }
        if(snake.body[0].y==cellcount||snake.body[0].y==-1){
            game_over();
        }
    }

    void game_over(){
        snake.reset();
        food.position=food.GenerateRandomPos(snake.body);
        running =false;
        score=0;
    }
    void collision_tail(){
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if(element_in_deque(snake.body[0],headlessbody)){
            game_over();
        }
    }

};

int main() {
    cout << "Starting the Game....." << endl;
    InitWindow(2*offset+cellsize * cellcount,2*offset+ cellsize * cellcount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();
    while (WindowShouldClose() == false) {
        BeginDrawing();
        if (eventtriggered(0.2)) game.Update();

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running=true;
        }

        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running=true;
        }

        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running=true;
        }

        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running=true;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5,(float)cellsize * cellcount+10,(float)cellsize * cellcount+10}, 5 , darkgreen);
        DrawText("RETRO SNAKE",offset-5 ,20 ,40,darkgreen );
        DrawText(TextFormat("%i",game.score),offset-5 ,offset+cellsize*cellcount+10 ,40,darkgreen );
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
