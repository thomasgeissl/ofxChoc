meta:
	ADDON_NAME = ofxChoc
	ADDON_DESCRIPTION = ofxAddon including the choc library by Julian Storer (the juce guy)
	ADDON_AUTHOR = Thomas Geissl
	ADDON_TAGS = "some useful utilities, e.g. a webview class"
	ADDON_URL = https://github.com/thomasgeissl/ofxChoc

osx:
	ADDON_FRAMEWORKS = WebKit

linux64:
	ADDON_PKG_CONFIG_LIBRARIES = webkit2gtk-4.0
