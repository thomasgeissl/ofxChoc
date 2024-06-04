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
            choc::ui::WebView::Options opts;
            opts.enableDebugMode = true;
            _webview = std::make_unique<choc::ui::WebView>(opts);
            _webview->addInitScript(
                R"xxx(
    window.___ofxChoc2Js = function(event, value) {
        if (ofxChoc.listeners[event]) {
            ofxChoc.listeners[event](value);
        }
    }; 
    window.ofxChoc = {
        listeners: {},
        addListener: function (event, callback) {
            this.listeners[event] = callback;
        },
        notifyEvent: function (event, value) {
            ___js2ofxChoc(event, value);
        }
    };
)xxx");
            _window.setContent(_webview->getViewHandle());
        };
        ~WebView()
        {
            // stopMessageLoop();
        }
        void setup(std::string title = "ofxChoc::WebView")
        {
            choc::ui::setWindowsDPIAwareness();
            _window.setWindowTitle(title);
            _window.setResizable(true);
            _window.setMinimumSize(300, 300);
            _window.setMaximumSize(1500, 1200);
            _window.windowClosed = []
            { choc::messageloop::stop(); };

            _window.toFront();

            // Bind the JavaScript function to the C++ callback
            _webview->bind("___js2ofxChoc", [this](const choc::value::ValueView &args) -> choc::value::Value
                           {
        auto message = "___js2ofxChoc() called with args: " + choc::json::toString(args);

        Event event;
        auto payload = ofJson::parse(choc::json::toString(args));
        // TODO: proper error handling
        event.name = payload[0];
        event.value = payload[1];

        _event.notify(event);

        return choc::value::createString(message); });

            startMessageLoop();
        }

        void update()
        {
        }
        void navigate(std::string url)
        {
            _webview->navigate(url);
        }
        void setHTML(std::string html)
        {
            _webview->setHTML(html);
        }
        void notifyEvent(std::string event, ofJson value)
        {
            std::string js = "window.___ofxChoc2Js(";
            js += "\"" + event + "\"";
            js += ", ";
            js += value.dump();
            js += ")";
            _webview->evaluateJavascript(js);
        }
        void startMessageLoop()
        {
            choc::messageloop::initialise();
            // _messageLoopThread = std::thread([]()
            //                                  { choc::messageloop::run(); });
        }
        void stopMessageLoop()
        {
            choc::messageloop::stop();
            if (_messageLoopThread.joinable())
            {
                _messageLoopThread.join();
            }
        }
        choc::ui::WebView *getWebViewPtr()
        {
            return _webview.get();
        }
        choc::ui::DesktopWindow _window;
        std::unique_ptr<choc::ui::WebView> _webview;
        std::thread _messageLoopThread;
        ofEvent<Event> _event;
    };
};
