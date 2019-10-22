QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Analyzer/Binarizator/binarizator.cpp \
    Analyzer/Binarizator/binarizatorsettingsform.cpp \
    Analyzer/Binarizator/colorbinarizator.cpp \
    Analyzer/Binarizator/colorselector.cpp \
    Analyzer/Binarizator/normalbinarizator.cpp \
    Analyzer/TimeDomainFilter/staticpointsfilter.cpp \
    Analyzer/TimeDomainFilter/staticpointsfilter/markedpoint.cpp \
    Analyzer/TimeDomainFilter/staticpointsfilter/pointtrace.cpp \
    Analyzer/TimeDomainFilter/staticpointsfilter/staticpointsfilterprivate.cpp \
    Analyzer/TimeDomainFilter/staticpointsfilter/staticpointsfiltersettings.cpp \
    Analyzer/rectsubtractor.cpp \
    Analyzer/redanalyzer.cpp \
    analyzersettingsform.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    Analyzer/Binarizator/binarizator.h \
    Analyzer/Binarizator/binarizatorsettingsform.h \
    Analyzer/Binarizator/binarizatorssettings.h \
    Analyzer/Binarizator/colorbinarizator.h \
    Analyzer/Binarizator/colorselector.h \
    Analyzer/Binarizator/normalbinarizator.h \
    Analyzer/TimeDomainFilter/pointsfilter_common.h \
    Analyzer/TimeDomainFilter/staticpointsfilter.h \
    Analyzer/TimeDomainFilter/staticpointsfilter/markedpoint.h \
    Analyzer/TimeDomainFilter/staticpointsfilter/pointtrace.h \
    Analyzer/TimeDomainFilter/staticpointsfilter/staticpointsfilterprivate.h \
    Analyzer/TimeDomainFilter/staticpointsfilter/staticpointsfiltersettings.h \
    Analyzer/backgroundanalyzer.h \
    Analyzer/rectsubtractor.h \
    Analyzer/redanalyzer.h \
    analyzersettingsform.h \
    mainwindow.h \
    qcustomplot.h

FORMS += \
    Analyzer/Binarizator/binarizatorsettingsform.ui \
    Analyzer/Binarizator/colorselector.ui \
    analyzersettingsform.ui \
    mainwindow.ui


INCLUDEPATH += $${PWD}/Analyzer/Binarizator

#CONFIG (debug, debug|release) {
#    #debug
#    LIBS += $$PWD/libs/opencv_world401d.lib
#    BUILD_FLAG = debug
#} else {
#    #release
#    LIBS += $$PWD/libs/opencv_world401.lib
#    BUILD_FLAG = release
#}

CONFIG (debug, debug|release) {
    #debug
    LIBS += "C:/OpenCV-4.0.1/lib/opencv_world401d.lib"
    BUILD_FLAG = debug
} else {
    #release
    LIBS += "C:/OpenCV-4.0.1/lib/opencv_world401.lib"
    BUILD_FLAG = release
}


include(../../common.pri)
DESTDIR = $${BIN_PATH}/



## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#DISTFILES +=

RESOURCES += \
    resources.qrc

