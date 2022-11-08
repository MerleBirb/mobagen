#include "HuntAndKillGenerator.h"
#include "../World.h"
#include "Random.h"
#include <climits>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>

bool HuntAndKillGenerator::Step(World* w)
{
    // check if we need to find a new starting point
    if (stack.empty())
    {
        auto point = randomStartPoint(w);  // start from 0
        if (point.x == INT_MAX && point.y == INT_MAX)
            return false;  // no empty space no fill

        grid[point.x][point.y] = true; // set first point to true
        stack.push_back(point);
    }
    
    // visit the current element
    auto current = stack.back();

    // random walk -> hunt. If not random walking, hunt for an empty cell
    if (!walk(w, current))
    {
        hunt(w, current);
    }

    return true;
}

bool HuntAndKillGenerator::walk(World* w, Point2D point)
{
    // check if we should go deeper
    std::vector<Point2D> visitables = getVisitables(w, point); // points that are unvisited

    // if we should not go deep, pop one element from the stack
    if (visitables.empty())
    {
        stack.pop_back();
        grid[point.x][point.y] = true;
        w->SetNodeColor(point, Color::Black);
        return false;
    }
    else
    {
        // go deeper
        auto r = Random::Range(0, visitables.size() - 1);
        auto next = visitables[r];
        
        stack.push_back(next);
        auto delta = next - point;

        // remove walls
        if (delta.y == -1)  // north
          w->SetNorth(point, false);
        else if (delta.x == 1)  // east
          w->SetEast(point, false);
        else if (delta.y == 1)  // south
          w->SetSouth(point, false);
        else if (delta.x == -1)  // west
          w->SetWest(point, false);

        grid[point.x][point.y] = true;
        w->SetNodeColor(next, Color::Green);
        w->SetNodeColor(point, Color::Red.Dark());

        return true;
    }
}

bool HuntAndKillGenerator::hunt(World* w, Point2D point)
{
    auto sideOver2 = w->GetSize() / 2;

    for (int xx = -sideOver2; xx < sideOver2; xx++)
    {
        for (int yy = -sideOver2; yy < sideOver2; yy++)
        {
            // if cell isn't visited, aka false
            if (!grid[xx][yy])
            {
                auto previous = Point2D(xx, yy);
                std::vector<Point2D> unvisitables = getUnvisitables(w, previous); // getting every cell that HAS been visited
            
                if (!unvisitables.empty()) // if the list of visited cells is not empty
                {
                    // choose a random cell that has been visited before
                    // you want to connect a visited cell to a not visited cell to make sure the maze is ALWAYS solveable
                    auto r = Random::Range(0, unvisitables.size() - 1);
                    auto next = unvisitables[r];
                    
                    stack.push_back(previous);
                    auto delta = next - previous;
                
                    // remove walls
                    // the delta will always be 1 or -1 because the neighbor offset will always be either
                    if (delta.y == -1)  // north
                      w->SetNorth(previous, false);
                    else if (delta.x == 1)  // east
                      w->SetEast(previous, false);
                    else if (delta.y == 1)  // south
                      w->SetSouth(previous, false);
                    else if (delta.x == -1)  // west
                      w->SetWest(previous, false);
                
                    grid[xx][yy] = true;
                    grid[point.x][point.y] = true;
                    w->SetNodeColor(previous, Color::Blue);
                }

                return true;
            }
        }
    }

    return false;
}

void HuntAndKillGenerator::Clear(World* w)
{
    grid.clear();
    stack.clear();
    auto sideOver2 = w->GetSize() / 2;

    for (int i = -sideOver2; i <= sideOver2; i++)
    {
        for (int j = -sideOver2; j <= sideOver2; j++)
        {
            grid[i][j] = false;
        }
    }
}

Point2D HuntAndKillGenerator::randomStartPoint(World* w)
{
    auto sideOver2 = w->GetSize() / 2;
    
    for (int y = -sideOver2; y <= sideOver2; y++)
      for (int x = -sideOver2; x <= sideOver2; x++)
        if (!grid[y][x]) return {x, y};
    return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillGenerator::getVisitables(World* w, const Point2D& p)
{
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  // todo: improve this
  // north
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y - 1) <= sideOver2) &&  // should be inside the board
      !grid[p.x][p.y - 1] &&       // not visited yet
      w->GetNorth({p.x, p.y - 1}))    // has wall
    visitables.emplace_back(p.x, p.y - 1);
  // east
  if ((abs(p.x + 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      !grid[p.x + 1][p.y] &&       // not visited yet
      w->GetEast({p.x + 1, p.y}))  // has wall
    visitables.emplace_back(p.x + 1, p.y);
  // south
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y + 1) <= sideOver2) &&  // should be inside the board
      !grid[p.x][p.y + 1] &&          // not visited yet
      w->GetSouth({p.x, p.y + 1}))    // has wall
    visitables.emplace_back(p.x, p.y + 1);
  // west
  if ((abs(p.x - 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      !grid[p.x - 1][p.y] &&       // not visited yet
      w->GetWest({p.x - 1, p.y}))  // has wall
    visitables.emplace_back(p.x - 1, p.y);

  return visitables;
}

std::vector<Point2D> HuntAndKillGenerator::getUnvisitables(World* w, const Point2D& p)
{
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  // north
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y - 1) <= sideOver2) &&  // should be inside the board
      grid[p.x][p.y - 1] &&           // visited
      w->GetNorth({p.x, p.y - 1}))    // has wall
    visitables.emplace_back(p.x, p.y - 1);
  // east
  if ((abs(p.x + 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      grid[p.x + 1][p.y] &&        // visited
      w->GetEast({p.x + 1, p.y}))  // has wall
    visitables.emplace_back(p.x + 1, p.y);
  // south
  if ((abs(p.x) <= sideOver2 &&
       abs(p.y + 1) <= sideOver2) &&  // should be inside the board
      grid[p.x][p.y + 1] &&           // visited
      w->GetSouth({p.x, p.y + 1}))    // has wall
    visitables.emplace_back(p.x, p.y + 1);
  // west
  if ((abs(p.x - 1) <= sideOver2 &&
       abs(p.y) <= sideOver2) &&    // should be inside the board
      grid[p.x - 1][p.y] &&        // visited
      w->GetWest({p.x - 1, p.y}))  // has wall
    visitables.emplace_back(p.x - 1, p.y);

  return visitables;
}