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
            _webview->bind("eventCallbackFn", [this](const choc::value::ValueView &args) -> choc::value::Value
                           {
        auto message = "eventCallbackFn() called with args: " + choc::json::toString(args);

        Event event;
        event.name = "event";
        event.value = ofJson::parse(choc::json::toString(args));

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
        void notifyEvent(ofJson value){
            std::string js = "console.log(";
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
