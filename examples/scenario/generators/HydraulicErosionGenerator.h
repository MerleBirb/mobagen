#ifndef MOBAGEN_HYDRAULIC_H
#define MOBAGEN_HYDRAULIC_H

#include "../GeneratorBase.h"
#include "Vector2.h"
#include "Point2D.h"

class HydraulicErosionGenerator : public ScenarioGeneratorBase
{
private:

	// constants
	float const RAINFALL = 5.0f;

	// source data
	float elevation[2048][2048];
    float water[2048][2048];
    float material[2048][2048];
    float previousMaterial[2048][2048];

	// calculated data
    Point2D waterFlow[2048][2048];
    float totalFlow[2048][2048];
    Point2D waterVelocity[2048][2048];

	float frequency = 1.0f;

	// functions
	void rain(int x, int y);
    void calculateFlow(int x, int y);

public:

	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() { return "Merle's Water Erosion"; }
};


#endif // MOBAGEN_HYDRAULIC_H