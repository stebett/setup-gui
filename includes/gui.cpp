//
// Created by ginko on 04/03/24.
//

#include "gui.h"
#include <imgui_impl_raylib.h>
#include "directoryNode.h"
#include <format>
#include <entt/entity/registry.hpp>

ImGuiIO *Gui::m_io;
DirectoryNode Gui::rootNode {};

void imguiInstructions();
void ImGuiShowRoot(const DirectoryNode& rootNode);

void imguiWindowMain(ImGuiIO io, const DirectoryNode& rootNode) {
    static bool show_demo_window = false;
    static bool show_instructions = false;
    static bool show_root = true;

    ImGui::Begin("Main");


    ImGui::Checkbox("Demo Window", &show_demo_window);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Checkbox("show_instructions", &show_instructions);
    if (show_instructions)
        imguiInstructions();

    ImGui::Checkbox("show_root", &show_root);
    if (show_root)
        ImGuiShowRoot(rootNode);

//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
    ImGui::End();
}

void imguiInstructions() {

    ImGui::Begin("Instructions");
    ImGui::SeparatorText("Hotkeys");

    ImGui::Text("Q per chiudere");
    ImGui::Text("R per restartare");
    ImGui::Text("P per pausare");
    ImGui::Text("I per togglare inventario");
    ImGui::Text("O per togglare attributi");
    ImGui::Text("LClick per attaccare");
    ImGui::Text("RClick per selezionare");
    ImGui::End();
}

void RecursivelyDisplayDirectoryNode(const DirectoryNode& parentNode)
{
    ImGui::PushID(&parentNode);
    if (parentNode.isDirectory)
    {
        if (ImGui::TreeNodeEx(parentNode.fileName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
        {
            for (const DirectoryNode& childNode : parentNode.children)
                RecursivelyDisplayDirectoryNode(childNode);
            ImGui::TreePop();
        }
    }
    else
    {
        if (ImGui::TreeNodeEx(parentNode.fileName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            // ... handle file click interaction
        }
    }
    ImGui::PopID();
}

void ImGuiShowRoot(const DirectoryNode& rootNode)
{
    if (ImGui::Begin("Example Directory Tree Window", nullptr, ImGuiWindowFlags_NoSavedSettings))
        RecursivelyDisplayDirectoryNode(rootNode);
    ImGui::End();
}


//void imguiSubAttributesStartValues() {
//    ImGui::Begin("Attribute startValues");
//    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); };
//    int n = {0};
//    for (auto subattr: AttributeConstants::subAttributeVec) {
//        ImGui::PushID(n);
//        bool disableR = Params::attributes.subAttrAtStart[subattr] ==
//                        Params::attributesOriginal.subAttrAtStart[subattr];
//        if (disableR) ImGui::BeginDisabled(true);
//        if (ImGui::Button("R")) {
//            Params::attributes.subAttrAtStart[subattr] = Params::attributesOriginal.subAttrAtStart[subattr];
//        };
//        if (disableR) ImGui::EndDisabled();
//        ImGui::SameLine();
//        ImGui::DragFloat(AttributeConstants::subAttributeString[subattr],
//                         &Params::attributes.subAttrAtStart[subattr],
//                         0.1f, 0, 30);
//        ImGui::PopID();
//        n++;
//    }
//
//    ImGui::End();
//}




void Gui::Instantiate() {
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    m_io->Fonts->AddFontDefault();
    Imgui_ImplRaylib_BuildFontAtlas();

}

void Gui::Update(DirectoryNode rootNode_) {
    rootNode = rootNode_;

    ImGui_ImplRaylib_ProcessEvents();

// Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(*m_io, rootNode);

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
