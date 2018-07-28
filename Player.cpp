#include "Player.h"


Player::Player(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames,
                               float p_startingAngle, bool p_isRotatingClockwise, float p_angleIncrementVal)
    : OrbitingObject::OrbitingObject(x,y,p_width,p_height,textureID,p_numFrames,
                                     p_startingAngle, p_isRotatingClockwise, p_angleIncrementVal)
{
}

void Player::update()
{
  handleInput();

  OrbitingObject::update();
}


void Player::handleInput()
{ 



}
