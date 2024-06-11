//
// Created by ginko on 08/06/24.
//

#include <filesystem>
#include <algorithm>
#include "directoryNode.h"
// from https://github.com/ocornut/imgui/issues/5137#issuecomment-1078671909

void
RecursivelyAddDirectoryNodes(DirectoryNode &parentNode, const std::filesystem::directory_iterator &directoryIterator) {
    for (const std::filesystem::directory_entry &entry: directoryIterator) {
        DirectoryNode &childNode = parentNode.children.emplace_back();
        childNode.fullPath = entry.path();
        childNode.fileName = entry.path().filename();
        if (childNode.isDirectory = entry.is_directory(); childNode.isDirectory)
            RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
    }

    auto moveDirectoriesToFront = [](const DirectoryNode &a, const DirectoryNode &b) {
        return (a.isDirectory > b.isDirectory);
    };
    std::sort(parentNode.children.begin(), parentNode.children.end(), moveDirectoriesToFront);
}

DirectoryNode CreateDirectoryNodeTreeFromPath(const std::filesystem::path &rootPath) {
    DirectoryNode rootNode;
    rootNode.fullPath = rootPath;
    rootNode.fileName = rootPath.filename();
    if (rootNode.isDirectory = std::filesystem::is_directory(rootPath); rootNode.isDirectory)
        RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

    return rootNode;
}


