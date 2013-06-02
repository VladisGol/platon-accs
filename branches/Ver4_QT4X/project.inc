config += qt debug
win32: DEFINES += IBPP_WINDOWS
unix: DEFINES += IBPP_LINUX
unix: LIBS += -lfbclient
