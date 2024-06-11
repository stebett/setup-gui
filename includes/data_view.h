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
    std::filesystem::path ephysPath;
    std::filesystem::path sessionPath;

    int CointainedFiles(std::filesystem::path);
    int CointainedDirectories(std::filesystem::path);
};


#endif //SETUP_GUI_DATA_VIEW_H
