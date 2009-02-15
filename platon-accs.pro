######################################################################
# Automatically generated by qmake (2.01a) ?? ????. 15 11:48:53 2009
######################################################################
config += qt debug
win32: DEFINES += IBPP_WINDOWS
unix: DEFINES += IBPP_LINUX
unix: LIBS += -lfbclient
DEFINES -= UNICODE

TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              debug \
              EssClasses \
              EssForms \
              EssWidgets \
              images \
              EssWidgets/hugo_model \
              ibpp/core
INCLUDEPATH += . EssForms ibpp/core EssWidgets EssClasses EssWidgets/hugo_model

# Input
HEADERS += DataClass.h \
           EssentialClass.h \
           mainWin.h \
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
           EssForms/abstracthip_editform.h \
           EssForms/DialogchoiceEidos.h \
           EssForms/DialogchoiceHypotesis.h \
           EssForms/DialogchoicePragma.h \
           EssForms/DialogEditHronologyEA.h \
           EssForms/DialogLogin.h \
           EssForms/HypotesisEditForm.h \
           EssForms/LinksExplorer.h \
           EssForms/PragmaEditForm.h \
           EssWidgets/AbstractMemHypModel.h \
           EssWidgets/commonroutines.h \
           EssWidgets/EAOneFrame.h \
           EssWidgets/EidosMemModel.h \
           EssWidgets/HronologyEaMemModel.h \
           EssWidgets/HypotesisMemModel.h \
           EssWidgets/HypPragmaMemModel.h \
           EssWidgets/LnkComboBox.h \
           EssWidgets/LnkdEidosMemModel.h \
           EssWidgets/LnkdHypMemModel.h \
           EssWidgets/LnkdPragmaMemModel.h \
           EssWidgets/PragmaMemModel.h \
           EssWidgets/qeidostreewidget.h \
           EssWidgets/hugo_model/hypotesismodel.h \
           EssWidgets/hugo_model/pragmamodel.h \
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
FORMS += mainwindow.ui EssForms/HypotesisEditForm.ui EssForms/LinksExplorer.ui
SOURCES += DataClass.cpp \
           main.cpp \
           mainWin.cpp \
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
           EssForms/abstracthip_editform.cpp \
           EssForms/DialogchoiceEidos.cpp \
           EssForms/DialogchoiceHypotesis.cpp \
           EssForms/DialogchoicePragma.cpp \
           EssForms/DialogEditHronologyEA.cpp \
           EssForms/DialogLogin.cpp \
           EssForms/HypotesisEditForm.cpp \
           EssForms/LinksExplorer.cpp \
           EssForms/PragmaEditForm.cpp \
           EssWidgets/AbstractMemHypModel.cpp \
           EssWidgets/commonroutines.cpp \
           EssWidgets/EAOneFrame.cpp \
           EssWidgets/EidosMemModel.cpp \
           EssWidgets/HronologyEaMemModel.cpp \
           EssWidgets/HypotesisMemModel.cpp \
           EssWidgets/HypPragmaMemModel.cpp \
           EssWidgets/LnkComboBox.cpp \
           EssWidgets/LnkdEidosMemModel.cpp \
           EssWidgets/LnkdHypMemModel.cpp \
           EssWidgets/LnkdPragmaMemModel.cpp \
           EssWidgets/PragmaMemModel.cpp \
           EssWidgets/qeidostreewidget.cpp \
           EssWidgets/hugo_model/hypotesismodel.cpp \
           EssWidgets/hugo_model/pragmamodel.cpp \
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
RESOURCES += images/resources.qrc
