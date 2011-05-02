#-------------------------------------------------
#
# Project created by QtCreator 2011-05-02T12:08:40
#
#-------------------------------------------------

TARGET = simple_lib
TEMPLATE = lib

DEFINES += SIMPLE_LIB_LIBRARY

SOURCES += simple_lib.cpp

HEADERS += simple_lib.h\
        simple_lib_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7058728
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = simple_lib.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
