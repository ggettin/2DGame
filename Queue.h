#include <queue>
#include <iostream>


class Queue{

private:
  std::queue<unsigned int> q;
  const unsigned int maxSize;



public:
  Queue();
  Queue(const Queue&) = delete;
  Queue& operator=(const Queue&) = delete;

  void push(unsigned int);
  unsigned int pop();
  unsigned int size() const;
  unsigned int front() const;
  unsigned int back() const;
  unsigned int getMaxSize() const;
  bool empty() const;



};
