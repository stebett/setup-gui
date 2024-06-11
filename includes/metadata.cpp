//
// Created by ginko on 08/06/24.
//

#include <vector>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include "metadata.h"


//SaveMetadata() {
//    saveFramesN()
//    saveDigIn()
//    saveLaserIn()
//    saveDuration()
//    copyArduinoCode()
//    copyCamParams()
//    saveZaberProtocol()
//}
//


toml::table RecordingPaths::LoadFromFile() {
    toml::table config;
    try {
        config = toml::parse_file(metadataPath);
    }
    catch (const toml::parse_error &err) {
        std::cerr << "WARNING: METADATA: Parsing failed:\n" << err << "\n";
        return config;
    }
    std::cout << "INFO: METADATA: Metadata file loaded successfully" << "\n";

    return config;
}


RecordingPaths::RecordingPaths() {
    toml::table config = LoadFromFile();
    dataRootPath = config["dataRootPath"].value_or("");
    ephysRecordingPath = config["ephysRecordingPath"].value_or("");
    cam1Path = config["cam1Path"].value_or("");
    cam2Path = config["cam2Path"].value_or("");
}


Metadata::Metadata(int ID_) : ID(ID_) {
}

void Metadata::StartExperiment() {

}

DirectoryNode Metadata::getRootNode() const {
    return CreateDirectoryNodeTreeFromPath(recordingPaths.dataRootPath);
}
