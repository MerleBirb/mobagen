#include "ImprovedGenerator.h"
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>

std::vector<Color32> ImprovedGenerator::Generate(int sideSize, float displacement)
{
      const int WATER_LEVEL = 75;
      const int SAND_LEVEL = 80;
      const int GRASS_LEVEL = 130;
      const int SNOW_LEVEL = 255;

      const int DISP_MULTIPLIER = 50;

	std::vector<Color32> colors;

    // create the noise cells
	FastNoiseLite perlin;
    perlin.SetFractalOctaves(3);
    perlin.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    perlin.SetFractalType(FastNoiseLite::FractalType_FBm);
    FastNoiseLite cellular;
    cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    cellular.SetFractalOctaves(3);

    // loop through the noise cells
    for (int l = 0; l < sideSize; l++)
    {
        for (int c = 0; c < sideSize; c++)
        {
            // noise values generated between 0 - 1
            float c1 = ((perlin.GetNoise((float)c, (float)l, displacement * DISP_MULTIPLIER) + 1) / 2) * 255;
            float c2 = ((cellular.GetNoise((float)c, (float)l, displacement * DISP_MULTIPLIER) + 1) / 2) * 255;
            auto avg = (c1 + c2) / 2;

            // 3 colors
            float red;
            float green;
            float blue;

            // heightmap biome generation using altitude
            if (avg < WATER_LEVEL)  // water level
            {
                red = 0;
                green = 0;
                blue = avg;
            }
            else if (avg >= WATER_LEVEL && avg < SAND_LEVEL) // sand level
            {
                red = avg;
                green = avg;
                blue = 0;
            }
            else if (avg >= SAND_LEVEL && avg < GRASS_LEVEL) // grass level
            {
                red = avg / 4;
                green = avg;
                blue = avg / 4;
            }
            else if (avg >= GRASS_LEVEL) // snow level
            {
                red = avg;
                green = avg;
                blue = avg;
            }

            colors.emplace_back(red, green, blue);  // (red, green, blue)
        }
    }

    std::cout << colors.size() << std::endl;

	return colors;
}

std::string ImprovedGenerator::GetName() { return "MERLE'S IMPROVED GENERATOR"; }