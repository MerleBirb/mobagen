#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world)
{
  // A. producer and consumer queue to build the map of cameFrom
  // B. navigate through the map of cameFrom in the moment we find an exit and return the path

  auto cat = world->getCat();
  std::map<int, std::map<int, bool>> cameFrom;

  std::vector<queueEntry> frontier;
  // bootstrap the first element into the queue
  frontier.push_back({cat, 0});

  // while we have elements to be visited, visit them
  while (!frontier.empty())
  {
    // fetching the first element
    auto head = frontier[0];
    frontier.erase(frontier.begin());

    // mark the head as visited
    // add neighbors to the queue, for each neighbor
    // - check if is not visited yet
    // - check if is not in the queue
    // - check if it is not blocked
    // - add to the queue adding 1 to the weight
    // - mark head position as it cameFrom
    // visit head neighbors

    // if neighbor is exit, exit point = head position

    // here you code your win condition and whatever your navigation process is
  }

  // populate the path to be returned
  // iterate from the exit point up to the origin and build the path
  // the first element is the cat move and the last is the catcher

  auto rand = Random::Range(0,5);
  auto pos = world->getCat();
  switch(rand){
    case 0:
      return World::NE(pos);
    case 1:
      return World::NW(pos);
    case 2:
      return World::E(pos);
    case 3:
      return World::W(pos);
    case 4:
      return World::SW(pos);
    case 5:
      return World::SE(pos);
    default:
      throw "random out of range";
  }
}
