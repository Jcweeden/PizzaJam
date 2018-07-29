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

  if (gameStarted)
  {
    //if has not made contact with planet yet
    if (!hasMadeContactWithTargetPlanet)
    {
      hasMadeContactWithTargetPlanet = TheCollManager::Instance()->isCollidingRectRect(player, targets[targetPlanetIndex], 1000);
    }
    //has already made contact with planet
    else if (hasMadeContactWithTargetPlanet)
    {//if has now passed the planet
      if (TheCollManager::Instance()->isCollidingRectRect(player, targets[targetPlanetIndex], 1000) == false)
      {
        TheSoundMixer::Instance()->playSound("lose",0);
        //then have lost
        hasMadeContactWithTargetPlanet = false;
        gameStarted = false;

        //reset old planet back to normal frame
        targets[targetPlanetIndex]->setFrame(0);
        
        targetPlanetIndex = -1;
        targetsRemaining = 0;

        //set sun's face
        sun->setFrame(2);

        player->setRow(1);
        //reset speed
        player->setSlowDown(true);

        scoreRect.x = 297;
        std::string lose = "Lose";
        scoreMessage = TTF_RenderText_Blended(fontTTF,
                                              lose.c_str(),
                                              textColour);
        scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage);
        SDL_FreeSurface( scoreMessage );
      }
    }
  }
  
  //if a target is selected
  //if the player has passed the target
}

void GameState::render ()
{
  background->setFrame(int(((SDL_GetTicks() / 1000) % 2)));
  background->drawFrame();


  if (targetPlanetIndex > -1)
  {
    drawTargetOutline();
  }

  for (size_t i = 0; i < targets.size(); i++)
  {
    targets[i]->draw();
  }
  
  player->setFrame(int(((SDL_GetTicks() / 100) % 2)));
  player->drawFrame();

  sunAnimation->setFrame(int(((SDL_GetTicks() / 400) % 2)));
  sunAnimation->drawFrame();
  sun->drawFrame();
  speechBubble->draw();


  SDL_RenderCopy(TheGame::Instance()->getRenderer(), scoreText, NULL, &scoreRect); 

  SDL_RenderCopy(TheGame::Instance()->getRenderer(), levelText, NULL, &levelRect); 

  if (gameStarted == false)
  {
    if (player->getSpeed() <= 0.15f)
    {
      if ((SDL_GetTicks() % 2000) < 1250) {
        SDL_RenderCopy(TheGame::Instance()->getRenderer(), pressToPlayText, NULL, &pressToPlayRect); 
      }
    }
  }
}

void GameState::handleInput()
{
  
  if(TheInputHandler::Instance()->getIsSpaceKeyPressed())
  {

    if (gameStarted)     //check if has started game
    {
      //check if colliding with target planet
      if( TheCollManager::Instance()->isCollidingRectRect(player, targets[targetPlanetIndex], 1000))
      {
        //if has collided
        //check if more targets to get
        if (--targetsRemaining > 0)
        {
          //decrement targets
          //targetsRemaining--;
          std::cout << "hit target - more to come\n";

          TheSoundMixer::Instance()->playSound("space",0);

          
          //update text
          if( scoreText != NULL ) SDL_DestroyTexture( scoreText );

          scoreMessage = TTF_RenderText_Blended(fontTTF,
                                                std::to_string(targetsRemaining).c_str(),
                                                textColour);
          scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage);
          SDL_FreeSurface( scoreMessage );


          hasMadeContactWithTargetPlanet = false;

          //reset old planet back to normal frame
          targets[targetPlanetIndex]->setFrame(0);
          
          //get a new target
          getNewTarget();
        }
        else
        {
          //level complete
          std::cout << "level complete\n";

          TheSoundMixer::Instance()->playSound("win",0);

          //increment level
          levelReached++;
          gameStarted = false;

          //set sun's face
          sun->setFrame(0);
          
          //reset old planet back to normal frame
          targets[targetPlanetIndex]->setFrame(0);

          targetPlanetIndex = -1;
          targetsRemaining = 0;

          //reset speed
          player->setSlowDown(true);

          player->setRow(1);

          player->changeOrbitDirection(); 
                
          //change text to show have won
          if( scoreText != NULL ) SDL_DestroyTexture( scoreText );

          scoreRect.x = 297;
          std::string win = "Win";
          scoreMessage = TTF_RenderText_Blended(fontTTF,
                                                win.c_str(),
                                                textColour);
          scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage);
          SDL_FreeSurface( scoreMessage );

          
          std::string level = "Level ";

          if (levelReached < 10) level.append("0");
          level.append(std::to_string(levelReached));
          
          levelMessage = TTF_RenderText_Blended(fontTTF,
                                                level.c_str(),
                                                textColourWhite);
          levelText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), levelMessage);
          SDL_FreeSurface( levelMessage );
          
        }
      }
      else //pressed space when not on target - lose
      {
        std::cout << "pressed space not on target\n";

        TheSoundMixer::Instance()->playSound("lose",0);

        
        player->changeOrbitDirection(); 
        hasMadeContactWithTargetPlanet = false;
        gameStarted = false;

        //set sun's face
        sun->setFrame(2);
        
        //reset old planet back to normal frame
        targets[targetPlanetIndex]->setFrame(0);
        targetPlanetIndex = -1;
        targetsRemaining = 0;

        player->setRow(1);

        //reset speed
        player->setSlowDown(true);

        scoreRect.x = 297;
        std::string lose = "Lose";
        scoreMessage = TTF_RenderText_Blended(fontTTF,
                                              lose.c_str(),
                                              textColour);
        scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage);
        SDL_FreeSurface( scoreMessage );
      }
    }
    else     //if not then start
    {
      if (player->getSpeed() <= 0.15f)
      {
        std::cout << "\nstart level\n";

        TheSoundMixer::Instance()->playSound("start",0);

        hasMadeContactWithTargetPlanet = false;
        gameStarted = true;
        targetsRemaining = levelReached;

        //set player face
        player->setRow(2);

        //set sun's face
        sun->setFrame(1);
        
        //update text
        if( scoreText != NULL ) SDL_DestroyTexture( scoreText );

        scoreRect.x = 302;
        scoreMessage = TTF_RenderText_Blended(fontTTF,
                                              std::to_string(targetsRemaining).c_str(),
                                              textColour);
        scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage);
        SDL_FreeSurface( scoreMessage );
      
        player->setSpeedUp(true);
        getNewTarget();
      }
    }

    if (gameStarted)
      player->changeOrbitDirection(); 
  }
}

void GameState::onEnter()
{
  loadSounds();
  
  gameStarted = false;
  levelReached = 1;

  hasMadeContactWithTargetPlanet = false;
  targetsRemaining = 0;
  targetPlanetIndex = -1;

  initText();


 TheTextureManager::Instance()->load("assets/background.png", "background", TheGame::Instance()->getRenderer());
  
  background = new GameObject(256, 256,
                       512, 512,
                       "background", 1);

  TheTextureManager::Instance()->load("assets/speechBubble.png", "speechBubble", TheGame::Instance()->getRenderer());
  
  speechBubble = new GameObject(315, 200,
                       64, 64,
                       "speechBubble", 1);
  
  TheTextureManager::Instance()->load("assets/sun.png", "sun", TheGame::Instance()->getRenderer());
  
  sun = new GameObject((TheGame::Instance()->getScreenWidth()/2), (TheGame::Instance()->getScreenWidth()/2),
                       128, 128,
                       "sun", 1);

  TheTextureManager::Instance()->load("assets/sunAnimation.png", "sunAnimation", TheGame::Instance()->getRenderer());
  sunAnimation = new GameObject((TheGame::Instance()->getScreenWidth()/2), (TheGame::Instance()->getScreenWidth()/2),
                       128, 128,
                       "sunAnimation", 1);

  TheTextureManager::Instance()->load("assets/moon.png", "moon", TheGame::Instance()->getRenderer());
  
  OrbitingObject* moon = new OrbitingObject(0, (TheGame::Instance()->getScreenWidth()/2),
                            64, 64,
                            "moon", 1,
                            0, true, 0.04f);
  
  TheTextureManager::Instance()->load("assets/planet01.png", "planet01", TheGame::Instance()->getRenderer());
  OrbitingObject* planet01 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet01", 1,
                                                0, true, 0.08f);
  targets.push_back(planet01);

  TheTextureManager::Instance()->load("assets/planet04.png", "planet04", TheGame::Instance()->getRenderer());
  OrbitingObject* planet04 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet04", 0,
                                                140, true, 0.12f);
  planet04->setMoon(moon);
  targets.push_back(planet04);

  TheTextureManager::Instance()->load("assets/planet03.png", "planet03", TheGame::Instance()->getRenderer());
  OrbitingObject* planet03 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                192, 64,
                                                "planet03", 1,
                                                45, false, 0.12f);
  targets.push_back(planet03);

  TheTextureManager::Instance()->load("assets/planet02.png", "planet02", TheGame::Instance()->getRenderer());
  OrbitingObject* planet02 = new OrbitingObject(0, (TheGame::Instance()->getScreenHeight()/2),
                                                64, 64,
                                                "planet02", 1,
                                                95, true, 0.08f);
  targets.push_back(planet02);

  TheTextureManager::Instance()->load("assets/player.png", "player", TheGame::Instance()->getRenderer());
  player = new Player(0, (TheGame::Instance()->getScreenHeight()/2),
                      64, 64,
                      "player", 1,
                      143, true, 0.1f);
}


void GameState::onExit()
{

  if( scoreText != NULL ) SDL_DestroyTexture( scoreText );
  if( levelText != NULL ) SDL_DestroyTexture( levelText );
  if( pressToPlayText != NULL ) SDL_DestroyTexture( pressToPlayText );

  TTF_CloseFont( fontTTF );
  fontTTF = NULL;
  TTF_Quit();

  
  for (GameObject *obj: targets)
  {
    //delete from targets
    obj->clean();
    delete obj;
  }  
  targets.clear();

  delete background;

  delete speechBubble;
  delete player;

  delete sun;
  delete sunAnimation;

  //Mix_FreeChunk( scratch );

  Mix_CloseAudio();

  
  //TheTextureManager::Instance()->clearFromTextureMap("ball");

}

void GameState::getNewTarget()
{
  //std::cout << "getNewTarget()\n";
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
      //std::cout << targets[randomTargetIndex]->getPosition().calcDistance(player->getPosition()) << ": found target\n";

      targetPlanetIndex = randomTargetIndex;
      targets[targetPlanetIndex]->setFrame(1);
      foundTarget = true;
      return;
    }
    else
    {
      targetPlanetIndex = -1;
      std::cout << targets[randomTargetIndex]->getPosition().calcDistance(player->getPosition()) << "\n";
    }
  } while (foundTarget == false && targetsToAttempt.size() > 0);
        std::cout << "no viable target found\n";

}

void GameState::drawTargetOutline()
{

  int frame = int(((SDL_GetTicks() / 100) % 2));
  
  targets[targetPlanetIndex]->getPosition();

  if (frame == 0)
  {
  filledCircleRGBA(TheGame::Instance()->getRenderer(),
                   targets[targetPlanetIndex]->getPosition().getX(), targets[targetPlanetIndex]->getPosition().getY(),
                   50,
                   255, 0, 0, 100);
  }
  else
  {
  filledCircleRGBA(TheGame::Instance()->getRenderer(),
                   targets[targetPlanetIndex]->getPosition().getX(), targets[targetPlanetIndex]->getPosition().getY(),
                   50,
                   0, 0, 255, 100);
  }
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
  
  fontTTF = TTF_OpenFont("Assets/outerSpace.ttf", 24);
  if (fontTTF == NULL) {
    fprintf(stderr, "error: font not found\n");
  }

  scoreMessage = TTF_RenderText_Solid(fontTTF, "HI", textColour); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  scoreText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), scoreMessage); 
  scoreRect.x = 302;  //controls the rect's x coordinate 
  scoreRect.y = 180; // controls the rect's y coordinte
  scoreRect.w = 40; // controls the width of the rect
  scoreRect.h = 40; // controls the height of the rect

  levelMessage = TTF_RenderText_Solid(fontTTF, "Level 01", textColourWhite); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  levelText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), levelMessage); 
  levelRect.x = 200;  //controls the rect's x coordinate 
  levelRect.y = 5; // controls the rect's y coordinte
  levelRect.w = 100; // controls the width of the rect
  levelRect.h = 27; // controls the height of the rect

  pressToPlayMessage = TTF_RenderText_Solid(fontTTF, "press Space to play", textColourWhite); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  pressToPlayText = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), pressToPlayMessage); 
  pressToPlayRect.x = 125;  //controls the rect's x coordinate 
  pressToPlayRect.y = 470; // controls the rect's y coordinte
  pressToPlayRect.w = 270; // controls the width of the rect
  pressToPlayRect.h = 27; // controls the height of the rect
}

void GameState::loadSounds()
{
  TheSoundMixer::Instance()->load("assets/lose.wav", "lose", SOUND_SFX);
  TheSoundMixer::Instance()->load("assets/win.wav", "win", SOUND_SFX);
  TheSoundMixer::Instance()->load("assets/correctlyPressSpace.wav", "space", SOUND_SFX);
  TheSoundMixer::Instance()->load("assets/startGame.wav", "start", SOUND_SFX);

}
