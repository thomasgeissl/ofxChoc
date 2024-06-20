#pragma once

#include "ofThread.h"
#include "ofColor.h"
#include <string>
#include <functional>
#include <iostream>

extern "C" {
#include "tinyfiledialogs.h"
}

namespace ofxChoc
{
    class Dialogs : public ofThread
    {
    public:
        using FileCallback = std::function<void(const std::string&)>;
        using ColorCallback = std::function<void(const ofColor&)>;
        using InputCallback = std::function<void(const std::string&)>;
        using MessageCallback = std::function<void()>;

        Dialogs() = default;
        ~Dialogs() {
            if (isThreadRunning()) {
                waitForThread(true);
            }
        }

        void openFileDialog(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback) {
            startDialogThread(title, defaultPath, filter, callback, false, false, false, false, false, false);
        }

        void saveFileDialog(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback) {
            startDialogThread(title, defaultPath, filter, callback, true, false, false, false, false, false);
        }

        void colorPickerDialog(const std::string& title, const ofColor& defaultColor, ColorCallback callback) {
            this->defaultColor = defaultColor;
            startDialogThread(title, "", "", nullptr, false, true, false, false, false, false);
            this->colorCallback = callback;
        }

        void messageBox(const std::string& title, const std::string& message, const std::string& dialogType = "ok", MessageCallback callback = nullptr) {
            this->message = message;
            this->dialogType = dialogType;
            startDialogThread(title, "", "", nullptr, false, false, true, false, false, false);
            this->messageCallback = callback;
        }

        void inputBox(const std::string& title, const std::string& message, const std::string& defaultInput, InputCallback callback) {
            this->message = message;
            this->defaultInput = defaultInput;
            startDialogThread(title, "", "", nullptr, false, false, false, true, false, false);
            this->inputCallback = callback;
        }

        void chooseFolderDialog(const std::string& title, const std::string& defaultPath, FileCallback callback) {
            startDialogThread(title, defaultPath, "", callback, false, false, false, false, true, false);
        }

    private:
        void startDialogThread(const std::string& title, const std::string& defaultPath, const std::string& filter, FileCallback callback, bool isSaveDialog, bool isColorPicker, bool isMessageBox, bool isInputBox, bool isChooseFolder, bool isThreadRunningCheck) {
            if (isThreadRunning()) {
                waitForThread(true);
            }

            this->title = title;
            this->defaultPath = defaultPath;
            this->filter = filter;
            this->fileCallback = callback;
            this->isSaveDialog = isSaveDialog;
            this->isColorPicker = isColorPicker;
            this->isMessageBox = isMessageBox;
            this->isInputBox = isInputBox;
            this->isChooseFolder = isChooseFolder;

            startThread();
        }

        void threadedFunction() override {
            if (isColorPicker) {
                colorPickerThread(title, defaultColor, colorCallback);
            } else if (isMessageBox) {
                messageBoxThread(title, message, dialogType, messageCallback);
            } else if (isInputBox) {
                inputBoxThread(title, message, defaultInput, inputCallback);
            } else if (isChooseFolder) {
                chooseFolderThread(title, defaultPath, fileCallback);
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

        void colorPickerThread(const std::string& title, const ofColor& defaultColor, ColorCallback callback) {
            unsigned char color[3] = { defaultColor.r, defaultColor.g, defaultColor.b };
            if (tinyfd_colorChooser(title.c_str(), NULL, color, color)) {
                ofColor selectedColor(color[0], color[1], color[2]);
                callback(selectedColor);
            } else {
                callback(defaultColor);
            }

            stopThread();
        }

        void messageBoxThread(const std::string& title, const std::string& message, const std::string& dialogType, MessageCallback callback) {
            tinyfd_messageBox(title.c_str(), message.c_str(), dialogType.c_str(), "info", 1);
            if (callback) {
                callback();
            }
            stopThread();
        }

        void inputBoxThread(const std::string& title, const std::string& message, const std::string& defaultInput, InputCallback callback) {
            const char* input = tinyfd_inputBox(title.c_str(), message.c_str(), defaultInput.c_str());
            if (input) {
                callback(input);
            } else {
                callback("");
            }
            stopThread();
        }

        void chooseFolderThread(const std::string& title, const std::string& defaultPath, FileCallback callback) {
            const char* folderName = tinyfd_selectFolderDialog(title.c_str(), defaultPath.c_str());
            if (folderName) {
                callback(folderName);
            } else {
                callback("");
            }
            stopThread();
        }

        std::string title;
        std::string defaultPath;
        std::string filter;
        FileCallback fileCallback;
        ColorCallback colorCallback;
        InputCallback inputCallback;
        MessageCallback messageCallback;
        std::string message;
        std::string defaultInput;
        std::string dialogType;
        ofColor defaultColor;
        bool isSaveDialog;
        bool isColorPicker;
        bool isMessageBox;
        bool isInputBox;
        bool isChooseFolder;
    };
}
