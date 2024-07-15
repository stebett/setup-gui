//
// Created by Stefano on 6/29/2024.
//
#include "path-manager.h"

#include "toml++/toml.hpp"
#include <spdlog/spdlog.h>


constexpr auto metadataPath{"../metadata.toml"};

PathManager::PathManager() {
    toml::table config{};
    try {
        config = toml::parse_file(metadataPath);
        spdlog::info("[PathManager]: Parsing succeded");
    } catch (const toml::parse_error &err) {
        spdlog::error("[PathManager]: Parsing failed: {}", err.what());
        return;
    }

    auto loadAndCheck = [&config](std::filesystem::path &filepath, const std::string &name) {
        filepath = config[name].value_or("");
        if (!exists(filepath))
            spdlog::error("[PathManager]: {} not found: {}", name, filepath.string());
    };

    loadAndCheck(dataRootPath, "dataRootPath");
    loadAndCheck(ephysRecordingPath, "ephysRecordingPath");
    loadAndCheck(cam1InputPath, "cam1Path");
    loadAndCheck(cam2InputPath, "cam2Path");
}

int PathManager::CointainedElements(const std::filesystem::path &directory_path) {
    int file_count{};
    try {
        file_count = static_cast<int>(std::ranges::distance(std::filesystem::directory_iterator(directory_path)));
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] [CointainedFiles] Error: {}", e.what());
        return -1;
    }
    return file_count;
}

void PathManager::initialize(const std::string &subject, const std::string &date) {
    sessionPath = dataRootPath / subject / fmt::format("{}_{}", subject, date);
    sessionPath = absolute(sessionPath);
    try {
        create_directories(sessionPath);
        spdlog::info("[PathManager] Created session directory at: {}", sessionPath.string());
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] Failed creating session directory at: {}\nwith error: {}", sessionPath.string(),
                      e.what());
    }
    cam1OutputPath = sessionPath / fmt::format("cam-1-frontal_{}_{}.mp4", subject, date);
    cam2OutputPath = sessionPath / fmt::format("cam-2-lateral_{}_{}.mp4", subject, date);
    initialized = true;
}

void PathManager::saveEphys() {
    if (!isInitialized()) {
        spdlog::error("[PathManager] Not moving ephys while not initialized");
        return;
    }
    const auto ephysOutputPath = sessionPath / "ephys";
    spdlog::info("[PathManager] [saveEphys] Moving from {} to {}", ephysRecordingPath.string(),
                 ephysOutputPath.string());
    if (std::filesystem::is_directory(ephysOutputPath)) {
        spdlog::error("[PathManager] [saveEphys] Directory already existing");
        return;
    }
    try {
        std::filesystem::copy(absolute(ephysRecordingPath), absolute(ephysOutputPath),
                              std::filesystem::copy_options::recursive);
        // std::filesystem::copy(absolute(ephysRecordingPath), absolute(ephysOutputPath));
        // std::filesystem::create_directory(ephysRecordingPath);
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] [saveEphys]\n{}", e.what());
    }
}

void PathManager::cleanRecordingDirs() const {
    try {
        spdlog::info("[PathManager] Removing directory: {}", ephysRecordingPath.string());
        std::filesystem::remove_all(absolute(ephysRecordingPath));
        std::filesystem::create_directories(absolute(ephysRecordingPath));
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] [cleanRecordingDirs]\n{}", e.what());
    }
    try {
        spdlog::info("[PathManager] Removing directory: {}", cam1InputPath.string());
        std::filesystem::remove_all(absolute(cam1InputPath));
        std::filesystem::create_directories(absolute(cam1InputPath));
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] [cleanRecordingDirs]\n{}", e.what());
    }
    try {
        spdlog::info("[PathManager] Removing directory: {}", cam2InputPath.string());
        std::filesystem::remove_all(absolute(cam2InputPath));
        std::filesystem::create_directories(absolute(cam2InputPath));
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] [cleanRecordingDirs]\n{}", e.what());
    }
}

bool PathManager::isInitialized() const {
    return initialized;
}
