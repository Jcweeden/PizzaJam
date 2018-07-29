#include "OrbitingObject.h"


OrbitingObject::OrbitingObject(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames,
                               float p_startingAngle, bool p_isRotatingClockwise, float p_angleIncrementVal)
    : isRotatingClockwise(p_isRotatingClockwise), angleIncrementVal(p_angleIncrementVal), angle(p_startingAngle), speedUp(false), slowDown(false), moon(nullptr),
      GameObject::GameObject(x,y,p_width,p_height,textureID,p_numFrames)
{
}

void OrbitingObject::clean()
{
  if (moon != nullptr) delete moon;
  GameObject::clean();
}

void OrbitingObject::update()
{

  if (isRotatingClockwise)
    angle += angleIncrementVal;
  else
    angle -= angleIncrementVal;

  if (slowDown)
  {
    if (angleIncrementVal > 0.1f)
      angleIncrementVal -= 0.01f;
    else slowDown = false;
  }
  
  else if (speedUp)
  {
    if (angleIncrementVal < 0.65f)
      angleIncrementVal += 0.01f;
    else speedUp = false;
  }   
  
  int radius = (TheGame::Instance()->getScreenWidth()/2) - 64; //sun's position - half of sun's sprite width
  
  getPosition().setX(TheGame::Instance()->getScreenWidth()/2 + cos((float)angle/10)*radius);
  getPosition().setY(TheGame::Instance()->getScreenHeight()/2 + sin((float)angle/10)*radius);
  
  GameObject::update();

  if (moon != nullptr) orbitMoon(getPosition(), GameObject::width/2+20);
}

void OrbitingObject::draw()
{
  GameObject::drawFrame();

  if (moon != nullptr)
  {
    moon->draw();
  }
}


void OrbitingObject::changeOrbitDirection()
{
  isRotatingClockwise = !isRotatingClockwise;
}


void OrbitingObject::setMoon(OrbitingObject* p_moon)
{
  moon = p_moon;
}

void OrbitingObject::orbitMoon(Vector2D origin, const int radius)
{
  if (isRotatingClockwise)
    angle += angleIncrementVal;
  else
    angle -= angleIncrementVal;

  if (slowDown)
  {
    if (angleIncrementVal > 0.1f)
      angleIncrementVal -= 0.01f;
    else slowDown = false;
  }
  
  else if (speedUp)
  {
    if (angleIncrementVal < 0.65f)
      angleIncrementVal += 0.01f;
    else speedUp = false;
  }   
  
  //int radius = (TheGame::Instance()->getScreenWidth()/2) - 64; //sun's position - half of sun's sprite width
  
  moon->getPosition().setX(origin.getX() + cos((float)angle/10)*radius);
  moon->getPosition().setY(origin.getY() + sin((float)angle/10)*radius);


  //int radi = (TheGame::Instance()->getScreenWidth()/2) - 64; //sun's position - half of sun's sprite width
  
  //moon->getPosition().setX(TheGame::Instance()->getScreenWidth()/2 + cos((float)angle/10)*radi);
  //moon->getPosition().setY(TheGame::Instance()->getScreenHeight()/2 + sin((float)angle/10)*radi);
}
