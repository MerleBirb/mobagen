#include "HydraulicErosionGenerator.h"
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>

std::vector<Color32> HydraulicErosionGenerator::Generate(int sideSize, float displacement)
{
    std::vector<Color32> colors;
    
    // create the noise cells
    FastNoiseLite perlin;
    perlin.SetFractalOctaves(3);
    perlin.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    perlin.SetFractalType(FastNoiseLite::FractalType_FBm);
    FastNoiseLite cellular;
    cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    cellular.SetFractalOctaves(3);

    // loop through noise cells
    for (int l = 0; l < sideSize; l++)
    {
        for (int c = 0; c < sideSize; c++)
        {
            // island generation
            // goes between -1 up to 1
            // 0 is the middle
            float posY = (float)((l - sideSize / 2)) / ((float)sideSize / 2);
            float posX = (float)((c - sideSize / 2)) / ((float)sideSize / 2);
            float const islandInfluence = ((1 - posX * posX) * (1 - posY * posY)) * 255;

            // noise values generated between 0 - 255
            float const noiseInfluence = ((1 + perlin.GetNoise((float)l, (float)c, displacement * 50)) / 2) * 255;
            elevation[l][c] = ((frequency * (islandInfluence)) + (frequency * (noiseInfluence))) / 2;

            if (elevation[l][c] < 50)
            {
                colors.emplace_back(Color::DarkBlue);
            }
            else if (elevation[l][c] < 100)
            {
                colors.emplace_back(Color::Yellow);
            }
            else if (elevation[l][c] < 150)
            {
                colors.emplace_back(Color::Green);
            }
            else if (elevation[l][c] < 200)
            {
                colors.emplace_back(Color::Brown);
            }
            else
            {
                colors.emplace_back(Color::White);
            }

            //colors.emplace_back(avg, avg, avg);  // (red, green, blue)
        }
    }

    std::cout << colors.size() << std::endl;

    return colors;
}