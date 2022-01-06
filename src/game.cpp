#include "game.h"
#include <iostream>
#include <thread>
#include "SDL.h"
#include <future>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {

  std::thread t2(&Game::PlacePoison, this, this->special);
  std::thread t1 (&Game::PlaceSpecialFood, this, this->special);
  // PlaceFood();
  //std::future<void> futor = std::async(std::launch::async, &Game::PlaceFood, this);
  // this->prm.set_value(0);
  PlaceFood();
  //PlaceSpecialFood();
  //futor.wait();
  t2.join();
  t1.join();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  std::promise<std::chrono::microseconds> prm;
  // prm.set_value(0);
  std::future<std::chrono::microseconds> ftr = prm.get_future();

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, special_food, poison);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  // int food_counter = this->ftr.get();
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      this->food_counter+=1;
      if((this->food_counter)%2 == 0){
        special->special_time = true;
      }
      // prms.set_value(food_counter);
      // prms.set_value(food_counter);
      return;
    }
  }
  
}

void  Game::PlaceSpecialFood(std::shared_ptr<Special> st){
    // std::unique_lock<std::mutex> uLock(_mutex);
    // _cond.wait(uLock, [this] { return (food_counter%2 == 0); });
    int x, y;
    if(st->special_time == true) {
      x = random_w(engine);
      y = random_h(engine);
      // Check that the location is not occupied by a snake item before placing
      // food.
      if (!snake.SnakeCell(x, y)) {
        special_food.x = x;
        special_food.y = y;
        // prms.set_value(0);
        st->special_time = false;
        return;
      }
  }


}

void  Game::PlacePoison(std::shared_ptr<Special> st){

    int x, y;
    if(st->special_time == true) {
      x = random_w(engine);
      y = random_h(engine);
      // Check that the location is not occupied by a snake item before placing
      // food.
      if (!snake.SnakeCell(x, y)) {
        poison.x = x;
        poison.y = y;
        st->special_time = false;
        return;
      }
  }


}

void Game::Update() { 
  if (!snake.alive) return;

  snake.Update();
  
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }

  if (special_food.x == new_x && special_food.y == new_y) {
    score+= 2;
    // PlaceSpecialFood(std::move(ftr));
  }
    if (poison.x == new_x && poison.y == new_y) {
      snake.alive = false;
  }

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }