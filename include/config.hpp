#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(Config,
    CONFIG_VALUE(Enable, bool, "Enbale Mod", true)
    CONFIG_VALUE(MinAcc, int, "Minimum Accuracy", 5, "Accuracy below this is causes the configured action")
    CONFIG_VALUE(Action, int, "Action On Mistake", 0, "What to do when an action is triggered")
    CONFIG_VALUE(Mistakes, int, "Allowed Mistakes", 0, "The number of mistakes that can occur with no action")
)

void ModSettings(class UnityEngine::GameObject* gameObject, bool firstActivation);
