#pragma once
#include "ofMain.h"
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include <thread>
#include <atomic>
#include <string>

namespace ofxChoc
{
    class JsRuntime
    {
    public:
        JsRuntime() 
            : _context(choc::javascript::createQuickJSContext()), 
              _replActive(true), 
              _currentInput(""),
              _newInput(false) 
        {
        }

        ~JsRuntime()
        {
            _replActive = false;
            if (_inputThread.joinable()) {
                _inputThread.join(); // Wait for the input thread to finish
            }
        }

        void setup()
        {
            // Start the input thread for reading console input
            _inputThread = std::thread([this]() {
                while (_replActive) {
                    std::string input;
                    std::getline(std::cin, input);
                    if(!input.empty()){
                        _currentInput = input;
                        _newInput = true;
                    }
                }
            });
        }

        void startRepl(){
            _replActive = true;
        }
        void stopRepl(){
            _replActive = false;
        }

        bool evaluateFile(std::string path, bool watch = false){
            return false;
        }

        void update()
        {
            if (_newInput) {
                _newInput = false; // Reset the flag
                std::cout << "Evaluating input: " << _currentInput << std::endl;
                try
                {
                    auto result = _context.evaluateExpression(_currentInput);

                    if (result.isVoid())
                    {
                        std::cout << "undefined" << std::endl;
                    }
                    else
                    {
                        auto resultType = result.getType();
                        if(result.isBool()){
                            auto value = result.getBool();
                            if(value){
                                std::cout << "true" << std::endl;
                            }else{
                                std::cout << "false" << std::endl;
                            }

                        }
                        if(result.isFloat()){
                            std::cout << result.getFloat64() << std::endl;
                        }
                        else if(result.isFloat32()){
                            std::cout << result.getFloat32() << std::endl;
                        }
                        else if(result.isFloat64()){
                            std::cout << result.getFloat64() << std::endl;
                        }
                        else if(result.isInt()){
                            std::cout << result.getInt64() << std::endl;
                        }
                        else if(result.isInt32()){
                            std::cout << result.getInt32() << std::endl;
                        }
                        else if(result.isInt64()){
                            std::cout << result.getInt64() << std::endl;
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }

        void onKeyPressed(int key)
        {
        }


    // private:
        choc::javascript::Context _context;
        std::atomic<bool> _replActive;
        std::string _currentInput; // Stores the input from user until evaluation
        std::atomic<bool> _newInput; // Flag to indicate new input

        std::thread _inputThread; // Thread for reading console input
    };
}
