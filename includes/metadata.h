//
// Created by ginko on 08/06/24.
//

#ifndef SETUP_GUI_METADATA_H
#define SETUP_GUI_METADATA_H

#include <string>
#include "toml++/toml.hpp"
#include "directoryNode.h"

constexpr char const *metadataPath{"metadata.toml"};


struct RecordingPaths {
    std::string dataRootPath;
    std::string ephysRecordingPath;
    std::string cam1Path;
    std::string cam2Path;

    RecordingPaths();
    static toml::table LoadFromFile();
};


class Metadata {
    int ID{};
    RecordingPaths recordingPaths{};
    time_t date{};
    time_t startTime{};

    void updateRootNode();
public:
    explicit Metadata(int ID_);

    void StartExperiment();
    DirectoryNode getRootNode() const;


};


#endif //SETUP_GUI_METADATA_H
