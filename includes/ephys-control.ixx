//
// Created by Stefano on 6/30/2024.
//
module;
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <spdlog/spdlog.h>


export module ephys_control;

using boost::asio::ip::tcp;
using namespace std::chrono_literals;


export class EphysControl {
public:
    EphysControl() : io_context(), command_socket(io_context), timer(io_context, 10s) {
    }

    void connectToHost(const std::string &host, unsigned short port) {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        try {
            boost::asio::connect(command_socket, endpoints);
            spdlog::info("[Ephys] connected to {} : {}", host, port);
            // readResponse();
        } catch (const std::exception &e) {
            spdlog::error("[Ephys] error connecting to {} : {} -> {}", host, port, e.what());
        }
    }

    void disconnectFromHost() {
        try {
            command_socket.close();
            spdlog::info("[Ephys] Disconnected");
        } catch (const std::exception &e) {
            spdlog::error("[Ephys] Error disconnecting -> {}", e.what());
        }
        spdlog::info("[Ephys] Disconnected");
    }

    void sendCommand(const std::string &command) {
        boost::asio::write(command_socket, boost::asio::buffer(command + "\n"));
    }

    void readResponse() // TODO fix
    {
        // timer.expires_from_now(5s);
        boost::asio::streambuf buffer;
        bool response_received = false;

        // timer.async_wait([&](const boost::system::error_code& ec) {
        //     if (!response_received && !ec) {
        //         command_socket.cancel();
        //         std::cout << "Read timed out" << std::endl;
        //     }
        // });

        boost::asio::async_read_until(command_socket, buffer, "\n",
                                      [&](const boost::system::error_code &ec, std::size_t) {
                                          if (!ec) {
                                              std::istream is(&buffer);
                                              std::string response;
                                              std::getline(is, response);
                                              std::cout << "Response: " << response << std::endl;
                                              response_received = true;
                                          } else {
                                              std::cerr << "Read error: " << ec.message() << std::endl;
                                          }
                                          // timer.cancel();
                                      });

        io_context.run();
        io_context.reset();
    }


    void initialize() {
        connectToHost("127.0.0.1", 5000);
    }

    void stop() {
        sendCommand("set runmode stop");
    }

    ~EphysControl() {
        disconnectFromHost();
    }


    void start() {
        spdlog::info("[Ephys] Starting");
        sendCommand("set runmode record");
        // readResponse();
    }

private:
    boost::asio::io_context io_context;
    tcp::socket command_socket;
    boost::asio::steady_timer timer;
};

int main() {
    try {
        EphysControl client;
        client.connectToHost("127.0.0.1", 5000);

        client.sendCommand("your_command_here");
        client.readResponse();

        client.disconnectFromHost();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
