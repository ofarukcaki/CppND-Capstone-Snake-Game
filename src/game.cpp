#include "game.h"
#include <iostream>
#include "SDL.h"
#include <thread>
#include <chrono>
#include <future>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height, &score),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  PlaceFood();
}

// Poison timer, if the snake get poisoned the thread 
// will return snake to normal after 5 seconds
void TimerThread(bool *poisoned) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    // get back to normal after 5 seconds
    *poisoned = false;
}

void Game::Run(Controller const &controller, Renderer *renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;



  while (running) {
    frame_start = SDL_GetTicks();


    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, *this);
    // pass renderer's address
    Update(renderer);
    renderer->Render(snake, food, &_wall, &_poisoned);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer->UpdateWindowTitle(score, frame_count);
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
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}



void Game::Update(Renderer *renderer) {
  if(this->_paused == true) {
    // update window title
    renderer->SetPauseTitle();
    return;
  };
  // std::cout << "update called : " << this->_paused << std::endl;
  if (!snake.alive) return;

  // send address of boolean _wall;
  snake.Update(&_wall);

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> dis(1, 10);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();

    // 20 percentage change for the poisonous food
    if(dis(gen) <= 2){
      _poisoned = true;
      // resolves 5 seconds later
      std::thread poisonTimer(TimerThread, &_poisoned);
      poisonTimer.detach();
    }
  }
}


// pause the game if it's already running
// resume otherwise
void Game::TriggerPause(){
  this->_paused ? Resume() : Pause();
}

// pause the game
void Game::Pause(){
  this->_paused = true;  
}

// resume the game
void Game::Resume(){
  this->_paused = false;
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }