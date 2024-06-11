//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#include <entt/entity/registry.hpp>
#include <imgui.h>
#include "directoryNode.h"

class Gui {
    static ImGuiIO *m_io;
    static DirectoryNode rootNode;

public:
    static void Instantiate();

    static void Update(DirectoryNode);

    static void Draw();

    ~Gui();
};

ImGuiIO InitGui();


#endif //GUI_MANAGER_H
