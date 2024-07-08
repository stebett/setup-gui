//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_ZABER_H
#define SETUP_GUI_ZABER_H


#include <string>
#if defined(_WIN32)           // Raylib workaround
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <boost/asio.hpp>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

#include "protocols.h"

import timer;

class Zaber {


    AdaptationProtocol protocol;

    bool initialized{false};
    bool running{false};
    bool movedX{true};
    bool movedY{true};

     int x{0};
     int y{0};
     int xNext{0};
     int yNext{0};
    double timeToNext{0};
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    std::string answer{};
    Timer timer {};

public:
    Zaber();

    ~Zaber();
    void initialize();

    [[nodiscard]] bool isInitialized() const;

    [[nodiscard]] const char *getAnswer() const;

    static bool connect(boost::asio::serial_port &serial,
                        const std::string &portname,
                        unsigned int baud_rate);

    void start();

    void loadProtocol();

    [[nodiscard]] bool isRunning() const;

    static std::string getProtocol();

    void sendMessage(const std::string &message);
    void moveX(int position);
    void moveY(int position);

    int getX();

    int getY();

    [[nodiscard]] int getNextY() const;

    [[nodiscard]] int getNextX() const;

    [[nodiscard]] float getSecondsToNext() const;


    void stop();
    void update();
};


#endif //SETUP_GUI_ZABER_H
