//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#include <imgui.h>
#include "game.h"

class Gui {
    static ImGuiIO *m_io;

public:
    static void Instantiate();

    static void Update(Game&);

    static void Draw();

    ~Gui();
};

ImGuiIO InitGui();


#endif //GUI_MANAGER_H
