//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_CAMERA_CONTROL_H
#define SETUP_GUI_CAMERA_CONTROL_H



class CameraController {
    bool connected{false};
    bool running{false};

public:
    bool IsRunning() const;
    void Start();
    void Stop();

    bool IsInitialized() const;

    void Initialize();
};



#endif //SETUP_GUI_CAMERA_CONTROL_H
