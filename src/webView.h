#pragma once
#include "ofMain.h"
#include "../libs/choc/gui/choc_WebView.h"
#include "../libs/choc/gui/choc_DesktopWindow.h"

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
        struct DomEvent
        {
            std::string selector;
            std::string eventName;
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
            for(var i = 0; i < ofxChoc.listeners[event].length; i++){
                ofxChoc.listeners[event][i](value);
            }
        }
    }; 
    window.ofxChoc = {
        listeners: {},
        addListener: function (event, callback) {
            if(!this.listeners[event]){
                this.listeners[event] = []
            }
            this.listeners[event].push(callback);
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

        if(event.name == "dom"){
            DomEvent domEvent;
            domEvent.eventName = event.value["eventName"].get<std::string>();
            domEvent.selector = event.value["selector"].get<std::string>();
            domEvent.value = event.value["event"];
            _domEvent.notify(domEvent);
        }else{
            _event.notify(event);
        }

            return choc::value::createString(message); });
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

        choc::ui::WebView *getWebViewPtr()
        {
            return _webview.get();
        }
        void addDomListener(std::string selector, std::string event)
        {
            std::string js = R"(
        function serializeEvent(event) {
            const commonProps = {
                isTrusted: event.isTrusted,
                type: event.type,
                timeStamp: event.timeStamp
            };

            let specificProps = {};

            if (event instanceof MouseEvent) {
                specificProps = {
                    screenX: event.screenX,
                    screenY: event.screenY,
                    clientX: event.clientX,
                    clientY: event.clientY,
                    ctrlKey: event.ctrlKey,
                    shiftKey: event.shiftKey,
                    altKey: event.altKey,
                    metaKey: event.metaKey,
                    button: event.button,
                    buttons: event.buttons,
                    relatedTarget: event.relatedTarget,
                    x: event.x,
                    y: event.y,
                    offsetX: event.offsetX,
                    offsetY: event.offsetY,
                    pageX: event.pageX,
                    pageY: event.pageY,
                    movementX: event.movementX,
                    movementY: event.movementY,
                    region: event.region
                };
            } else if (event instanceof KeyboardEvent) {
                specificProps = {
                    key: event.key,
                    code: event.code,
                    location: event.location,
                    ctrlKey: event.ctrlKey,
                    shiftKey: event.shiftKey,
                    altKey: event.altKey,
                    metaKey: event.metaKey,
                    repeat: event.repeat,
                    isComposing: event.isComposing,
                    charCode: event.charCode,
                    keyCode: event.keyCode,
                    which: event.which
                };
            } else if (event instanceof Event && event.type === 'load') {
                specificProps = {
                    // Add any specific properties for the load event if necessary
                };
            } else if (event instanceof Event) {
                specificProps = {
                    // Handle other common events and their properties
                };
            }

            return ({ ...commonProps, ...specificProps });
        }
    )";

            if (selector == "window")
            {
                js += "window.addEventListener('";
            }
            else if (selector == "document")
            {
                js += "document.addEventListener('";
            }
            else
            {
                js += "document.querySelector('";
                js += selector;
                js += "').addEventListener('";
            }

            js += event;
            js += "', function(e){ const serializedEvent = serializeEvent(e); window.ofxChoc.notifyEvent('dom', {selector: '" + selector + "', eventName: '" + event + "', event: serializedEvent}); });";

            _webview->evaluateJavascript(js);
        }

        choc::ui::DesktopWindow _window;
        std::unique_ptr<choc::ui::WebView> _webview;
        ofEvent<Event> _event;
        ofEvent<DomEvent> _domEvent;
    };
};
