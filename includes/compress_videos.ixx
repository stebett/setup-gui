//
// Created by Stefano on 6/29/2024.
//
module;
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <filesystem>

export module compress_videos;

constexpr auto ffmpeg_path = "../bins/ffmpeg-master-latest-win64-gpl/bin/ffmpeg";

export bool compressVideos(const std::filesystem::path &input_path, const std::filesystem::path &output_file, int framerate = 500) {
    if (std::filesystem::exists(output_file)) {
        spdlog::error("[Video Compression] video already existing at: {}", output_file.string());
        return false;
    }
    const auto command = fmt::format("{} -n -framerate {} -i {}image_%09d.bmp -c:v libx264 -preset ultrafast {}",
                                     std::filesystem::absolute(ffmpeg_path).string(),
                                     framerate,
                                     input_path.string(),
                                     output_file.string());

    const auto success = system(command.c_str());
    if (success == 0) {
        spdlog::info("[Video Compression] compression return code: {}", success);
    } else {
        spdlog::error("[Video Compression] compression return code: {}", success);
    }
    return success == 0;
}
