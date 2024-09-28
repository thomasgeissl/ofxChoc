#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

int main(int argc, const char **argv)
{
    auto window = std::make_shared<ofAppNoWindow>();
    ofRunApp(window, std::make_shared<ofApp>());
    return ofRunMainLoop();
}
