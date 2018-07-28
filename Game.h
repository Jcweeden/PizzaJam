#ifndef _Game_
#define _Game_
#include <SDL2/SDL.h>

#include "GameStateManager.h"
#include "InputHandler.h"
#include "GameObject.h"
//#include "OrbitingObject.h"

#include "TextureManager.h"
#include "GameState.h"

#include <iostream>
#include <vector>

class Game {
public:

  static Game* Instance();
        
  bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();

  void quit();

  int getScreenWidth() { return 512; }
  int getScreenHeight() { return 512; }
  
  SDL_Renderer* getRenderer() const { return m_pRenderer; }
  GameStateManager* getStateManager() {return m_pGameStateManager;}


public:

  Game(){}

  bool m_bRunning;

  //static instance variable
  static Game* s_pInstance;
  
  SDL_Window* m_pWindow;
  SDL_Renderer* m_pRenderer;

  GameStateManager* m_pGameStateManager;

  std::vector<GameObject*> m_gameObjects;
};

typedef Game TheGame;

#endif
