#include "ofMain.h"
#include "ofApp.h"
//#include "ofxChoc.h"

int main(int argc, const char **argv)
{
	// choc::ui::DesktopWindow _window({100, 100, 800, 600});
	// choc::ui::WebView _webview;
	// choc::ui::setWindowsDPIAwareness();
	// _window.setWindowTitle("title");
	// _window.setResizable(true);
	// _window.setMinimumSize(300, 300);
	// _window.setMaximumSize(1500, 1200);
	// _window.windowClosed = []
	// { choc::messageloop::stop(); };
	// _window.setContent(_webview.getViewHandle());

	// _webview.setHTML(R"xxx(
    //   <!DOCTYPE html> <html>
    //     <head> <title>Page Title</title> </head>
    //     <script>
    //       var eventCounter = 0;
    //       function sendEvent()
    //       {
    //         // When you invoke a function, it returns a Promise object
    //         eventCallbackFn({ counter: ++eventCounter }, "Hello World")
    //           .then ((result) => { document.getElementById ("eventResultDisplay").innerText = result; });
    //       }
    //     </script>

    //     <body>
    //       <h1>CHOC WebView Demo</h1>
    //       <p>This is a demo of a choc::ui::WebView window</p>
    //       <p><button onclick="sendEvent()">Click to invoke an event callback</button></p>
    //       <p><input type="file" /></p>
    //       <p id="eventResultDisplay"></p>
    //     </body>
    //   </html>
    // )xxx");

	// _webview.bind("eventCallbackFn", [](const choc::value::ValueView &args) -> choc::value::Value
	// 			  {
    //             auto message = "eventCallbackFn() called with args: " + choc::json::toString (args);
    //             choc::messageloop::postMessage ([message]
    //                 {
    //                     ofLogNotice() << "WebView callback message: " << message;
    //                 }
    //             );
    //             return choc::value::createString (message); });
	// _window.toFront();
	// choc::messageloop::run();
	// return -1;

	ofSetupOpenGL(1024, 768, OF_WINDOW);
	return ofRunApp(new ofApp());
}
