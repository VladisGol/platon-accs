TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
unix: LIBS += -lfbclient -L./LIBS
LIBS +=-lwthttp
LIBS +=-lwt
LIBS +=-lboost_signals
win32: DEFINES += IBPP_WINDOWS
unix:DEFINES += IBPP_LINUX IBPP_GCC
DEPENDPATH += EssClasses
INCLUDEPATH += EssClasses
unix:DEPENDPATH += ibpp/core
win32:DEPENDPATH += ibpp\core
unix:INCLUDEPATH += ibpp/core
win32:INCLUDEPATH += ibpp\core

SOURCES += main.cpp \
    ibpp/core/user.cpp \
    ibpp/core/transaction.cpp \
    ibpp/core/time.cpp \
    ibpp/core/statement.cpp \
    ibpp/core/service.cpp \
    ibpp/core/row.cpp \
    ibpp/core/exception.cpp \
    ibpp/core/events.cpp \
    ibpp/core/dbkey.cpp \
    ibpp/core/date.cpp \
    ibpp/core/database.cpp \
    ibpp/core/blob.cpp \
    ibpp/core/array.cpp \
    ibpp/core/all_in_one.cpp \
    ibpp/core/_tpb.cpp \
    ibpp/core/_spb.cpp \
    ibpp/core/_rb.cpp \
    ibpp/core/_ibs.cpp \
    ibpp/core/_ibpp.cpp \
    ibpp/core/_dpb.cpp \
    EssClasses/sqlmanager.cpp \
    EssClasses/restype.cpp \
    EssClasses/rescopy.cpp \
    EssClasses/resclass.cpp \
    EssClasses/pragma.cpp \
    EssClasses/piterator.cpp \
    EssClasses/objtype.cpp \
    EssClasses/objcopy.cpp \
    EssClasses/objclass.cpp \
    EssClasses/hypotesis.cpp \
    EssClasses/extraatrib.cpp \
    EssClasses/eidos.cpp \
    EssClasses/dbetc.cpp \
    EssClasses/AssociatedExtraAttribute.cpp \
    EssClasses/acttype.cpp \
    EssClasses/actcopy.cpp \
    EssClasses/actclass.cpp \
    WT_models/HypotesisMemModel.cpp \
    WT_models/AbstractMemHypModel.cpp \
    WT_models/PragmaMemModel.cpp

HEADERS += \
    ibpp/core/ibpp.h \
    ibpp/core/iberror.h \
    ibpp/core/ibase.h \
    ibpp/core/_ibpp.h \
    EssClasses/transactionstru.h \
    EssClasses/sqlmanager.h \
    EssClasses/restype.h \
    EssClasses/rescopy.h \
    EssClasses/resclass.h \
    EssClasses/pragma.h \
    EssClasses/piterator.h \
    EssClasses/objtype.h \
    EssClasses/objcopy.h \
    EssClasses/objclass.h \
    EssClasses/hypotesis.h \
    EssClasses/extraatrib.h \
    EssClasses/eidos.h \
    EssClasses/dbetc.h \
    EssClasses/associatedextraattribute.h \
    EssClasses/acttype.h \
    EssClasses/actcopy.h \
    EssClasses/actclass.h \
    EssentialClass.h \
    WT_models/HypotesisMemModel.h \
    WT_models/AbstractMemHypModel.h \
    WT_models/PragmaMemModel.h


