#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
// #include "game.h"
class Game;

class Controller {
 public:
  // receive a Game object reference
  void HandleInput(bool &running, Snake &snake, Game &game) const;
  void OppositeDirection(Snake::Direction input);
  // get the reference of the Game class to
  // will be used to pause*resume game state
//   void SetGame(Game const &game);

 private:
  // the game isntance
//   Game game;
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite, bool &poison) const;
};

#endif
