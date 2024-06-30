//
// Created by ginko on 11/06/24.
//

#include "path-manager.h"
//
// Created by Stefano on 6/29/2024.
//

#include "toml++/toml.hpp"
#include <spdlog/spdlog.h>


constexpr auto metadataPath{"metadata.toml"};

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
    loadAndCheck(cam1Path, "cam1Path");
    loadAndCheck(cam2Path, "cam2Path");
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

void PathManager::computeSession(const std::string &subject, const std::string &date) {
    sessionPath = dataRootPath / subject / fmt::format("{}_{}", subject, date);
    sessionPath = absolute(sessionPath);
    try {
        create_directories(sessionPath);
        spdlog::info("[PathManager] Created session directory at: {}", sessionPath.string());
    } catch (const std::filesystem::filesystem_error &e) {
        spdlog::error("[PathManager] Failed creating session directory at: {}\nwith error: {}", sessionPath.string(), e.what());
    }
}
