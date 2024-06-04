#pragma once
#include "ofMain.h"
#include "httplib.h"
using namespace httplib;
// https://github.com/yhirose/cpp-httplib

class WebServer : public ofThread
{
public:
  void setup(int port)
  {
    _port = port;
    _server.Get("/hi", [](const Request &req, Response &res)
                { res.set_content("Hello World!", "text/plain"); });

    auto ret = _server.set_mount_point("/", ofToDataPath("webview/dist"));
    if (!ret)
    {
      // The specified base directory doesn't exist...
    }

    startThread(true); // blocking
  }

  void threadedFunction()
  {

    // start
    // while(isThreadRunning()) {
    _server.listen("localhost", _port);
    // }
    // done
  }
  void stop()
  {
    _server.stop();
  }

  httplib::Server _server;
  int _port;
};