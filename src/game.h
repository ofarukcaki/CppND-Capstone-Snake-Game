#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer *renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  // methods for pausing/resuming the game
  void TriggerPause();

 friend class Controller;

 protected:
  bool _wall; // wall around the game screen

 private:
  Snake snake;
  SDL_Point food;
  // Renderer _renderer;
  // pause state
  // true if game is paused
  bool _paused;
  bool _poisoned;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update(Renderer *renderer);
  void Pause();
  void Resume();

};

#endif