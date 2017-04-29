#include <vector>
#include <list>
#include "chunk.h"
#include "multiSprite.h"

class ExplodingSprite : public MultiSprite {
public:
  ExplodingSprite(const MultiSprite* s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  bool done() { return chunks.empty(); }
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};
