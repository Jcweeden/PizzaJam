#ifndef _State_
#define _State_

#include <string>
#include <vector>


class State {

public:

  State();
  
  virtual void update() = 0;
  virtual void render() = 0;
  virtual void handleInput() = 0;
  
  virtual void onEnter() = 0;
  virtual void onExit() = 0;

  virtual std::string getStringID() = 0;

  bool getIsValid() const { return m_isValid; }
  void setIsValid(bool isValid) { m_isValid = isValid; }

  
private:
  bool m_isValid;
};


#endif
