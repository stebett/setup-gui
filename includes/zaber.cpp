//
// Created by ginko on 11/06/24.
//

#include "zaber.h"
#include <iostream>
#include <boost/array.hpp>
#include <spdlog/spdlog.h>


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
bool Zaber::Connect(boost::asio::serial_port &serial,
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


void Zaber::Start() {

}

void Zaber::LoadProtocol(Zaber::Protocol) {
}

bool Zaber::IsInitialized() {
    return initialized;
}

std::string_view Zaber::GetAnswer() {
    return answer;
}

bool Zaber::IsRunning() {
    return false;
}

std::string read(boost::asio::serial_port &serial) {
    boost::asio::streambuf buf;
    std::string response;

    try {
        boost::asio::read_until(serial, buf, "\r\n");

        std::istream is(&buf);
        std::getline(is, response);
    } catch (boost::system::system_error& e) {
        std::cerr << "Error reading from serial port: " << e.what() << std::endl;
    }

    return response;
}

std::string readLine()
{
    unsigned int max_length = 5000;
    char *message {};
    unsigned char c_sum = 0;               // assuming 8-bit chars
    char *p = message + 1;                 // skip the type character

    if (strlen(message) + 6 < max_length)  // is there room for the checksum?
    {
        while (*p != 0x00)
        {
            c_sum += (unsigned char)*p++;  // calculate the checksum
        }

        c_sum = ~c_sum + 1;                // negate, increment and char size truncates

        // add the checksum to the message
        sprintf(p, ":%02X\r\n", c_sum);
        return message;
    }

    return "";
}
int Zaber::getX() {
    static int last_x {};
    if (!initialized) return 0;
    if (!moved) return last_x;

    sendMessage("/2 1 get pos");
    const auto answer_pos = answer.find_last_of(' ');
    last_x =  std::atoi(answer.substr(answer_pos).c_str());
    moved = false;
    return last_x;
}

int Zaber::getY() {
    static int last_x {};
    if (!initialized) return 0;
    if (!moved) return last_x;

    sendMessage("/1 1 get pos");
    const auto answer_pos = answer.find_last_of(' ');
    last_x =  std::atoi(answer.substr(answer_pos).c_str());
    moved = false;
    return last_x;}

int Zaber::getNextY() const {
    return 0;
}

int Zaber::getNextX() const {
    return 0;
}

std::string Zaber::GetProtocol() {
    return std::string();
}

void Zaber::sendMessage(const std::string &message) {
    spdlog::info("[Zaber] Sending command: {}", message);
    boost::asio::write(serial, boost::asio::buffer(message + "\r\n"));
    answer = read(serial);
    spdlog::info("[Zaber] [Read] {}", answer);
}

std::time_t Zaber::getTimeToNext() const {
    return 0;
}

Zaber::Zaber(): io(),
                serial(io) {
    spdlog::info("[Zaber] [Constructor] Initializing");
}

void Zaber::Initialize() {
    spdlog::info("[Zaber] Initializing");
    SelectComPorts();

    if (Connect(serial, "COM3", 115200)) {
        spdlog::info("[Zaber] Initialization successfull");
        initialized = true; // TODO error handling
    }
}

Zaber::~Zaber() {
    spdlog::info("[Zaber] Closing serial port)");
    try {
        serial.close(); // Close the serial port
    } catch (const std::exception &e) {
        spdlog::error("[Zaber] Error closing serial port: {}", e.what());
    }
}

void Zaber::Stop() {
    sendMessage("/1 1 stop");
    sendMessage("/2 1 stop");
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
