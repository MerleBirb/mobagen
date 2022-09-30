#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world)
{
  auto cat = world->getCat();
  std::map<int, std::map<int, bool>> visited;

  std::vector<queueEntry> queue;
  // bootstrap the first element into the queue
  queue.push_back({cat, 0});

  // while we have elements to be visited, visit them
  while (!queue.empty())
  {
    // fetching the first element
    auto head = queue[0];
    queue.erase(queue.begin());

    // mark the head as visited
    // for each neighbor
    // - check if is not visited yet
    // - check if is not in the queue
    // - check if it can be visited
    // - add to the queue adding 1 to the weight
    // - mark the neighbor as it came from the current head we popped
    

    // here you code your win condition and whatever your navigation process is
  }

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
