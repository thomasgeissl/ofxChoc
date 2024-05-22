#pragma once

#include "./libs/choc/gui/choc_WebView.h"
#include "./libs/choc/gui/choc_DesktopWindow.h"
#include "./libs/choc/containers/choc_ArgumentList.h"

namespace ofxChoc
{

    class WebView
    {
    public:
        struct Event
        {
            std::string name;
            ofJson value;
        };
        WebView() : _window({100, 100, 800, 600})
        {
            _window.setContent(_webview.getViewHandle());
        };
        ~WebView()
        {
            stopMessageLoop();
        }
        void setup(std::string title = "ofxCho::WebView")
        {
            choc::ui::setWindowsDPIAwareness();
            _window.setWindowTitle(title);
            _window.setResizable(true);
            _window.setMinimumSize(300, 300);
            _window.setMaximumSize(1500, 1200);
            _window.windowClosed = []
            { choc::messageloop::stop(); };

            _window.toFront();

            _webview.bind("eventCallbackFn", [this](const choc::value::ValueView &args) -> choc::value::Value
                          {
                auto message = "eventCallbackFn() called with args: " + choc::json::toString (args);
                choc::messageloop::postMessage ([this, message]
                    {
                        ofLogNotice() << "WebView callback message: " << message;
                        Event event;
                        event.name = "event";
                        event.value = ofJson::parse(message);
                        this->_event.notify(event);
                    }
                );
                return choc::value::createString (message); });

            Event event;
            event.name = "event";
            _event.notify(event);
        }
        void update()
        {
        }
        void navigate(std::string url)
        {
            _webview.navigate(url);
        }
        void setHTML(std::string html)
        {
            _webview.setHTML(html);
        }
        void startMessageLoop()
        {
            _messageLoopThread = std::thread([]()
            {
                choc::messageloop::run();
            });
        }
        void stopMessageLoop()
        {
            choc::messageloop::stop();
            if (_messageLoopThread.joinable())
            {
                _messageLoopThread.join();
            }
        }
        choc::ui::DesktopWindow _window;
        choc::ui::WebView _webview;
        std::thread _messageLoopThread;
        ofEvent<Event> _event;
    };
};