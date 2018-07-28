#ifndef _GameState_
#define _GameState_

#include "SDL2/SDL2_gfxPrimitives.h"
#include <SDL2/SDL_ttf.h>

#include "CollisionManager.h"

#include "State.h"

//#include "GameObject.h"
//#include "OrbitingObject.h"

#include "Vector2D.h"

#include <string>
#include <iostream>
#include <vector>

class GameObject;
class OrbitingObject;
class Player;
//class declarations of objects here

class GameState : public State {  
  
public:

  GameState(){}
  
  virtual void update();
  virtual void render();
  void handleInput();

  virtual void onEnter();
  virtual void onExit();

  void initText();
  
  virtual std::string getStringID() {return gameStateID;}

  void getNewTarget();

  void drawTargetOutline();
  
public:


  static const std::string gameStateID;

  std::vector<GameObject*> targets;

  GameObject* sun;
  
  Player* player;


  int targetPlanetIndex;
  bool gameStarted;
  int levelReached;

   //text
  TTF_Font* fontTTF; //this opens a font style and sets a size
  SDL_Color textColour = {255,255,255};  // this is the color in rgb format
  
  SDL_Surface* scoreMessage;
  SDL_Texture* scoreText; 
  SDL_Rect scoreRect;

};

#endif
