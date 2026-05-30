#pragma once

#include "../../../libs/choc/gui/choc_WebView.h"
#include "./bindings.h"
#include "./events.h"

inline void registerOfEventsBindings(choc::ui::WebView *webview)
{
    webview->bind("_ofxChoc_ofBindings_getKeyPressed",
                  [](const choc::value::ValueView &args) -> choc::value::Value
                  {
                      if (args.size() == 1)
                          return choc::value::createBool(ofGetKeyPressed((int)args[0].getFloat64()));
                      return choc::value::createBool(false);
                  });

    webview->evaluateJavascript(R"(
ofxChoc_events = {
    getKeyPressed: function(keyCode) { return _ofxChoc_ofBindings_getKeyPressed(keyCode) },
};
)");
}

inline void registerOfBindings(choc::ui::WebView *webview)
{
    registerOfEventsBindings(webview);

    webview->evaluateJavascript(R"(
of = {
    // ...ofxChoc_application,
    // ...ofxChoc_utils,
    // ...ofxChoc_gl,
    // ...ofxChoc_graphics,
    ...ofxChoc_events,
};
    )");
}
