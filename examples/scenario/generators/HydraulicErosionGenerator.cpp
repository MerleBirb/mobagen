#include "HydraulicErosionGenerator.h"
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>

std::vector<Color32> HydraulicErosionGenerator::Generate(int sideSize, float displacement)
{
    const int DISP_MULTIPLIER = 50; // displacement multiplier
    const float FREQUENCY = 1.0f;

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

            float islandInfluence = (2 - (abs(posY) + abs(posX))) / 2;
            islandInfluence *= 255;

            // noise values generated between 0 - 255
            float c1 = ((perlin.GetNoise((float)c, (float)l, displacement * DISP_MULTIPLIER) + 1) / 2) * 255;
            //float c2 = ((cellular.GetNoise((float)c, (float)l, displacement * DISP_MULTIPLIER) + 1) / 2) * 255;
            
            auto avg = (c1 + islandInfluence) / 2;

            if (avg < 50)
            {
                colors.emplace_back(Color::DarkBlue);
            }
            else if (avg < 100)
            {
                colors.emplace_back(Color::Yellow);
            }
            else if (avg < 150)
            {
                colors.emplace_back(Color::Green);
            }
            else if (avg < 200)
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