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
            float const squareBump = ((1 - posX * posX) * (1 - posY * posY)) * 255;

            // noise values generated between 0 - 255
            float const noiseInfluence = ((1 + perlin.GetNoise((float)l, (float)c, displacement * 50)) / 2) * 255;
            auto e = 1 * ((0.25 * (squareBump)) + (0.25 * (noiseInfluence)));
            e += 0.5 * ((0.5 * (squareBump)) + (0.5 * (noiseInfluence)));
            e += 0.25 * ((1 * (squareBump)) + (1 * (noiseInfluence)));

            // multiple levels of noise
            elevation[l][c] = e / (1 + 0.5 + 0.25);



            if (elevation[l][c] < 100)
            {
                colors.emplace_back(Color::DarkBlue);
            }
            else if (elevation[l][c] < 110)
            {
                colors.emplace_back(Color::Yellow);
            }
            else if (elevation[l][c] < 125)
            {
                colors.emplace_back(Color::Green);
            }
            else if (elevation[l][c] < 150)
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

void HydraulicErosionGenerator::rain(int x, int y)
{
    water[x][y] += RAINFALL;
}

void HydraulicErosionGenerator::calculateFlow(int x, int y) {
    auto const NORTH = Point2D(0, 1);
    auto const SOUTH = Point2D(0, -1);
    auto const EAST = Point2D(1, 0);
    auto const WEST = Point2D(-1, 0);

    Point2D p(x, y);

    auto directions = {EAST, NORTH, WEST, SOUTH};

    // flows to each neighbor
    totalFlow[x][y] = 0;

    Point2D vec(0,0);

    auto deltaNorth = elevation[p.x][p.y] - elevation[NORTH.x + x][NORTH.y + y];
    auto deltaSouth = elevation[p.x][p.y] - elevation[SOUTH.x + x][SOUTH.y + y];
    auto deltaEast = elevation[p.x][p.y] - elevation[EAST.x + x][EAST.y + y];
    auto deltaWest = elevation[p.x][p.y] - elevation[WEST.x + x][WEST.y + y];

    auto deltaVertical = deltaNorth - deltaSouth;
    auto deltaHorizontal = deltaEast - deltaWest;

    vec.x = deltaHorizontal;
    vec.y = deltaVertical;

    totalFlow[x][y] += pow(((vec.x * vec.x) + (vec.y * vec.y)), 2);
    //waterFlow = vec;
}