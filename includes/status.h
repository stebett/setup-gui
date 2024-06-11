//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_STATUS_H
#define SETUP_GUI_STATUS_H


class Status {
    bool initialized{false};
    bool running{false};
    double startTime{0};
public:
    bool IsInitialized();
    bool isRunning();
    double getStartTime();
    void Initialize();
    void Start();

};


#endif //SETUP_GUI_STATUS_H
