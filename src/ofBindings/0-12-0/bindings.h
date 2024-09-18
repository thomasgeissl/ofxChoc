#pragma once

#include "./application.h"
#include "./utils.h"
#include "./gl.h"
#include "./events.h"

inline void registerOfBindings(choc::javascript::Context &context){
    registerOfApplicationBindings(context);
    registerOfUtilsBindings(context);
    registerOfGlBindings(context);
    registerOfEventsBindings(context);

    context.run (R"(

of = {
    ...ofxChoc_application,
    ...ofxChoc_utils,
    ...ofxChoc_gl,
    ...ofxChoc_events,
};

    )");
}

