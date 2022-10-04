#ifndef AGENT_H
#define AGENT_H
#include "Point2D.h"
#include <map>

class World;

struct queueEntry
{
  queueEntry() = default;
  Point2D position;
  int weight;
  bool operator<(const queueEntry& rhs) const // rhs = right hand side
  {
	  return this->weight < rhs.weight;
  }
};

class Agent
{
public:
  explicit Agent()= default;;
  virtual Point2D Move(World*)=0;

  std::map<int, std::map<int, bool>> visited;

  std::map<int, std::map<int, queueEntry>> BuildWorld(World*);
};

#endif  // AGENT_H
