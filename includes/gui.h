//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#if defined(_WIN32)           // Raylib workaround
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <boost/asio.hpp> // MUST BE INCLUDED HERE FOR SOME REASON

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

#include <imgui.h>
import game;

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
