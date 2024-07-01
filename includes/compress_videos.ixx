//
// Created by Stefano on 6/29/2024.
//
module;
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <filesystem>

export module compress_videos;

constexpr auto ffmpeg_path = "C:/Users/Stefano/Downloads/ffmpeg-master-latest-win64-gpl/bin/ffmpeg";
constexpr int framerate = 6;

export bool compressVideos(const std::filesystem::path &input_path, const std::filesystem::path &output_file) {
    if (std::filesystem::exists(output_file)) {
        spdlog::error("[Video Compression] video already existing at: {}", output_file.string());
        return false;
    }
    const auto command = fmt::format("{} -n -framerate {} -i {}image_%09d.bmp -c:v libx264 -preset ultrafast {}",
                                     ffmpeg_path,
                                     framerate,
                                     input_path.string(),
                                     output_file.string());

    const auto success = system(command.c_str());
    spdlog::info("[Video Compression] compression return code: {}", success);
    return success == 0;
}
