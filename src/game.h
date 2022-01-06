#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <mutex>
#include <condition_variable>
#include <future>

class Special {

  public:
    bool special_time = false;
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  SDL_Point special_food;
  SDL_Point poison;
  int food_counter = 0;
  std::shared_ptr<Special> special;
  
  std::mutex _mutex;
  std::condition_variable _cond;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void PlaceSpecialFood(std::shared_ptr<Special> st);
  void PlacePoison(std::shared_ptr<Special> st);
  void Update();
};



#endif