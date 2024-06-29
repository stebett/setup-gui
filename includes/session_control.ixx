//
// Created by Stefano on 6/29/2024.
//

module;
#include <array>
#include <chrono>
#include <string>
#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

export module session_control;


export enum Subject {
    SB017,
    SB018,
    SB019,
    SB020,
    SB021,
    SB022,
    SubjectNotFound,
};

export constexpr std::array subjectList{
    SB017,
    SB018,
    SB019,
    SB020,
    SB021,
    SB022,
};



export class Session {
    Subject subject {SubjectNotFound};
    std::string date;

public:
    void SwitchSubject(const Subject new_subject) {
        spdlog::info("[Session] Switching to subject {}", enum2String(new_subject));
        subject = new_subject;
    }


    [[nodiscard]] char const *  getSubject() const {
        return enum2String(subject);
    }


    static std::string getDate() {
        const std::time_t timeptr = std::time(nullptr);
        return fmt::format("{:%Y%m%d}", fmt::localtime(timeptr));
    }

    static std::string getDatePretty() {
        const std::time_t timeptr = std::time(nullptr);
        return fmt::format("{:%Y-%m-%d}", fmt::localtime(timeptr));
    }

    static char const * enum2String(const Subject subjectEnum) {
        switch (subjectEnum) {
            case SB017: return "SB017";
            case SB018: return "SB018";
            case SB019: return "SB019";
            case SB020: return "SB020";
            case SB021: return "SB021";
            case SB022: return "SB022";
            default: return "Error";
        }
    }

};
