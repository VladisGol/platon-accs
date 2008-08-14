######################################################################
# Automatically generated by qmake (2.01a) ?? ???. 14 18:29:31 2008
######################################################################
config += qt debug
win32:DEFINES += IBPP_WINDOWS
unix:DEFINES += IBPP_LINUX
unix:LIBS += -lfbclient
DEFINES -= UNICODE

TEMPLATE = app
TARGET = 
DEPENDPATH += . EssClasses EssWidgets ibpp/core
INCLUDEPATH += . EssClasses ibpp/core EssWidgets

# Input
HEADERS += EssentialClass.h \
           firstFormapp.h \
           phipotesystablemodel.h \
           qeidostreewidget.h \
           EssClasses/actclass.h \
           EssClasses/actcopy.h \
           EssClasses/acttype.h \
           EssClasses/associatedextraattribute.h \
           EssClasses/dbetc.h \
           EssClasses/eidos.h \
           EssClasses/extraatrib.h \
           EssClasses/hypotesis.h \
           EssClasses/objclass.h \
           EssClasses/objcopy.h \
           EssClasses/objtype.h \
           EssClasses/piterator.h \
           EssClasses/pragma.h \
           EssClasses/resclass.h \
           EssClasses/rescopy.h \
           EssClasses/restype.h \
           EssClasses/sqlmanager.h \
           EssClasses/transactionstru.h \
           EssWidgets/AbstractMemHypModel.h \
           EssWidgets/HypotesisMemModel.h \
           EssWidgets/hypotesismodel.h \
           EssWidgets/PragmaMemModel.h \
           EssWidgets/pragmamodel.h \
           ibpp/core/_ibpp.h \
           ibpp/core/ibase.h \
           ibpp/core/iberror.h \
           ibpp/core/ibpp.h \
           ibpp/core/_ibpp.cpp \
           ibpp/core/_dpb.cpp \
           ibpp/core/_ibs.cpp \
           ibpp/core/_rb.cpp \
           ibpp/core/_spb.cpp \
           ibpp/core/_tpb.cpp \
           ibpp/core/array.cpp \
           ibpp/core/blob.cpp \
           ibpp/core/database.cpp \
           ibpp/core/date.cpp \
           ibpp/core/dbkey.cpp \
           ibpp/core/events.cpp \
           ibpp/core/exception.cpp \
           ibpp/core/row.cpp \
           ibpp/core/service.cpp \
           ibpp/core/statement.cpp \
           ibpp/core/time.cpp \
           ibpp/core/transaction.cpp \
           ibpp/core/user.cpp
FORMS += MyFirstForm.ui
SOURCES += firstFormapp.cpp \
           main.cpp \
           phipotesystablemodel.cpp \
           qeidostreewidget.cpp \
           EssClasses/actclass.cpp \
           EssClasses/actcopy.cpp \
           EssClasses/acttype.cpp \
           EssClasses/AssociatedExtraAttribute.cpp \
           EssClasses/dbetc.cpp \
           EssClasses/eidos.cpp \
           EssClasses/extraatrib.cpp \
           EssClasses/hypotesis.cpp \
           EssClasses/objclass.cpp \
           EssClasses/objcopy.cpp \
           EssClasses/objtype.cpp \
           EssClasses/piterator.cpp \
           EssClasses/pragma.cpp \
           EssClasses/resclass.cpp \
           EssClasses/rescopy.cpp \
           EssClasses/restype.cpp \
           EssClasses/sqlmanager.cpp \
           EssWidgets/AbstractMemHypModel.cpp \
           EssWidgets/HypotesisMemModel.cpp \
           EssWidgets/hypotesismodel.cpp \
           EssWidgets/PragmaMemModel.cpp \
           EssWidgets/pragmamodel.cpp \
           ibpp/core/_dpb.cpp \
           ibpp/core/_ibpp.cpp \
           ibpp/core/_ibs.cpp \
           ibpp/core/_rb.cpp \
           ibpp/core/_spb.cpp \
           ibpp/core/_tpb.cpp \
           ibpp/core/all_in_one.cpp \
           ibpp/core/array.cpp \
           ibpp/core/blob.cpp \
           ibpp/core/database.cpp \
           ibpp/core/date.cpp \
           ibpp/core/dbkey.cpp \
           ibpp/core/events.cpp \
           ibpp/core/exception.cpp \
           ibpp/core/row.cpp \
           ibpp/core/service.cpp \
           ibpp/core/statement.cpp \
           ibpp/core/time.cpp \
           ibpp/core/transaction.cpp \
           ibpp/core/user.cpp
