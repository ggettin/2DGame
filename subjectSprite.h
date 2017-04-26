#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "smartSprite.h"
#include "Player.h"

class SubjectSprite : public Player {
public:
  SubjectSprite(const std::string&);
  // SubjectSprite(const std::string&, const Vector2f& pos,
  //               const Vector2f& vel, const std::vector<Frame*>& fms);
  SubjectSprite(const SubjectSprite&);

  void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );

private:
  std::list<SmartSprite*> observers;
  SubjectSprite& operator=(const SubjectSprite&);
};
