#include "World.h"
#include "imgui.h"
#include "Utils.h"
#include "ImGuiExtra.h"
#include "Random.h"

#include "behaviours/SeparationRule.h"
#include "behaviours/CohesionRule.h"
#include "behaviours/AlignmentRule.h"
#include "behaviours/MouseInfluenceRule.h"
#include "behaviours/BoundedAreaRule.h"
#include "behaviours/WindRule.h"

#if defined(_WIN32)
#include "windows.h"
#include "psapi.h"
#endif


using namespace utils;

World::World(Engine *pEngine): GameObject(pEngine) {
    initializeRules();
    setNumberOfBoids(nbBoids);
    applyFlockingRulesToAllBoids();
}

void World::initializeRules()
{
    //Starting Rules
    boidsRules.emplace_back(std::make_unique<SeparationRule>(25., 4.75));
    boidsRules.emplace_back(std::make_unique<CohesionRule>(4.25));
    boidsRules.emplace_back(std::make_unique<AlignmentRule>(2.9));
    boidsRules.emplace_back(std::make_unique<MouseInfluenceRule>(2.));
    // todo: make this call engine and every frame to keep in sync with the real size. Cache it on window
    boidsRules.emplace_back(std::make_unique<BoundedAreaRule>(
            engine->window->size().y, engine->window->size().x, 80, 3.5, false));
    boidsRules.emplace_back(std::make_unique<WindRule>(1., 6.f, false));

    //Starting weights are saved as defaults
    defaultWeights = new float[boidsRules.size()];
    int i = 0;
    for (const auto& rule : boidsRules) {
        defaultWeights[i++] = rule->weight;
    }
}

void World::applyFlockingRulesToAllBoids()
{
    for (const auto& boid : boids)
    {
        boid->setFlockingRules(boidsRules);
    }
}

void World::setNumberOfBoids(int number)
{
    auto diff = boids.size() - number;

    if (diff == 0)
    {
        return;
    }
        //Need to add boids
    else if (diff < 0)
    {
        //Back to positive
        diff = -diff;

        //Add boids equal to diff
        for (int i = 0; i < diff; i++)
        {
            BoidPtr boidPtr = createBoid();
            cachedBoids.push_back(boidPtr.get()); //lookup list
            boids.push_back(std::move(boidPtr)); //owning list
        }
    }
        //Too much boid, remove them
    else
    {
        //Remove from end
        for (int i = 0; i < diff; i++)
        {
            boids.pop_back();
            cachedBoids.pop_back();
        }

    }
}

void World::randomizeBoidPositionAndVelocity(Boid* boid) {
    // todo: give boid a transform to store position, rotation... etc
    boid->setPosition(Vector2::Random(engine->window->size().x, engine->window->size().y));

    boid->setVelocity(Vector2().Rotate(Random::Range(0,360)) * desiredSpeed); //Random dir
}

void World::warpParticleIfOutOfBounds(Particle* particle) {
    //Correct position with windows borders
    Vector2 position = particle->getTransform().position;
    Vector2 sizeWindow = engine->window->size(); // todo: make it int

    if (position.x < 0) {
        position.x += sizeWindow.x;
    }
    else if (position.x > sizeWindow.x) {
        position.x -= sizeWindow.x;
    }

    if (position.y < 0) {
        position.y += sizeWindow.y;
    }
    else if (position.y > sizeWindow.y) {
        position.y -= sizeWindow.y;
    }

    //If the position changed
    if (position != particle->getShape().getPosition())
    {
        particle->setPosition(position);
    }
}

BoidPtr World::createBoid() {
    //Create new boid
    BoidPtr boid = std::make_unique<Boid>(&cachedBoids); //TODO : CHANGE

    randomizeBoidPositionAndVelocity(boid.get());
    boid->setFlockingRules(boidsRules);
    boid->setDetectionRadius(detectionRadius);
    boid->setSpeed(desiredSpeed);
    boid->setHasConstantSpeed(hasConstantSpeed);
    boid->drawAcceleration = showAcceleration;
    boid->drawDebugRadius = showRadius;
    boid->drawDebugRules = showRules;

    return boid;
}

std::vector<Boid*>* World::getAllBoids()
{
    return &cachedBoids;
}

void World::drawGeneralUI()
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once); //Next header is opened by default
    if (ImGui::CollapsingHeader("General"))
    {

        if (ImGui::DragInt("Number of Boids", &nbBoids))
        {
            if (nbBoids < 0)
                nbBoids = 0;
            setNumberOfBoids(nbBoids);
        }

        ImGui::SameLine(); HelpMarker("Drag to change the weight's value or CTRL+Click to input a new value.");

        if (ImGui::SliderFloat("Neighborhood Radius", &detectionRadius, 0.0f, 250.0f, "%.f"))
        {
            for (const auto& boid : boids)
            {
                boid->setDetectionRadius(detectionRadius);
            }
        }

        //Speeds

        ImGui::SetNextItemOpen(false, ImGuiCond_Once);
        if (ImGui::TreeNode("Movement Settings"))
        {

            if (ImGui::Checkbox("Has Constant Speed", &hasConstantSpeed))
            {
                for (const auto& boid : boids)
                {
                    boid->setHasConstantSpeed(hasConstantSpeed);
                }
            }

            const char* speedLabel = hasConstantSpeed ? "Speed" : "Max Speed";
            if (ImGui::SliderFloat(speedLabel, &desiredSpeed, 0.0f, 300.0f, "%.f"))
            {
                for (const auto& boid : boids)
                {
                    boid->setSpeed(desiredSpeed);
                }
            }

            //Acceleration

            if (ImGui::Checkbox("Has Max Acceleration", &hasMaxAcceleration))
            {
                for (const auto& boid : boids)
                {
                    if (hasMaxAcceleration)
                    {
                        boid->setMaxAcceleration(maxAcceleration);
                    }
                    else {
                        boid->setMaxAcceleration(10000.);
                    }
                }
            }
            ImguiTooltip("Boids keeps more momentum when the acceleration is capped.");

            if (hasMaxAcceleration)
            {
                if (ImGui::SliderFloat("Max Acceleration", &maxAcceleration, 0.0f, 35.0f, "%.f"))
                {
                    for (const auto& boid : boids)
                    {
                        boid->setMaxAcceleration(maxAcceleration);
                    }
                }
            }

            ImGui::TreePop();
        }


        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Display Settings"))
        {

            if (ImGui::Checkbox("Show Acceleration", &showAcceleration))
            {
                for (const auto& boid : boids)
                {
                    boid->drawAcceleration = showAcceleration;
                }
            }

            if (ImGui::Checkbox("Show Radius", &showRadius))
            {
                for (const auto& boid : boids)
                {
                    boid->drawDebugRadius = showRadius;
                }
            }

            if (ImGui::Checkbox("Show Rules", &showRules))
            {
                for (const auto& boid : boids)
                {
                    boid->drawDebugRules = showRules;
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::Button("Randomize Boids position and velocity"))
        {
            for (const auto& boid : boids)
            {
                randomizeBoidPositionAndVelocity(boid.get());
            }
        }
    }
}

void World::drawRulesUI()
{
    if (ImGui::CollapsingHeader("Rules"))
    {
        for (auto& rule : boidsRules)
        {

            if (rule->drawImguiRule()) //display the UI and returns true if a value has been changed
            {
                applyFlockingRulesToAllBoids();
            }
            ImGui::Separator();

        }

        if (ImGui::Button("Restore Default Weights"))
        {
            int i = 0;
            //restore default values
            for (auto& rule : boidsRules)
            {
                rule->weight = defaultWeights[i++];
            }
            applyFlockingRulesToAllBoids();
        }

        ImGui::Spacing();
    }
}

void World::Update(float deltaTime) {
    // todo: when the boid become a GameObject, we wont need this
    for (auto& b : boids) {
        b->update(deltaTime);
    }

    // move the first boid
    if (engine->getInputArrow() != Vector2::zero() && getNbBoids() > 0)
    {
        Boid* firstBoid = *getAllBoids()->begin();
        firstBoid->applyForce(engine->getInputArrow() * 20.f);
    }
}

void World::updatePositions(float deltaTime)
{
    for (auto& b : boids) {
        b->updatePosition(deltaTime);
        warpParticleIfOutOfBounds(b.get());
    }
}

int World::getNbBoids() const {
    return nbBoids;
}


void World::showConfigurationWindow()
{
    //Resized once at first windows appearance
    ImGui::SetNextWindowPos(ImVec2(850, 20), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(320, 550), ImGuiCond_Once);

    ImGui::Begin("Configuration"); // begin window

    ImGui::Text("Control the simulation with those settings.");
    ImGui::Spacing();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);

    drawGeneralUI();

    //ImGui::SetNextItemOpen(true, ImGuiCond_Once); //Next header is opened by default

    drawRulesUI();

    drawPerformanceUI();

    ImGui::End(); // end window
}

void World::drawPerformanceUI()
{
    if (ImGui::CollapsingHeader("Performance"))
    {
        //The  functions called are Windows specific
#if defined(_WIN32)
        ///FPS Count
        float framePerSecond = 1. / deltaTime.asSeconds();
        ImGui::Text("Frames Per Second (FPS) : %.f", framePerSecond);
        PlotVar("Frame duration (ms)", deltaTime.asMilliseconds());
        ImGui::Separator();

        ///CPU and RAM
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

        //Total Virtual Memory
        std::int64_t totalVirtualMem = memInfo.ullTotalPageFile;

        //Virtual Memory currently used
        std::int64_t virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

        //Virtual Memory current process
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        std::size_t virtualMemUsedByMe = pmc.PrivateUsage;

        //Total RAM
        std::int64_t totalPhysMem = memInfo.ullTotalPhys;

        //Ram Currently Used
        std::int64_t physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

        //Ram Current used by Process
        std::size_t physMemUsedByMe = pmc.WorkingSetSize;

        int div = 1048576;

        //PC info

        ImGui::Text("Total Virtual Memory : %uMb \n", totalVirtualMem / div);
        ImGui::Text("Total RAM : %uMb \n", totalPhysMem / div);

        //ImGui::Text("Virtual Memory Currently Used : %iMb \n", virtualMemUsed / div);
        //ImGui::Text("RAM Currently Used : %uMb \n", physMemUsed / div);
        //ImGui::Separator();

        ImGui::Text("Virtual Memory used by process : %uMb \n", virtualMemUsedByMe / div);
        PlotVar("Virtual Memory Consumption (Mb)", virtualMemUsedByMe / div);

        ImGui::Text("RAM used by process : %uMb \n", physMemUsedByMe / div);
        PlotVar("Ram Consumption (Mb)", physMemUsedByMe / div);

#else
        ImGui::Text("Only implemented on Windows.");
#endif
    }
}
