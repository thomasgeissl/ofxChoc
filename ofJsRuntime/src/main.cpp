#include "ofMain.h"
#include "ofApp.h"
//#include "ofxChoc.h"

int main(int argc, const char **argv)
{
    auto width = 2014;
    auto height = 768;
    auto watch = false;
    auto gameLoopActive = true;
    choc::ArgumentList args (argc, argv);

    // Resolve file path BEFORE ofSetupOpenGL, which changes cwd to the app bundle on macOS
    std::filesystem::path filePath;
    if(args.contains("--file")){
        filePath = std::filesystem::absolute(args.getExistingFile("--file", false));
    } else if(args.contains("-f")){
        filePath = std::filesystem::absolute(args.getExistingFile("-f", false));
    }

    if(args.contains("--watch")){
        watch = true;
    }

    ofGLWindowSettings glSettings;
    glSettings.setGLVersion(3, 2);
    glSettings.setSize(width, height);
    ofCreateWindow(glSettings);

    if(!filePath.empty()){
        return ofRunApp(new ofApp(filePath, gameLoopActive, watch));
    } else {
        return ofRunApp(new ofApp());
    }
}
