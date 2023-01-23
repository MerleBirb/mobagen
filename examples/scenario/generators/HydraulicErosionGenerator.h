#ifndef MOBAGEN_HYDRAULIC_H
#define MOBAGEN_HYDRAULIC_H

#include "../GeneratorBase.h"

class HydraulicErosionGenerator : public ScenarioGeneratorBase
{
private:


public:

	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() { return "Merle's Water Erosion"; }
};


#endif // MOBAGEN_HYDRAULIC_H