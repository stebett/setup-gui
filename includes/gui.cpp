//
// Created by ginko on 04/03/24.
//

#include "gui.h"
#include <imgui_impl_raylib.h>
#include "game.h"


import compress_videos;
import session_control;

constexpr ImVec4 red{1., 0., 0., 1.};
constexpr ImVec4 green{0., 1., 0., 1.};
ImGuiIO *Gui::m_io;

void controlWindow(Game &game) {
    ImGui::SeparatorText("Control Panel");
    ImGui::BeginTable("table1", 2);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Start Experiment")) { game.controls.Start(); }
    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Initialize Experiment")) { game.controls.Initialize(); }
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Switch Protocol")) { game.controls.SwitchProtocol(); }
    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Switch Subject")) {
        ImGui::OpenPopup("Choose Subject");
    }
    if (ImGui::BeginPopupModal("Choose Subject", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
        for (const auto subject: subjectList) {
            ImGui::PushID(subject);
            if (ImGui::Button(Session::enum2String(subject))) {
                game.session.SwitchSubject(subject);
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndPopup();
    }
    ImGui::EndTable();
}

void generalWindow(Game const &game) {
    ImGui::SeparatorText("General");
    static const auto date = Session::getDatePretty();
    ImGui::Text("Subject: %s", game.session.getSubject());
    ImGui::Text("Date: %s", date.c_str());
}

void statusWindow(Game &game) {
    ImGui::SeparatorText("Status");
    if (game.IsInitialized()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Experiment initialized");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Experiment not initialized");
    }
    if (game.IsRunning()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Experiment running");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Experiment not running");
    }
    if (game.IsRunning()) {
        ImGui::Text("Time from start: %f", game.getStartTime());
    } else {
        ImGui::Text("Time from start: 0");
    }
}

void zaberWindow(Game &game) {
    ImGui::SeparatorText("Zaber");
    if (game.zaber.isInitialized()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Zaber initialized");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Zaber not Initialized");
    }
    if (game.zaber.isRunning()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Zaber running");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Zaber not Running");
    }
    ImGui::Text("Protocol: %s", game.zaber.getProtocol().c_str());
    ImGui::Text("X position: %d", game.zaber.getX());
    ImGui::Text("Y position: %d", game.zaber.getY());
    ImGui::Text("Next X position: %d", game.zaber.getNextX());
    ImGui::Text("Next Y position: %d", game.zaber.getNextY());
    ImGui::Text("Time to next: %f", game.zaber.getSecondsToNext());
    ImGui::Text("Last answer: %s", game.zaber.getAnswer());
}

void ephysWindow(const Game &game) {
    ImGui::SeparatorText("Ephys");
    if (game.ephys.IsInitialized()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Ephys initialized");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Ephys not Initialized");
    }
    if (game.ephys.IsRunning()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Ephys running");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Ephys not Running");
    }
}

void camerasWindow(const Game &game) {
    ImGui::SeparatorText("Cameras");
    if (game.camera1Frontal.IsInitialized()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Camera 1 frontal initialized");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Camera 1 frontal not Initialized");
    }
    if (game.camera2Lateral.IsInitialized()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Camera 2 lateral initialized");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Camera 2 lateral not Initialized");
    }
    if (game.camera1Frontal.IsRunning()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Camera 1 frontal running");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Camera 1 frontal not Running");
    }
    if (game.camera2Lateral.IsRunning()) {
        ImGui::TextColored({0., 1., 0., 1.}, "Camera 2 lateral running");
    } else {
        ImGui::TextColored({1., 0., 0., 1.}, "Camera 2 lateral not Running");
    }
}

void dataWindow(Game const &game) {
    ImGui::SeparatorText("Data control");
    static int cam1Files = game.dataView.CointainedElements(game.dataView.cam1Path);
    static int cam2Files = game.dataView.CointainedElements(game.dataView.cam2Path);
    static int ephysFiles = game.dataView.CointainedElements(game.dataView.ephysRecordingPath);
    static ImVec4 color{};

    color = cam1Files > 0 ? green : red;
    ImGui::Text("Camera 1 Frontal path:\n\t%s", game.dataView.cam1Path.string().c_str());
    ImGui::TextColored(color, "Number of files contained: %i", cam1Files);
    // ImGui::Text("Expected number of files: %i", static_cast<int>(game.getStartTime()) * 500); //TODO fix this )

    color = cam2Files > 0 ? green : red;
    ImGui::Text("Camera 2 Lateral path:\n\t%s", game.dataView.cam2Path.string().c_str());
    ImGui::TextColored(color, "Number of files contained: %i", cam2Files);
    // ImGui::Text("Expected number of files: %i", static_cast<int>(game.getStartTime()) * 500); //TODO fix this )

    color = ephysFiles > 0 ? green : red;
    ImGui::Text("Ephys path:\n\t%s", game.dataView.ephysRecordingPath.string().c_str());
    ImGui::TextColored(color, "Number of files contained: %i", ephysFiles);
    // ImGui::Text("Expected number of files: %i", static_cast<int>(game.getStartTime()) * 500); //TODO fix this )
}

void manualWindow(Game &game) {
    ImGui::SeparatorText("Manual control");

    if (ImGui::Button("Zaber Initialize")) {
        game.zaber.initialize();
    }
    if (!game.zaber.isInitialized()) ImGui::BeginDisabled(true);
    if (ImGui::Button("Zaber Start")) {
        game.zaber.start();
    }
    static char message[32] = "";
    ImGui::InputText("Message", message, 32);
    if (ImGui::Button("Zaber Send Message")) {
        game.zaber.sendMessage(message);
    }
    if (ImGui::Button("Zaber Stop")) {
        game.zaber.stop();
    }
    if (!game.zaber.isInitialized()) ImGui::EndDisabled();
    ImGui::BeginDisabled(true);
    if (ImGui::Button("Ephys Initialize")) {
        game.ephys.Initialize();
    }
    if (ImGui::Button("Ephys Start")) {
        game.ephys.Start();
    }
    if (ImGui::Button("Ephys Stop")) {
        game.ephys.Stop();
    }
    if (ImGui::Button("Cameras 1 Frontal Initialize")) {
        game.camera1Frontal.Initialize();
    }
    if (ImGui::Button("Camera 1 Frontal Start")) {
        game.camera1Frontal.Start();
    }
    if (ImGui::Button("Cameras 1 Frontal Stop")) {
        game.camera1Frontal.Stop();
    }
    if (ImGui::Button("Cameras 2 Lateral Initialize")) {
        game.camera2Lateral.Initialize();
    }
    if (ImGui::Button("Camera 2 Lateral Start")) {
        game.camera2Lateral.Start();
    }
    if (ImGui::Button("Cameras 2 Lateral Stop")) {
        game.camera2Lateral.Stop();
    }

    if (ImGui::Button("Copy Ephys")) {
    }
    if (ImGui::Button("Copy Metadata")) {
    }
    ImGui::EndDisabled();

    static const char *result = "";
    if (ImGui::Button("Compress Videos")) {
        result = compressVideos() ? "Success!" : "Failure!";
        ImGui::OpenPopup("Result");
    }
    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Result", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(result);
        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
}


void imguiWindowMain(ImGuiIO io, Game &game) {
    static bool show_demo_window = false;
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Main", nullptr, window_flags);

    ImGui::BeginTable("tableMain", 2);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    controlWindow(game);
    generalWindow(game);
    statusWindow(game);
    zaberWindow(game);
    camerasWindow(game);
    ImGui::TableSetColumnIndex(1);
    dataWindow(game);
    manualWindow(game);

    ImGui::SeparatorText("Extra Options");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::EndTable();

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
    ImGui::End();
}


void Gui::Instantiate() {
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    m_io->Fonts->AddFontDefault();

    ImFont *font_title = m_io->Fonts->AddFontFromFileTTF("../AnonymousPro.ttf", 15.0f, nullptr,
                                                         m_io->Fonts->GetGlyphRangesDefault());
    IM_ASSERT(font_title != nullptr);
    Imgui_ImplRaylib_BuildFontAtlas();
}

void Gui::Update(Game &game) {
    ImGui_ImplRaylib_ProcessEvents();

    // Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(*m_io, game);

    // Rendering
    ImGui::Render();
}

void Gui::Draw() {
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}

Gui::~Gui() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}
