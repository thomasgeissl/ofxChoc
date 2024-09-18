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
              _gameLoopActive(false),
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

        bool exists(std::string name){
            std::string expression = name +" != null && " + name + "!= undefined";
            return _context.evaluateExpression(expression).getBool();
        }

        bool evaluateFile(std::filesystem::path path, bool gameLoopActive){
            ofFile file(path);
            if (file.exists())
            {
                ofBuffer buffer = file.readToBuffer();
                std::string fileContent = buffer.getText();
                ofLog() << "File Content: " << fileContent;
                try{
                    _context.evaluateExpression(fileContent);
                }catch(choc::javascript::Error& e){
                    ofLogError() << e.what();
                    return false;
                }
                _gameLoopActive = gameLoopActive;
                // TODO: do we need to call setup?
                return true;
            }
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
            if(_gameLoopActive){
                if(exists("update")){
                    _context.run("update()");
                }else{
                    ofLogNotice() << "has no update";
                }
            }
        }

        void draw(){
            if(_gameLoopActive){
                if(exists("draw")){
                    _context.run("draw()");
                }
            }
        }

        void onKeyPressed(int key)
        {
            if(_gameLoopActive){
                if(exists("keyPressed")){
                    std::string expression = "keyPressed("+ofToString(key)+")";
                    _context.run(expression);
                }
            }
        }
        void onKeyReleased(int key)
        {
            if(_gameLoopActive){
                if(exists("keyReleased")){
                    std::string expression = "keyReleased("+ofToString(key)+")";
                    _context.run(expression);
                }
            }
        }
        void onMouseMoved(int x, int y)
        {
            if(_gameLoopActive){
                if(exists("mouseMoved")){
                    std::string expression = "mouseMoved("+ofToString(x)+", "+ofToString(y)+")";
                    _context.run(expression);
                }
            }
        }
        void onMouseDragged(int x, int y, int button)
        {
            if(_gameLoopActive){
                if(exists("mouseDragged")){
                    std::string expression = "mouseDragged("+ofToString(x)+", "+ofToString(y)+", "+ofToString(button)+")";
                    _context.run(expression);
                }
            }
        }
        void onMousePressed(int x, int y, int button)
        {
            if(_gameLoopActive){
                if(exists("mousePressed")){
                    std::string expression = "mousePressed("+ofToString(x)+", "+ofToString(y)+", "+ofToString(button)+")";
                    _context.run(expression);
                }
            }
        }
        void onMouseReleased(int x, int y, int button)
        {
            if(_gameLoopActive){
                if(exists("mouseReleased")){
                    std::string expression = "mouseReleased("+ofToString(x)+", "+ofToString(y)+", "+ofToString(button)+")";
                    _context.run(expression);
                }
            }
        }
        void onMouseEntered(int x, int y)
        {
            if(_gameLoopActive){
                if(exists("mouseEntered")){
                    std::string expression = "mouseEntered("+ofToString(x)+", "+ofToString(y)+")";
                    _context.run(expression);
                }
            }
        }
        void onMouseExited(int x, int y)
        {
            if(_gameLoopActive){
                if(exists("mouseExited")){
                    std::string expression = "mouseExited("+ofToString(x)+", "+ofToString(y)+")";
                    _context.run(expression);
                }
            }
        }
        void onWindowResized(int w, int h)
        {
            if(_gameLoopActive){
                if(exists("windowResized")){
                    std::string expression = "windowResized("+ofToString(w)+", "+ofToString(h)+")";
                    _context.run(expression);
                }
            }
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
