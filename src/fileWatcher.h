#pragma once
#include "ofMain.h"
#include "../libs/choc/platform/choc_FileWatcher.h"

namespace ofxChoc
{
    class FileWatcher
    {
    public:
        FileWatcher(std::filesystem::path fileOrFolderToWatch,
                    uint32_t millisecondsBetweenChecks = 0) : _watcher(choc::file::Watcher(fileOrFolderToWatch, [this](const choc::file::Watcher::Event &event) -> void
                                                                                           {
                                                                                               choc::file::Watcher::Event eventCopy = event;
                                                                                               _event.notify(eventCopy);
                                                                                           },
                                                                                           millisecondsBetweenChecks))
        {
        }
        choc::file::Watcher _watcher;
        ofEvent<choc::file::Watcher::Event> _event;
    };
};
