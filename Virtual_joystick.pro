QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++2a

SOURCES += \
  VJoyConnector.cpp \
  infobox.cpp \
    main.cpp \
    mainwindow.cpp \
    scanbox.cpp \
  vjdevice.cpp

HEADERS += \
  Structs.h \
  VJoyConnector.h \
  infobox.h \
   mainwindow.h \
   scanbox.h \
	  public.h \
  vjdevice.h \
		vjoyinterface.h
FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/Libs
DEPENDPATH += $$PWD/Libs

DISTFILES += \
  lib/amd64/vJoyInterface.dll \
  lib/amd64/vJoyInterface.lib

win32: LIBS += -L$$PWD/lib/amd64/ -lvJoyInterface -lUser32

INCLUDEPATH += $$PWD/lib/amd64
DEPENDPATH += $$PWD/lib/amd64
