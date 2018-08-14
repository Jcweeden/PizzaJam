#ifndef _InputHandler_
#define _InputHandler_

#include "SDL2/SDL.h"
#include "Game.h"


#include <iostream>
#include <vector>

class InputHandler
{
  
public:
  static InputHandler* Instance();

  void update(); //collects new input
  void clean();  //remove init joysticks - //no joysticks - unneeded


  //returns true if parameter key is pressed
  bool isKeyDown(SDL_Scancode key);

  bool getIsSpaceKeyPressed()
  {
    if (isSpaceKeyPressed)
    {
      isSpaceKeyPressed = false;
      return true;
    }
    else
    {
    return isSpaceKeyPressed;
    }
  }
  
private:

  InputHandler(){isSpaceKeyPressed = false;};
  
  static InputHandler* s_pInstance;


  //keys
  const Uint8* m_keystates;
  bool isSpaceKeyPressed;
  
};

typedef InputHandler TheInputHandler;

#endif
