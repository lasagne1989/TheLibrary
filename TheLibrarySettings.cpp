#include "pch.h"
#include "TheLibrary.h"


void TheLibrary::RenderSettings() {
    ImGui::TextUnformatted("Set up your reading level");

    CVarWrapper enableCvar = cvarManager->getCvar("lib_enabled");
    if (!enableCvar) { return; }
    bool enabled = enableCvar.getBoolValue();
    if (ImGui::Checkbox("Enable plugin", &enabled)) {
        enableCvar.setValue(enabled);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Toggle The Library");
    }

    CVarWrapper modeCvar = cvarManager->getCvar("lib_mode");
    if (!modeCvar) { return; }
    std::string libMode = modeCvar.getStringValue();
    static int f = 1;
    std::string timeMode = "time";
    std::string jumpMode = "jump";
    if (ImGui::RadioButton("Time Mode", &f, 0)) {
        modeCvar.setValue(timeMode);
    }; ImGui::SameLine();
    if (ImGui::RadioButton("Jump Mode", &f, 1)) {
        modeCvar.setValue(jumpMode);
    };

    CVarWrapper timeCvar = cvarManager->getCvar("lib_time");
    if (!timeCvar) { return; }
    float timeDelay = timeCvar.getFloatValue();
    static int e = 1;
    float easyMode = 1.5f;
    float medMode = 1.0f;
    float hardMode = 0.5f;
    float insaneMode = 0.25f;
    if (ImGui::RadioButton("Easy (1.5s)", &e, 0)) {
        timeCvar.setValue(easyMode);
    }; ImGui::SameLine();
    if (ImGui::RadioButton("Medium (1.0s)", &e, 1)) {
        timeCvar.setValue(medMode);
    }; ImGui::SameLine();
    if (ImGui::RadioButton("Hard (0.5s)", &e, 2)) {
        timeCvar.setValue(hardMode);
    }; ImGui::SameLine();
    if (ImGui::RadioButton("Insane (0.25s)", &e, 3)) {
        timeCvar.setValue(insaneMode);
    };

    CVarWrapper bonkersCvar = cvarManager->getCvar("bonkers_enabled");
    if (!bonkersCvar) { return; }
    bool bonkers = bonkersCvar.getBoolValue();
    if (ImGui::Checkbox("Bonkers Mode", &bonkers)) {
        bonkersCvar.setValue(bonkers);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("I'm not sure why you would do this to yourself");
    }

    CVarWrapper ballcamCvar = cvarManager->getCvar("ballcamtoggle_enabled");
    if (!ballcamCvar) { return; }
    bool ballcamToggle = ballcamCvar.getBoolValue();
    if (ImGui::Checkbox("Automatially Toggle Ball Cam", &ballcamToggle)) {
        ballcamCvar.setValue(ballcamToggle);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("choose whether Ball Cam automatically turns off on disappear or not");
    }

}
