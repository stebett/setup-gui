//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_DATA_VIEW_H
#define SETUP_GUI_DATA_VIEW_H

#include <filesystem>

class PathManager {
public:
    std::filesystem::path cam1InputPath;
    std::filesystem::path cam2InputPath;
    std::filesystem::path cam1OutputPath;
    std::filesystem::path cam2OutputPath;
    std::filesystem::path ephysRecordingPath;
    std::filesystem::path dataRootPath;
    std::filesystem::path sessionPath;
    bool initialized {false};

    PathManager();
    static int CointainedElements(const std::filesystem::path &directory_path);
    void initialize(const std::string& subject, const std::string& date);
    void saveEphys();
    [[nodiscard]] bool isInitialized() const;
};


#endif //SETUP_GUI_DATA_VIEW_H
