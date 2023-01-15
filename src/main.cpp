#include "main.hpp"
#include "config.hpp"

static ModInfo modInfo;

static int numMistakes = 0;

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ScoreModel_NoteScoreDefinition.hpp"

using namespace GlobalNamespace;

int CalculateAccuracy(NoteCutInfo& noteCutInfo) {
	auto definition = ScoreModel::GetNoteScoreDefinition(noteCutInfo.noteData->scoringType);
	return round(definition->maxCenterDistanceCutScore * (1 - std::clamp(noteCutInfo.cutDistanceToCenter / 0.3, 0.0, 1.0)));
}

#include "GlobalNamespace/CoreGameHUDController.hpp"

MAKE_HOOK_MATCH(CoreGameHUDController_Start, &CoreGameHUDController::Start, void, CoreGameHUDController* self) {

    numMistakes = 0;

    CoreGameHUDController_Start(self);
}

#include "GlobalNamespace/NoteController.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/StandardLevelFailedController.hpp"
#include "GlobalNamespace/StandardLevelRestartController.hpp"

MAKE_HOOK_MATCH(NoteController_SendNoteWasCutEvent, &NoteController::SendNoteWasCutEvent, void, NoteController* self, ByRef<NoteCutInfo> noteCutInfo) {

    if(noteCutInfo->get_allIsOK() && getConfig().Enable.GetValue() && CalculateAccuracy(*noteCutInfo) < getConfig().MinAcc.GetValue()) {
        if(numMistakes < getConfig().Mistakes.GetValue())
            numMistakes++;
        else {
            switch(getConfig().Action.GetValue()) {
            case 0:
                return;
            case 1:
                noteCutInfo->directionOK = false;
                break;
            case 2:
                UnityEngine::Resources::FindObjectsOfTypeAll<StandardLevelFailedController*>().First()->HandleLevelFailed();
                break;
            case 3:
                UnityEngine::Resources::FindObjectsOfTypeAll<StandardLevelRestartController*>().First()->RestartLevel();
                break;
            }
        }
    }
    NoteController_SendNoteWasCutEvent(self, noteCutInfo);
}

extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getLogger().info("Completed setup!");
}

#include "questui/shared/QuestUI.hpp"

extern "C" void load() {
    il2cpp_functions::Init();

    getConfig().Init(modInfo);

    QuestUI::Init();
    QuestUI::Register::RegisterModSettingsViewController(modInfo, ModSettings);

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), CoreGameHUDController_Start);
    INSTALL_HOOK(getLogger(), NoteController_SendNoteWasCutEvent);
    getLogger().info("Installed all hooks!");
}
