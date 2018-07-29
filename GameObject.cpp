#include "GameObject.h"

GameObject::GameObject( int x, int y, int p_width, int p_height, std::string p_textureID, int p_numFrames) :
    position(x,y), width(p_width), height(p_height), textureID(p_textureID), numFrames(p_numFrames), velocity(0,0), acceleration(0,0), currentFrame(0), currentRow(1)
{

}

void GameObject::draw()
{
  TheTextureManager::Instance()->draw(textureID, (int)position.getX()- width/2, (int)position.getY() -height/2, width, height, /*m_currentRow, m_currentFrame,*/ TheGame::Instance()->getRenderer(), SDL_FLIP_NONE);
}

void GameObject::drawFrame()
{
  TheTextureManager::Instance()->drawFrame(textureID, (int)position.getX()- width/2, (int)position.getY() -height/2, width, height, /*m_currentFrame,*/ currentFrame, TheGame::Instance()->getRenderer(), currentRow);

}


void GameObject::update()
{
  velocity += acceleration;
  position += velocity;
};

void GameObject::clean()
{
}
