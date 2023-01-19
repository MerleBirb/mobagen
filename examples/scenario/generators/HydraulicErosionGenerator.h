#ifndef MOBAGEN_HYDRAULIC_H
#define MOBAGEN_HYDRAULIC_H

#include "../GeneratorBase.h"

class HydraulicErosionGenerator : ScenarioGeneratorBase
{
private:

	int size;

	// source data
	

public:

	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() override;
};


#endif // MOBAGEN_HYDRAULIC_H