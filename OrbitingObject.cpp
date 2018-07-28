#include "OrbitingObject.h"


OrbitingObject::OrbitingObject(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames,
                               float p_startingAngle, bool p_isRotatingClockwise, float p_angleIncrementVal)
    : isRotatingClockwise(p_isRotatingClockwise), angleIncrementVal(p_angleIncrementVal), angle(p_startingAngle),
      GameObject::GameObject(x,y,p_width,p_height,textureID,p_numFrames)
{
}

void OrbitingObject::update()
{

  if (isRotatingClockwise)
    angle += angleIncrementVal;
  else
    angle -= angleIncrementVal;

  /*
  if (angle >= 360.0f) angle -= 360.0f;
  else if (angle <= -360.0f) angle += 360.0f;
  */
  
  int radius = (TheGame::Instance()->getScreenWidth()/2) - 64; //sun's position - half of sun's sprite width
  
  getPosition().setX(TheGame::Instance()->getScreenWidth()/2 + cos((float)angle/10)*radius);
  getPosition().setY(TheGame::Instance()->getScreenHeight()/2 + sin((float)angle/10)*radius);
  
  GameObject::update();
}


void OrbitingObject::changeOrbitDirection()
{
  isRotatingClockwise = !isRotatingClockwise;
}
