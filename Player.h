#ifndef _Player_
#define _Player_

#include "I_GameObject.h"
#include "GameObject.h"
#include "OrbitingObject.h"
#include "Game.h"
#include "Vector2D.h"
#include "InputHandler.h"

#include <iostream>
#include <string>


class Player : public OrbitingObject {

public:

  Player(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames,
                 float p_startingAngle, bool p_isRotatingClockwise, float p_angleIncrementVal);

  virtual void update();
  virtual void draw();

  void handleInput();

  

public:

  float angle;
  float angleIncrementVal;
  bool isRotatingClockwise;
  
};

#endif
