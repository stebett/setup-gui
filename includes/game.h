//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_GAME_H
#define SETUP_GUI_GAME_H


#include "control.h"
#include "zaber.h"
#include "path-manager.h"
#include "ephys.h"
#include "camera-control.h"

import session_control;
import ephys_control;

class Game {
    bool initialized{false};
    bool running{false};
    double startTime{0};

public:
    Controls controls;
    Session session;
    Zaber zaber;
    PathManager pathManager;
    EphysControl ephys;
    CameraController camera1Frontal;
    CameraController camera2Lateral;
public:
    bool IsInitialized();

    bool isRunning();

    double getStartTime();

    void Initialize();

    void Start();

    bool IsRunning();
};


#endif //SETUP_GUI_GAME_H
