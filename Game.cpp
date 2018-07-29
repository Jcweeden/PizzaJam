#include "Game.h"

Game* Game::s_pInstance = 0;

Game* Game::Instance()
{
  if(s_pInstance == 0)
  {
    s_pInstance = new Game();
    return s_pInstance;
  }

  return s_pInstance;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
  int flags = 0;

  if (fullscreen) {//if method parameter = true then make variable in line below
    flags = SDL_WINDOW_SHOWN;
  }
  
  // initialise SDL
  if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    std::cout << "Game: SDL init success\n";
    // if succeeded create our window
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
 
    // if the window creation succeeded create our renderer
    if(m_pWindow != 0)
    {
      std::cout << "Game: Window creation success\n";
      m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

      if (m_pRenderer != 0) {
        std::cout << "Game: Renderer creation success\n";
        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
      }
      else {
        std::cout << "Game: Renderer init failed\n";
        return false;
      }
    }
    else
    {
      std::cout << "Game: Window init failed\n";
      return false;
    }
  }
  else
  {
    std::cout << "Game: SDL init failed\n";
    return false; // sdl could not initialise
  }

  std::cout << "Game: Init success\n";
  m_bRunning = true;//start the main loop

  m_pGameStateManager = new GameStateManager();
   
  m_pGameStateManager->changeState(new GameState());
   
  return true;
}


void Game::render()
{
  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour


  //draw background
  for(size_t i = 0; i < m_gameObjects.size(); i++)
  {
    m_gameObjects[i]->draw();
  }

  //draw game / menu
  m_pGameStateManager->render();

  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {
  //   m_currentFrame = int(((SDL_GetTicks() /100) % 6));

   //loop through and update our objects
   for(size_t i = 0; i < m_gameObjects.size(); i++)
   {
     m_gameObjects[i]->update();
   }

   m_pGameStateManager->update();

}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  for (size_t i = 0; i < m_gameObjects.size(); i++) {
    m_gameObjects[i]->clean();
  }

  m_gameObjects.clear();

  TheInputHandler::Instance()->clean();
  
  SDL_DestroyWindow (m_pWindow);
  SDL_DestroyRenderer (m_pRenderer);
  SDL_Quit();
}

void Game::handleEvents()
{
  TheInputHandler::Instance()->update();

  m_pGameStateManager->handleInput();

}

void Game::quit() {
  m_bRunning = false;
}
