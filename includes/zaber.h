//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_ZABER_H
#define SETUP_GUI_ZABER_H


#include <string>
#include <ctime>
#if defined(_WIN32)           // Raylib workaround
    #define NOGDI             // All GDI defines and routines
    #define NOUSER            // All USER defines and routines
#endif

#include <boost/asio.hpp>

#if defined(_WIN32)           // raylib uses these names as function parameters
    #undef near
    #undef far
#endif

class Zaber {
    enum Protocol {
    };

    Protocol protocol;

    bool initialized{false};
    bool running{false};
    bool moved{true};
    unsigned int x{0};
    unsigned int y{0};
    unsigned int xNext{0};
    unsigned int yNext{0};
    double timeToNext{0};
    boost::asio::io_service io;
    boost::asio::serial_port serial;
    std::string answer {};

public:
    Zaber();
    ~Zaber();

    bool IsInitialized();
    std::string_view GetAnswer();

    bool Connect(boost::asio::serial_port &serial,
                 const std::string &portname,
                 unsigned int baud_rate);

    void Start();

    void LoadProtocol(Protocol);

    bool IsRunning();

    std::string GetProtocol();

    void sendMessage(const std::string &message);

    int getX() ;

    int getY() ;

    int getNextY() const;

    int getNextX() const;

    std::time_t getTimeToNext() const;

    void Initialize();

    void Stop();
};


#endif //SETUP_GUI_ZABER_H
