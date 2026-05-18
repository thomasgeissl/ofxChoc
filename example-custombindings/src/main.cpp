#include "ofMain.h"
#include "ofApp.h"

int main()
{
    ofGLWindowSettings glSettings;
    glSettings.setGLVersion(3, 2);
    glSettings.setSize(800, 300);
    ofCreateWindow(glSettings);

    return ofRunApp(new ofApp());
}
