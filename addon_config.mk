meta:
	ADDON_NAME = ofxChoc
	ADDON_DESCRIPTION = ofxAddon including the choc library by Julian Storer/tracktion
	ADDON_AUTHOR = Thomas Geissl
	ADDON_TAGS = "some useful utilities, e.g. a webview class"
	ADDON_URL = https://github.com/thomasgeissl/ofxChoc

common:
	ADDON_SOURCES_EXCLUDE = libs/choc/audio/io/rtaudio/%

osx:
	ADDON_FRAMEWORKS = WebKit

linux64:
	# 22.04: webkit2gtk-4.0; 24.04+: webkit2gtk-4.1 — gtk+-3.0 needed by choc_MessageLoop.h
	ADDON_PKG_CONFIG_LIBRARIES = gtk+-3.0 $(shell pkg-config --exists webkit2gtk-4.1 && echo webkit2gtk-4.1 || echo webkit2gtk-4.0)
