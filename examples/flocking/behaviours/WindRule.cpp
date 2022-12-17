#include "WindRule.h"
#include <imgui.h>
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"

Vector2 WindRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
    // todo: add a wind force here
    // hint: use the windAngle variable
    Vector2 windForce = Vector2::zero();

    windForce.x = (cos(windAngle) * boid->getPosition().x) - (sin(windAngle) * boid->getPosition().y);
    windForce.y = (sin(windAngle) * boid->getPosition().x) + (cos(windAngle) * boid->getPosition().y);
    windForce *= weight;

    return Vector2::normalized(windForce);
}

bool WindRule::drawImguiRuleExtra() {
    ImGui::SetCurrentContext(world->engine->imGuiContext);
    bool valueHasChanged = false;

    if (ImGui::SliderAngle("Wind Direction", &windAngle, 0)) {
        valueHasChanged = true;
    }

    return valueHasChanged;
}