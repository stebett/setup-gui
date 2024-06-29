//
// Created by ginko on 11/06/24.
//

#include "zaber.h"
#include <iostream>
#include <boost/array.hpp>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

// Function to read data from the serial port
std::string readFromSerialPort(boost::asio::serial_port &serial) {
    char buffer[100]; // Buffer to store incoming data
    boost::system::error_code ec;
    // Read data from the serial port
    size_t len
            = read(serial, boost::asio::buffer(buffer), ec);
    if (ec) {
        spdlog::error("[Zaber] Error reading from serial port: {}", ec.message());
        return "";
    }
    // Return the read data as a string
    return {buffer, len};
}

// Function to write data to the serial port
void writeToSerialPort(boost::asio::serial_port &serial,
                       const std::pmr::string &message) {
    boost::system::error_code ec;
    // Write data to the serial port
    spdlog::info("[Zaber] Writing: {}", message);
    write(serial, boost::asio::buffer(message), ec);
    if (ec) {
        spdlog::error("[Zaber] Error writing to serial port: {}", ec.message());
    }
}

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

int Zaber::getX() {
    static int last_x{};
    if (!initialized) return 0;
    if (!movedX) return last_x;

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

std::string Zaber::getProtocol() {
    return {};
}

void Zaber::sendMessage(const std::string &message) {
    spdlog::info("[Zaber] Sending command: {}", message);
    boost::asio::write(serial, boost::asio::buffer(message + "\r\n"));
    answer = read(serial);
    spdlog::info("[Zaber] [Read] {}", answer);
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

Zaber::Zaber(): io(),
                serial(io) {
    spdlog::info("[Zaber] [Constructor] Initializing");
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
    sleep(2);
    spdlog::info("[Zaber] Setting speed to {}", protocol.speed);
    sendMessage(fmt::format("/set maxspeed {}", protocol.speed));
    movedX = true;
    movedY = true;
    timer.Reset();
}

Zaber::~Zaber() {
    spdlog::info("[Zaber] Closing serial port)");
    try {
        serial.close(); // Close the serial port
    } catch (const std::exception &e) {
        spdlog::error("[Zaber] Error closing serial port: {}", e.what());
    }
}

void Zaber::stop() {
    sendMessage("/1 1 stop");
    sendMessage("/2 1 stop");
}

void Zaber::Update() {
    if (!running) return;
    if (timer.Elapsed() <= protocol.getInterval()) return;

    moveX(xNext);
    moveY(yNext);
    timer.Reset();
}


//
//
// // Write the message to the serial port
// writeToSerialPort(serial, message);
// cout << "Message sent: " << message << endl;
//
// // Read the response from the server
// string response = readFromSerialPort(serial);
// if (!response.empty()) {
//     cout << "Response received: " << response << endl;
// }
//
