config += qt \
    debug
win32:DEFINES += IBPP_WINDOWS
unix:DEFINES += IBPP_LINUX IBPP_GCC
unix:LIBS += -lfbclient -L./lib
# #####################################################################
# Automatically generated by qmake (2.01a) ?? ????. 13 08:52:31 2010
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    EssClasses \
    EssForms \
    EssWidgets \
    EidosStudio \
    images
unix:DEPENDPATH += ibpp/core
unix:DEPENDPATH += EssWidgets/hugo_model
win32:DEPENDPATH += ibpp\core
win32:DEPENDPATH += EssWidgets\hugo_model
INCLUDEPATH += . \
    EssForms \
    EssClasses \
    EssWidgets \
    EidosStudio \
    EssWidgets \
    images
unix:INCLUDEPATH += ibpp/core
unix:INCLUDEPATH += EssWidgets/hugo_model
win32:INCLUDEPATH += ibpp\core
win32:INCLUDEPATH += EssWidgets\hugo_model

# Input
HEADERS += es_mainwindow.h \
    aboutprogram.h \
    DataClass.h \
    EssentialClass.h \
    mainWin.h \
    actclass.h \
    actcopy.h \
    acttype.h \
    associatedextraattribute.h \
    dbetc.h \
    eidos.h \
    extraatrib.h \
    hypotesis.h \
    objclass.h \
    objcopy.h \
    objtype.h \
    piterator.h \
    pragma.h \
    resclass.h \
    rescopy.h \
    restype.h \
    sqlmanager.h \
    transactionstru.h \
    abstracthip_editform.h \
    DialogchoiceEidos.h \
    DialogchoiceHypotesis.h \
    DialogchoicePragma.h \
    DialogEditHronologyEA.h \
    DialogLogin.h \
    HypotesisEditForm.h \
    LinksExplorer.h \
    Multilinks.h \
    PragmaEditForm.h \
    AbstractMemHypModel.h \
    commonroutines.h \
    EAOneFrame.h \
    EidosMemModel.h \
    HronologyEaMemModel.h \
    HypotesisMemModel.h \
    HypPragmaMemModel.h \
    LnkComboBox.h \
    LnkdEidosMemModel.h \
    LnkdHypMemModel.h \
    LnkdPragmaMemModel.h \
    PragmaMemModel.h \
    qeidostreewidget.h \
    hypotesismodel.h \
    pragmamodel.h \
    _ibpp.h \
    ibase.h \
    iberror.h \
    ibpp.h \
    _ibpp.cpp \
    _dpb.cpp \
    _ibs.cpp \
    _rb.cpp \
    _spb.cpp \
    _tpb.cpp \
    array.cpp \
    blob.cpp \
    database.cpp \
    date.cpp \
    dbkey.cpp \
    events.cpp \
    exception.cpp \
    row.cpp \
    service.cpp \
    statement.cpp \
    time.cpp \
    transaction.cpp \
    user.cpp \
    DLL_plugin.h \
    dll_handler.h
FORMS += es_mainwindow.ui \
    aboutprogram.ui \
    mainwindow.ui \
    HypotesisEditForm.ui \
    LinksExplorer.ui \
    Multilinks.ui \
    dll_handler.ui
SOURCES += es_mainwindow.cpp \
    aboutprogram.cpp \
    DataClass.cpp \
    main.cpp \
    mainWin.cpp \
    actclass.cpp \
    actcopy.cpp \
    acttype.cpp \
    AssociatedExtraAttribute.cpp \
    dbetc.cpp \
    eidos.cpp \
    extraatrib.cpp \
    hypotesis.cpp \
    objclass.cpp \
    objcopy.cpp \
    objtype.cpp \
    piterator.cpp \
    pragma.cpp \
    resclass.cpp \
    rescopy.cpp \
    restype.cpp \
    sqlmanager.cpp \
    abstracthip_editform.cpp \
    DialogchoiceEidos.cpp \
    DialogchoiceHypotesis.cpp \
    DialogchoicePragma.cpp \
    DialogEditHronologyEA.cpp \
    DialogLogin.cpp \
    HypotesisEditForm.cpp \
    LinksExplorer.cpp \
    Multilinks.cpp \
    PragmaEditForm.cpp \
    AbstractMemHypModel.cpp \
    commonroutines.cpp \
    EAOneFrame.cpp \
    EidosMemModel.cpp \
    HronologyEaMemModel.cpp \
    HypotesisMemModel.cpp \
    HypPragmaMemModel.cpp \
    LnkComboBox.cpp \
    LnkdEidosMemModel.cpp \
    LnkdHypMemModel.cpp \
    LnkdPragmaMemModel.cpp \
    PragmaMemModel.cpp \
    qeidostreewidget.cpp \
    hypotesismodel.cpp \
    pragmamodel.cpp \
    _dpb.cpp \
    _ibpp.cpp \
    _ibs.cpp \
    _rb.cpp \
    _spb.cpp \
    _tpb.cpp \
    all_in_one.cpp \
    array.cpp \
    blob.cpp \
    database.cpp \
    date.cpp \
    dbkey.cpp \
    events.cpp \
    exception.cpp \
    row.cpp \
    service.cpp \
    statement.cpp \
    time.cpp \
    transaction.cpp \
    user.cpp \
    dll_handler.cpp
RESOURCES += resources.qrc


