//
// Created by Stefano on 6/28/2024.
//

#include "protocols.h"
#include "toml++/toml.hpp"
#include "spdlog/spdlog.h"

int AdaptationProtocol::getDirection() {
    switch (dist01(rng)) {
        case 0: return -1;
        case 1: return 1;
        default: return 0;
    };
}

int AdaptationProtocol::getNextX() {
    int direction = getDirection();
    spdlog::info("[Protocol] direction = {}", direction);
    int nextX = direction * shift_x + current_x;
    if (nextX < min_x || nextX > max_x) {
        direction = -direction;
        nextX = direction * shift_x + current_x;
    }
    if (nextX < min_x || nextX > max_x) {
        spdlog::error("[Protocol] Next X position out of bounds: {}", nextX);
        return start_x;
    }
    return nextX;
}

int AdaptationProtocol::getNextY() const {
    return start_y;
}

float AdaptationProtocol::getInterval() const {
    return interval_seconds;
}

void AdaptationProtocol::Load() {
    toml::table config;
    try {
        config = toml::parse_file("protocols.toml");
    } catch (const toml::parse_error &err) {
        spdlog::error("[Protocols] Parsing failed:", err.what());
    }

    start_x = config["start_x"].value_or(0);
    shift_x = config["shift_x"].value_or(0);
    max_x = config["max_x"].value_or(0);
    min_x = config["min_x"].value_or(0);
    start_y = config["start_y"].value_or(0);
    interval_seconds = config["interval_seconds"].value_or(0.0f);
    speed = config["speed"].value_or(0);
    current_x = start_x;
    spdlog::info("[Protocols] file loaded successfully");
}
