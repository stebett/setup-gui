//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_DATA_VIEW_H
#define SETUP_GUI_DATA_VIEW_H

#include <filesystem>

class DataView {
public:
    std::filesystem::path cam1Path;
    std::filesystem::path cam2Path;
    std::filesystem::path ephysRecordingPath;
    std::filesystem::path dataRootPath;
    std::filesystem::path sessionPath;

    DataView();
    static int CointainedElements(const std::filesystem::path &directory_path);
};

#endif //SETUP_GUI_DATA_VIEW_H
