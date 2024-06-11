//
// Created by ginko on 08/06/24.
//

#ifndef SETUP_GUI_DIRECTORYNODE_H
#define SETUP_GUI_DIRECTORYNODE_H

#include <string>
#include <vector>
#include <filesystem>

struct DirectoryNode {
    std::string fullPath;
    std::string fileName;
    std::vector<DirectoryNode> children;
    bool isDirectory;
};

DirectoryNode CreateDirectoryNodeTreeFromPath(const std::filesystem::path &rootPath);


void RecursivelyAddDirectoryNodes(DirectoryNode &parentNode);

#endif //SETUP_GUI_DIRECTORYNODE_H
