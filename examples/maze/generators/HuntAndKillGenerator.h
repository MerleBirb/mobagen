#ifndef HUNTKILL_H
#define HUNTKILL_H

#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include "Random.h"
#include <climits>
#include <vector>
#include <map>

class HuntAndKillGenerator : public MazeGeneratorBase
{
private:

	std::map<int, std::map<int, bool>> grid; // map of int and int bool to mark completed cells
    std::vector<Point2D> stack; // check cells 
    std::vector<Point2D> getVisitables(World* w, const Point2D& p); // get cells that havent been visited
    std::vector<Point2D> getUnvisitables(World* w, const Point2D& p); // get cells that have been visited
    bool walk(World* w, Point2D p); // random walk
    bool hunt(World* w, Point2D p); // random hunt
    Point2D randomStartPoint(World* world); // random start point, defaults to (-10, -10)

public:

	HuntAndKillGenerator() = default;
	std::string GetName() override { return "Hunt and Kill Generator"; };
	bool Step(World* world) override;
    void Clear(World* world) override;
};

#endif // HUNTKILL_H