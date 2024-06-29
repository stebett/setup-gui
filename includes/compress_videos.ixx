//
// Created by Stefano on 6/29/2024.
//
module;
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <filesystem>

export module compress_videos;

constexpr auto video_path =  "C:/Users/Stefano/Desktop/cam-2-lateral/";
constexpr auto output_file = "C:/Users/Stefano/Desktop/output.mp4";
constexpr auto ffmpeg_path = "C:/Users/Stefano/Downloads/ffmpeg-master-latest-win64-gpl/bin/ffmpeg";
constexpr int framerate = 6;

export bool compressVideos() {
    if (std::filesystem::exists(output_file)) return false;
    const auto command = fmt::format("{} -n -framerate {} -i {}image_%09d.bmp -c:v libx264 -preset ultrafast {}",
                                  ffmpeg_path,
                                  framerate,
                                  video_path,
                                  output_file);

    const auto success = system(command.c_str());
    spdlog::info("[Video Compression] compression return code: {}", success);
    return success == 0;
}