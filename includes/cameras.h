//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_CAMERAS_H
#define SETUP_GUI_CAMERAS_H



class Camera {
    bool connected{false};
    bool running{false};

public:
    bool IsRunning();
    void Start();
    void Stop();
};



#endif //SETUP_GUI_CAMERAS_H
