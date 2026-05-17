#pragma once
#include "ofMain.h"
#include "../libs/choc/javascript/choc_javascript_QuickJS.h"
#include "../libs/choc/javascript/choc_javascript_Console.h"
#include <thread>
#include <atomic>
#include <string>
#include "./ofBindings/0-12-1/bindings.h"
#include "./ofBindings/0-12-1/objectBindings.h"

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
            registerOfObjectBindings(_context, _registry);
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
            std::string expression = "typeof " + name + " === 'function'";
            return _context.evaluateExpression(expression).getBool();
        }

        void invoke(const std::string& expression){
            try {
                _context.evaluateExpression(expression);
            } catch(choc::javascript::Error& e) {
                ofLogError("ofJsRuntime") << e.what();
            }
        }

        bool evaluateFile(std::filesystem::path path, bool gameLoopActive){
            ofFile file(path);
            if (!file.exists())
            {
                ofLogError("ofJsRuntime") << "File not found: " << path;
                return false;
            }
            // Release any C++ objects created by the previous script run
            _registry.clear();

            // Expose the script's directory as __dirname (absolute path, no trailing slash)
            std::string dir = path.parent_path().string();
            // Escape backslashes for Windows paths just in case
            for (auto& c : dir) if (c == '\\') c = '/';
            try { _context.evaluateExpression("var __dirname = '" + dir + "';"); }
            catch (...) {}

            ofBuffer buffer = file.readToBuffer();
            std::string fileContent = buffer.getText();
            try{
                _context.evaluateExpression(fileContent);
            }catch(choc::javascript::Error& e){
                ofLogError("ofJsRuntime") << "JS error in " << path << ": " << e.what();
                return false;
            }
            _gameLoopActive = gameLoopActive;
            return true;
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
                    invoke("update()");
                }
            }
        }

        void draw(){
            if(_gameLoopActive){
                if(exists("draw")){
                    invoke("draw()");
                }
            }
        }

        void onKeyPressed(int key)
        {
            if(_gameLoopActive && exists("keyPressed"))
                invoke("keyPressed("+ofToString(key)+")");
        }
        void onKeyReleased(int key)
        {
            if(_gameLoopActive && exists("keyReleased"))
                invoke("keyReleased("+ofToString(key)+")");
        }
        void onMouseMoved(int x, int y)
        {
            if(_gameLoopActive && exists("mouseMoved"))
                invoke("mouseMoved("+ofToString(x)+","+ofToString(y)+")");
        }
        void onMouseDragged(int x, int y, int button)
        {
            if(_gameLoopActive && exists("mouseDragged"))
                invoke("mouseDragged("+ofToString(x)+","+ofToString(y)+","+ofToString(button)+")");
        }
        void onMousePressed(int x, int y, int button)
        {
            if(_gameLoopActive && exists("mousePressed"))
                invoke("mousePressed("+ofToString(x)+","+ofToString(y)+","+ofToString(button)+")");
        }
        void onMouseReleased(int x, int y, int button)
        {
            if(_gameLoopActive && exists("mouseReleased"))
                invoke("mouseReleased("+ofToString(x)+","+ofToString(y)+","+ofToString(button)+")");
        }
        void onMouseEntered(int x, int y)
        {
            if(_gameLoopActive && exists("mouseEntered"))
                invoke("mouseEntered("+ofToString(x)+","+ofToString(y)+")");
        }
        void onMouseExited(int x, int y)
        {
            if(_gameLoopActive && exists("mouseExited"))
                invoke("mouseExited("+ofToString(x)+","+ofToString(y)+")");
        }
        void onWindowResized(int w, int h)
        {
            if(_gameLoopActive && exists("windowResized"))
                invoke("windowResized("+ofToString(w)+","+ofToString(h)+")");
        }
        void onDragEvent(ofDragInfo dragInfo){
            if(_gameLoopActive && exists("dragEvent"))
                invoke("dragEvent("+ofToString(dragInfo.position.x)+","+ofToString(dragInfo.position.y)+")");
        }
        void onMessage(ofMessage message){
            if(_gameLoopActive){
                if(exists("gotMessage")){
                    // std::string expression = "gotMessage("+ofToString(message.message)+", "+ofToString(message.arg1)+", "+ofToString(message.arg2)+", "+ofToString(message.arg3)+")";
                    // _context.run(expression);
                }
            }
        }



    // private:
        choc::javascript::Context _context;
        Registry _registry;
        std::atomic<bool> _replActive;
        bool _gameLoopActive;
        std::string _currentInput; // Stores the input from user until evaluation
        std::atomic<bool> _newInput; // Flag to indicate new input
        std::thread _inputThread; // Thread for reading console input
    };
}
