#include "TextureManager.h"
#include "Game.h"
#include "OrbitingObject.h"
#include "Player.h"
#include <stdlib.h>     /* srand, rand */
#include <limits.h>

const std::string GameState::gameStateID = "PLAY";

void GameState::update ()
{
  player->update();
  sun->update();
  for (size_t i = 0; i < targets.size(); i++)
  {
    targets[i]->update();
  }

  //if a target is selected
  //if the player has passed the target
}

void GameState::render ()
{
  SDL_RenderCopy(TheGame::Instance()->getRenderer(), scoreText, NULL, &scoreRect); 

  if (targetPlanetIndex > -1)
  {
    drawTargetOutline();
  }

  for (size_t i = 0; i < targets.size(); i++)
  {
    targets[i]->draw();
  }

  player->draw();
  sun->draw();
}

void GameState::handleInput()
{
  
  if(TheInputHandler::Instance()->getIsSpaceKeyPressed())
  {

    if (gameStarted)     //check if has started game
    {
      
      if( TheCollManager::Instance()->isCollidingRectRect(player, targets[targetPlanetIndex], 1000))
      {
        std::cout << "win\n";

        //check if more targets to get
        //decrement targets

        getNewTarget();
      }
           
      //check if colliding with target planet
      //if has
      //else
      //run complete
    }
    else     //if not then start
    {
      gameStarted = true;

      player->setSpeed(0.85f);
      getNewTarget();
    }

    player->changeOrbitDirection(); 
  }
}

void GameState::onEnter()
{
  //std::vector<GameObject*> gameObjects;

  gameStarted = false;
  levelReached = 1;
  targetPlanetIndex = -1;

  initText();
  
  TheTextureManager::Instance()->load("assets/sun.png", "sun", TheGame::Instance()->getRenderer());
  
  sun = new GameObject((TheGame::Instance()->getScreenWidth()/2), (TheGame::Instance()->getScreenWidth()/2),
                       128, 128,
                       "sun", 1);
  
  TheTextureManager::Instance()->load("assets/planet01.png", "planet01", TheGame::Instance()->getRenderer());
  OrbitingObject* planet01 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet01", 1,
                                                0, true, 0.08f);
  targets.push_back(planet01);

  TheTextureManager::Instance()->load("assets/planet02.png", "planet02", TheGame::Instance()->getRenderer());
  OrbitingObject* planet02 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet02", 1,
                                                95, true, 0.08f);
  targets.push_back(planet02);

  TheTextureManager::Instance()->load("assets/planet03.png", "planet03", TheGame::Instance()->getRenderer());
  OrbitingObject* planet03 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                192, 64,
                                                "planet03", 1,
                                                45, false, 0.12f);
  targets.push_back(planet03);

  TheTextureManager::Instance()->load("assets/planet04.png", "planet04", TheGame::Instance()->getRenderer());
  OrbitingObject* planet04 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet04", 1,
                                                140, false, 0.16f);
  targets.push_back(planet04);

  TheTextureManager::Instance()->load("assets/player.png", "player", TheGame::Instance()->getRenderer());
  player = new Player(0, (TheGame::Instance()->getScreenHeight()/2),
                      64, 64,
                      "player", 1,
                      143, true, 0.25f);
}


void GameState::onExit()
{

  if( scoreText != NULL ) SDL_DestroyTexture( scoreText );
  TTF_CloseFont( fontTTF );
  fontTTF = NULL;
  
  for (GameObject *obj: targets)
  {
    //delete from targets
    delete obj;
  }  
  targets.clear();

  delete player;

  delete sun;
  
  //TheTextureManager::Instance()->clearFromTextureMap("ball");

}

void GameState::getNewTarget()
{
  std::cout << "getNewTarget()\n";
  size_t randomTargetIndex;
  bool foundTarget = false;
  std::vector<unsigned> targetsToAttempt = {0,1,2,3}; //vector storing indexs of genes not yet attempted to overwrite
    
  do {
    //randomly select a previously unchecked target
    unsigned randIndex = rand() % targetsToAttempt.size();
    randomTargetIndex = targetsToAttempt[randIndex];

    //remove the target so it will not be selected again
    targetsToAttempt.erase(targetsToAttempt.begin() + randIndex);
      
    //if the target is far enough away
    if (targets[randomTargetIndex]->getPosition().calcDistance(player->getPosition()) > 230)
    {
      std::cout << targets[randomTargetIndex]->getPosition().calcDistance(player->getPosition()) << ": found target\n";

      targetPlanetIndex = randomTargetIndex;
      
      foundTarget = true;
    }
    else
    {
      targetPlanetIndex = -1;
    }
  } while (foundTarget == false && targetsToAttempt.size() > 0);
}

void GameState::drawTargetOutline()
{
  targets[targetPlanetIndex]->getPosition();

  filledCircleRGBA(TheGame::Instance()->getRenderer(),
                   targets[targetPlanetIndex]->getPosition().getX(), targets[targetPlanetIndex]->getPosition().getY(),
                   50,
                   255, 255, 255, 60);
  circleRGBA(TheGame::Instance()->getRenderer(),
             targets[targetPlanetIndex]->getPosition().getX(), targets[targetPlanetIndex]->getPosition().getY(),
             50,
             0, 0, 0, 255);
      
}

void GameState::initText()
{

  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }
  
  fontTTF = TTF_OpenFont("Assets/ka1.ttf", 24);
  if (fontTTF == NULL) {
    fprintf(stderr, "error: font not found\n");
  }

  scoreMessage = TTF_RenderText_Solid(fontTTF, "1", textColour); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage); 
  scoreRect.x = 225;  //controls the rect's x coordinate 
  scoreRect.y = 0; // controls the rect's y coordinte
  scoreRect.w = 30; // controls the width of the rect
  scoreRect.h = 30; // controls the height of the rect

}
