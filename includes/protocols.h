//
// Created by Stefano on 6/28/2024.
//

#ifndef PROTOCOLS_H
#define PROTOCOLS_H
#include <random>

class AdaptationProtocol {
private:
    int shift_x{};
    int max_x{};
    int min_x{};
    float interval_seconds{};

    std::random_device dev;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist01;

private:
    int getDirection();

public:
    int start_x{};
    int start_y{};
    int speed{};
    int current_x{};



public:
    AdaptationProtocol() :
    rng(dev()), dist01(0, 1) {}

    [[nodiscard]] int getNextX();

    [[nodiscard]] int getNextY() const;

    [[nodiscard]] float getInterval() const;

    void Load();
};



#endif //PROTOCOLS_H
