#ifndef _OrbitingObject_
#define _OrbitingObject_

#include "I_GameObject.h"
#include "GameObject.h"
#include "Game.h"
#include "Vector2D.h"

#include <iostream>
#include <string>


class OrbitingObject : public GameObject {

public:

  OrbitingObject(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames,
                 float p_startingAngle, bool p_isRotatingClockwise, float p_angleIncrementVal);
  //virtual ~OrbitingObject();

  
  virtual void update();
  virtual void draw();

  virtual void clean();
  
  void changeOrbitDirection();

  void setMoon (OrbitingObject* p_moon);
  void orbitMoon(Vector2D origin, const int radius);
  
  void setSpeed(float val) { angleIncrementVal = val; }
  const float getSpeed() { return angleIncrementVal; }

  void setSlowDown(bool val) { slowDown = val; speedUp = !val; }
  void setSpeedUp(bool val) { speedUp = val; slowDown = !val; }

  virtual void setFrame(int frame) { currentFrame = frame; if (moon != nullptr) moon->setFrame(frame);}

public:

  float angle;
  float angleIncrementVal;
  bool isRotatingClockwise;

  bool speedUp, slowDown;

  OrbitingObject* moon;

};

#endif
