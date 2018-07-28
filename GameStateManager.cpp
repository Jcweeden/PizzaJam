#include "GameStateManager.h"

std::vector<GameState*> gameStates;


void GameStateManager::update()
{
  if (!gameStates.empty()){
    gameStates.back()->update();
  }
}

void GameStateManager::render()
{
  if (gameStates.empty() == false){
    gameStates.back()->render();
  }
}

void GameStateManager::handleInput()
{
  if (gameStates.empty() == false){
    gameStates.back()->handleInput();
  }
}


void GameStateManager::pushState(GameState* pState)
{
  gameStates.push_back(pState); //add 
  gameStates.back()->onEnter(); //run state init
}

void GameStateManager::changeState(GameState* pState)
{
  if(!gameStates.empty()){
        if(gameStates.back()->getStringID() == pState->getStringID()){
            return; // do nothing if they are the same
        }

        if(gameStates.back()->getIsValid()){
            gameStates.back()->setIsValid(false); // Mark the state as invalid
        }
    }

    // push back our new state
    gameStates.push_back(pState);

    gameStates.back()->onEnter();
}

void GameStateManager::popState()
{
  if (gameStates.empty() == false){
    gameStates.back()->onExit();
    delete gameStates.back();
    gameStates.pop_back();
  }
}

void GameStateManager::dequeueState(){
    if(!gameStates.empty()){
        // If the state is invalid we proceed to dequeue the state
      if(!gameStates[0]->getIsValid()) {
        gameStates[0]->onExit();
            delete gameStates[0];
            gameStates.erase(gameStates.begin());

            // Reset the Input handler buttons state
            // This line is extremely important, fixes an issue with the "State traveling"
            // when a button is in the position of another button in another state
            // this will prevent the accident of traveling 2 states with 1 click.
            // TheInputHandler::Instance()->reset();
        }
    }
}
