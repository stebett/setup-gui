//
// Created by ginko on 11/06/24.
//

#include "zaber.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost//array.hpp>

void Zaber::Connect() {

}

void Zaber::Start() {

}

void Zaber::LoadProtocol(Zaber::Protocol) {

}

bool Zaber::IsInitialized() {
    return false;
}

bool Zaber::IsRunning() {
    return false;
}

int Zaber::getX() const {
    return 0;
}

int Zaber::getY() const {
    return 0;
}

int Zaber::getNextY() const {
    return 0;
}

int Zaber::getNextX() const {
    return 0;
}

std::string Zaber::GetProtocol() {
    return std::string();
}

std::time_t Zaber::getTimeToNext() const {
    return 0;
}

void Zaber::Initialize() {
    boost::asio::io_service io;
    boost::asio::serial_port serial(io, "COM1");  // Replace "COM1" with your serial port (e.g., "/dev/ttyUSB0" on Linux)

    // Set serial port options (as shown earlier)
    serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
    serial.set_option(boost::asio::serial_port_base::character_size(8));
    serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
}

void Zaber::Stop() {

}
