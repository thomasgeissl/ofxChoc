#pragma once
#include "ofMain.h"
#include "registry.h"
#include "helpers.h"
#include "../../libs/choc/javascript/choc_javascript_QuickJS.h"

inline void registerOfVideoBindings(choc::javascript::Context&        ctx,
                                     ObjectStore<ofVideoPlayer>&        players,
                                     ObjectStore<ofVideoGrabber>&       grabbers)
{
