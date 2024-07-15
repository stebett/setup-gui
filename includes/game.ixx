//
// Created by Stefano on 01-Jul-24.
//

module;

#include "control.h"
#include "zaber.h"
#include "path-manager.h"
#include "camera-control.h"

export module game;

export import session_control;
export import ephys_control;
export import compress_videos;
export import timer;

export class Game {
    bool initialized{false};
    bool running{false};
    Timer timer;

public:
    Controls controls;
    Session session;
    Zaber zaber;
    PathManager pathManager;
    EphysControl ephys;
    CameraController camera1Frontal;
    CameraController camera2Lateral;

public:
    bool isInitialized() const { return initialized; }

    bool isRunning() const { return running; }

    float getTimeRunning() const { return timer.Elapsed(); }

    void initialize(const Subject subject) {
        initialized = true;
        session.initialize(subject);
        if (!zaber.isInitialized()) zaber.initialize();
        if (!ephys.isInitialized()) ephys.initialize();
        pathManager.initialize(session.getSubject(), session.getDate());
    }

    void start() {
        running = true;
        zaber.start();
        ephys.start();
        timer.Reset();
    }

    void stop() {
        running = false;
        initialized = false;
        zaber.stop();
        ephys.stop();
        pathManager.saveEphys();
        const int fps = session.getSubject() == "calibration" ? 10 : 500;

        compressVideos(pathManager.cam1InputPath, pathManager.cam1OutputPath, fps);
        compressVideos(pathManager.cam2InputPath, pathManager.cam2OutputPath, fps);
// TODO save metadata
    }

    void update() {
        zaber.update();
    }
};
