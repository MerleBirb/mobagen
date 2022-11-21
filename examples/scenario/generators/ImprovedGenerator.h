#ifndef MOBAGEN_IMPROVEDGENERATOR_H
#define MOBAGEN_IMPROVEDGENERATOR_H

#include "../GeneratorBase.h"

class ImprovedGenerator : public ScenarioGeneratorBase
{
 public:

	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() override;
};

#endif  // MOBAGEN_IMPROVEDGENERATOR_H