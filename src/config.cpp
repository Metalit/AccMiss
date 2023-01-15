#include "main.hpp"
#include "config.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/Touchable.hpp"

using namespace QuestUI;

const std::vector<std::string> actions = {
    "Miss",
    "Bad Cut",
    "Fail",
    "Restart"
};

void ModSettings(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if(!firstActivation)
        return;

    self->get_gameObject()->AddComponent<HMUI::Touchable*>();

    auto vertical = BeatSaberUI::CreateVerticalLayoutGroup(self);
    vertical->set_childControlHeight(false);
    vertical->set_childForceExpandHeight(false);
    vertical->set_spacing(1);

    AddConfigValueToggle(vertical, getConfig().Enable);

    AddConfigValueIncrementInt(vertical, getConfig().MinAcc, 1, 0, 15);

    AddConfigValueDropdownEnum(vertical, getConfig().Action, actions);

    AddConfigValueIncrementInt(vertical, getConfig().Mistakes, 1, 0, 10);
}
