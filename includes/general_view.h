//
// Created by ginko on 11/06/24.
//

#ifndef SETUP_GUI_GENERAL_VIEW_H
#define SETUP_GUI_GENERAL_VIEW_H


#include <string>

class GeneralView {
    std::string subject;
    std::string date;

public:
    std::string getSubject();
    std::string getDate();
};


#endif //SETUP_GUI_GENERAL_VIEW_H
