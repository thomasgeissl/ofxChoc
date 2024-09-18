#pragma once
#include "ofMain.h"
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "../libs/choc/javascript/choc_javascript_Console.h"
#include <thread>
#include <atomic>
#include <string>
#include "./ofBindings/0-12-0/bindings.h"

namespace ofxChoc
{
    class JsRuntime
    {
    public:
        JsRuntime() 
            : _context(choc::javascript::createQuickJSContext()), 
              _replActive(false), 
              _gameLoopActive(true),
              _currentInput(""),
              _newInput(false) 
        {
            choc::javascript::registerConsoleFunctions(_context);
            registerOfBindings(_context);
        }

        ~JsRuntime()
        {
            stopRepl();
        }

        void setup()
        {
        }

        void startRepl(){
            _replActive = true;
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
        void stopRepl(){
            _replActive = false;
            if (_inputThread.joinable()) {
                _inputThread.join(); 
            }
        }
        void startGameLoop(){
            _gameLoopActive = true;
        }
        void stopGameLoop(){
            _gameLoopActive = false;
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

                if(_gameLoopActive){
                    if(_context.evaluateExpression("update")){
                        _context.run("update");
                    }
                }
            }
        }

        void draw(){
            if(_gameLoopActive){
                if(_context.evaluateExpression("draw")){
                    _context.run("draw");
                }
            }
        }

        void onKeyPressed(int key)
        {
        }


    // private:
        choc::javascript::Context _context;
        std::atomic<bool> _replActive;
        bool _gameLoopActive;
        std::string _currentInput; // Stores the input from user until evaluation
        std::atomic<bool> _newInput; // Flag to indicate new input

        std::thread _inputThread; // Thread for reading console input
    };
}
