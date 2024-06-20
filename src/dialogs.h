#pragma once

#include "ofThread.h"
#include <string>
#include <functional>
#include <iostream>
#include <array>

extern "C" {
#include "tinyfiledialogs.h"
}

namespace ofxChoc
{
    class Dialogs : public ofThread
    {
    public:
        using FileCallback = std::function<void(const std::string&)>;
        using ColorCallback = std::function<void(const std::array<unsigned char, 3>&)>;

        Dialogs() = default;
        ~Dialogs() {
            if (isThreadRunning()) {
                waitForThread(true);
            }
        }

        void openFileDialog(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback) {
            if (isThreadRunning()) {
                waitForThread(true);
            }

            this->title = title;
            this->defaultPath = defaultPath;
            this->filter = filter;
            this->fileCallback = callback;
            this->isSaveDialog = false;
            this->isColorPicker = false;

            startThread();
        }

        void saveFileDialog(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback) {
            if (isThreadRunning()) {
                waitForThread(true);
            }

            this->title = title;
            this->defaultPath = defaultPath;
            this->filter = filter;
            this->fileCallback = callback;
            this->isSaveDialog = true;
            this->isColorPicker = false;

            startThread();
        }

        void colorPickerDialog(const std::string& title, const std::array<unsigned char, 3>& defaultColor, ColorCallback callback) {
            if (isThreadRunning()) {
                waitForThread(true);
            }

            this->title = title;
            this->defaultColor = defaultColor;
            this->colorCallback = callback;
            this->isColorPicker = true;

            startThread();
        }

    private:
        void threadedFunction() override {
            if (isColorPicker) {
                colorPickerThread(title, defaultColor, colorCallback);
            } else if (isSaveDialog) {
                fileDialogThread(title, defaultPath, filter, fileCallback, true);
            } else {
                fileDialogThread(title, defaultPath, filter, fileCallback, false);
            }
        }

        void fileDialogThread(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback, bool saveDialog) {
            const char* filterPatterns[] = { filter.c_str() };
            const char* fileName = saveDialog ?
                tinyfd_saveFileDialog(title.c_str(), defaultPath.c_str(), 1, filterPatterns, NULL) :
                tinyfd_openFileDialog(title.c_str(), defaultPath.c_str(), 1, filterPatterns, NULL, 0);

            if (fileName) {
                callback(fileName);
            } else {
                callback("");
            }

            stopThread();
        }

        void colorPickerThread(const std::string& title, const std::array<unsigned char, 3>& defaultColor, ColorCallback callback) {
            unsigned char color[3] = { defaultColor[0], defaultColor[1], defaultColor[2] };
            if (tinyfd_colorChooser(title.c_str(), NULL, color, color)) {
                std::array<unsigned char, 3> selectedColor = { color[0], color[1], color[2] };
                callback(selectedColor);
            } else {
                callback(defaultColor);
            }

            stopThread();
        }

        std::string title;
        std::string defaultPath;
        std::string filter;
        FileCallback fileCallback;
        ColorCallback colorCallback;
        std::array<unsigned char, 3> defaultColor;
        bool isSaveDialog;
        bool isColorPicker;
    };
}
