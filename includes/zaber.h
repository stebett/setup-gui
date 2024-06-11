//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_ZABER_H
#define SETUP_GUI_ZABER_H


#include <string>
#include <ctime>

class Zaber {
    enum Protocol {};
    bool connected;
    bool running;
    Protocol protocol;
    float x;
    float y;
    float xNext;
    float yNext;
    double timeToNext;


public:
    bool IsInitialized();
    void Connect();
    void Start();
    void LoadProtocol(Protocol);

    bool IsRunning();

    std::string GetProtocol();

    int getX() const;

    int getY() const;

    int getNextY() const;

    int getNextX() const;

    std::time_t getTimeToNext() const;

    void Initialize();

    void Stop();
};


#endif //SETUP_GUI_ZABER_H
