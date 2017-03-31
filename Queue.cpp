#include "Queue.h"
#include "gamedata.h"


Queue::Queue() :
  q(), maxSize(Gamedata::getInstance().getXmlInt("queueMax")){}


void Queue::push(unsigned int num){
  if(q.size() == maxSize){
    q.pop();
  }
  q.push(num);
}

unsigned int Queue::pop(){
  int popped = q.front();
  q.pop();
  return popped;
}

unsigned int Queue::size() const {
  return q.size();
}

unsigned int Queue::front() const {
  return q.front();
}

unsigned int Queue::back() const {
  return q.back();
}

unsigned int Queue::getMaxSize() const {
  return maxSize;
}

bool Queue::empty() const {
  return q.empty();
}
