#ifndef _TextureManager_
#define _TextureManager_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include <map>

class TextureManager
{
public:
  static TextureManager* Instance();

  void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip);

  void drawThreeWide(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip);

  
  bool load(std::string file, std::string mapID, SDL_Renderer* pRenderer);

  void clearFromTextureMap(std::string id);

  std::map<std::string , SDL_Texture*> textureMap;
  
private:
  
  TextureManager () {};

  static TextureManager* s_pInstance;
  
};

typedef TextureManager TheTextureManager;

#endif
