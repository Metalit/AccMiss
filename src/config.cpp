#include "main.hpp"
#include "config.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/GameObject.hpp"

using namespace QuestUI;

const std::vector<std::string> actions = {
    "Miss",
    "Bad Cut",
    "Fail",
    "Restart"
};

void ModSettings(UnityEngine::GameObject* gameObject, bool firstActivation) {
    if(!firstActivation)
        return;

    auto vertical = BeatSaberUI::CreateVerticalLayoutGroup(gameObject);
    vertical->set_childControlHeight(false);
    vertical->set_childForceExpandHeight(false);
    vertical->set_spacing(1);
    vertical->get_rectTransform()->set_anchoredPosition({0, 31});

    AddConfigValueToggle(vertical, getConfig().Enable);

    AddConfigValueIncrementInt(vertical, getConfig().MinAcc, 1, 0, 15);

    AddConfigValueDropdownEnum(vertical, getConfig().Action, actions);

    AddConfigValueIncrementInt(vertical, getConfig().Mistakes, 1, 0, 10);
}
