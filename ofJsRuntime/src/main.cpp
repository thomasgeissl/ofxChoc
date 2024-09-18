#include "ofMain.h"
#include "ofApp.h"
//#include "ofxChoc.h"

int main(int argc, const char **argv)
{
    auto width = 2014;
    auto height = 768;
    auto watch = false;
    choc::ArgumentList args (argc, argv);
	ofSetupOpenGL(width, height, OF_WINDOW);

    if(args.contains("--watch")){
        watch = true;
    }

    if(args.contains("--file")){
	    return ofRunApp(new ofApp(args.getExistingFile("--file", false), watch));
    }if(args.contains("-f")){
	    return ofRunApp(new ofApp(args.getExistingFile("-f", false), watch));
    }else{
	    return ofRunApp(new ofApp());
    }
}
