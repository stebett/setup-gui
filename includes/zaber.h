//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_ZABER_H
#define SETUP_GUI_ZABER_H


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

    void Connect();
    void Start();
    void LoadProtocol(Protocol);
};


#endif //SETUP_GUI_ZABER_H
