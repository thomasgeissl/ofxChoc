#pragma once
#include "ofMain.h"
#include "../libs/httplib/httplib.h"
// https://github.com/yhirose/cpp-httplib
using namespace httplib;
namespace ofxChoc
{
  class WebServer : public ofThread
  {
  public:
    void setup(int port, std::string rootPath)
    {
      _port = port;
      _server.Get("/hi", [](const Request &req, Response &res)
                  { res.set_content("Hello World!", "text/plain"); });

      auto ret = _server.set_mount_point("/", rootPath);
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
}