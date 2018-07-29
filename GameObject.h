#ifndef _GameObject_
#define _GameObject_

#include "I_GameObject.h"
#include "Game.h"
#include "Vector2D.h"
#include "TextureManager.h"

#include <iostream>
#include <string>

//class OrbitingObject;

class GameObject : public I_GameObject {

public:

  GameObject(int x, int y, int p_width, int p_height, std::string textureID, int p_numFrames);
//~GameObject();


  virtual void draw();
  virtual void drawFrame();
  virtual void update();
  virtual void clean();

  Vector2D& getPosition() { return position; }

int getWidth() { return width; }
int getHeight() { return height; }

virtual void setFrame(int frame) { currentFrame = frame;}
int getFrame() { return currentFrame;}

void setRow(int row) { currentRow = row;}

protected:

  int width;
  int height;

  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;


  int numFrames;
  int currentRow;
  int currentFrame;

  std::string textureID;

};

#endif
