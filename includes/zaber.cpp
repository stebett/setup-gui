//
// Created by ginko on 11/06/24.
//

#include "zaber.h"
#include <iostream>
#include <boost/array.hpp>
#include <spdlog/spdlog.h>
#include <fmt/core.h>


bool SelectComPorts() //added function to find the present serial
{
    char lpTargetPath[5000]; // buffer to store the path of the COMPORTS
    bool gotPort = false; // in case the port is not found

    for (int i = 0; i < 20; i++) // checking ports from COM0 to COM255
    {
        std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2
        DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 5000);
        spdlog::debug("[Zaber] Testing: {}", str);

        // Test the return value and error if any
        if (test != 0) //QueryDosDevice returns zero if it didn't find an object
        {
            spdlog::info("[Zaber] {}: {}", str, lpTargetPath);
            gotPort = true;
        }

        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            spdlog::error("[Zaber] Insufficient buffer)");
        }
    }

    return gotPort;
}

std::string read(boost::asio::serial_port &serial) {
    boost::asio::streambuf buf;
    std::string response;

    try {
        boost::asio::read_until(serial, buf, "\r\n");

        std::istream is(&buf);
        std::getline(is, response);
    } catch (boost::system::system_error &e) {
        std::cerr << "Error reading from serial port: " << e.what() << std::endl;
    }

    return response;
}


// Function to configure the serial port
bool Zaber::connect(boost::asio::serial_port &serial,
                    const std::string &portname,
                    unsigned int baud_rate) {
    try {
        spdlog::info("[Zaber] Opening serial port {}", portname);
        serial.open(portname);
        // Set the baud rate
        spdlog::info("[Zaber] Configuring serial port {}", portname);
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
        serial.set_option(boost::asio::serial_port_base::character_size(8));
        serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        serial.set_option(
            boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    } catch (const std::exception &e) {
        spdlog::error("[Zaber] Error configuring serial port: {}", e.what());
    }
    return serial.is_open();
}

std::string Zaber::getProtocol() {
    return {};
}

void Zaber::sendMessage(const std::string &message) {
    spdlog::info("[Zaber] Sending command: {}", message);
    boost::asio::write(serial, boost::asio::buffer(message + "\r\n"));
    answer = read(serial);
    spdlog::info("[Zaber] [Read] {}", answer);
}


void Zaber::start() {
    running = true;
}

void Zaber::loadProtocol() {
    protocol.Load();
}

bool Zaber::isInitialized() const {
    return initialized;
}

const char *Zaber::getAnswer() const {
    return answer.c_str();
}

bool Zaber::isRunning() const {
    return running;
}


int Zaber::getX() {
    static int last_x{};
    if (!initialized) return 0;
    if (!movedX || timer.Elapsed() < 1) return last_x;

    sendMessage("/2 1 get pos");
    const auto answer_pos = answer.find_last_of(' ');
    last_x = std::atoi(answer.substr(answer_pos).c_str());
    movedX = false;
    protocol.current_x = last_x;
    xNext = protocol.getNextX();

    return last_x;
}

int Zaber::getY() {
    static int last_y{};
    if (!initialized) return 0;
    if (!movedY) return last_y;
    yNext = protocol.getNextY();

    sendMessage("/1 1 get pos");
    const auto answer_pos = answer.find_last_of(' ');
    last_y = std::atoi(answer.substr(answer_pos).c_str());
    movedY = false;
    return last_y;
}

int Zaber::getNextY() const {
    return yNext;
}

int Zaber::getNextX() const {
    return xNext;
}

void Zaber::moveX(int position) {
    sendMessage(fmt::format("/2 1 move abs {}", position));
    movedX = true;
}

void Zaber::moveY(int position) {
    sendMessage(fmt::format("/1 1 move abs {}", position));
    movedY = true;
}

float Zaber::getSecondsToNext() const {
    return protocol.getInterval() - timer.Elapsed();
}

void Zaber::initialize() {
    spdlog::info("[Zaber] Initializing");
    SelectComPorts();

    if (connect(serial, "COM3", 115200)) {
        spdlog::info("[Zaber] Initialization successfull");
        initialized = true; // TODO error handling
    }
    spdlog::info("[Zaber] Loading protocol");
    loadProtocol();

    spdlog::info("[Zaber] Moving to start ({}, {})", protocol.start_x, protocol.start_y);
    sendMessage(fmt::format("/set maxspeed {}", 200000));
    moveX(protocol.start_x);
    moveY(protocol.start_y);
    Sleep(2000);
    spdlog::info("[Zaber] Setting speed to {}", protocol.speed);
    sendMessage(fmt::format("/set maxspeed {}", protocol.speed));
    movedX = true;
    movedY = true;
    timer.Reset();
}


void Zaber::stop() {
    sendMessage("/1 1 stop");
    sendMessage("/2 1 stop");
    running = false;
}

void Zaber::Update() {
    if (!running) return;
    if (timer.Elapsed() <= protocol.getInterval()) return;

    moveX(xNext);
    moveY(yNext);
    timer.Reset();
}


Zaber::Zaber()
    : io(), serial(io) {
    spdlog::info("[Zaber] [Constructor] Initializing");
}


Zaber::~Zaber() {
    if (isInitialized()) stop();
    spdlog::info("[Zaber] Closing serial port)");
    try {
        serial.close(); // Close the serial port
    } catch (const std::exception &e) {
        spdlog::error("[Zaber] Error closing serial port: {}", e.what());
    }
}
