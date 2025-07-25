QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    resources/images.qrc

# WASM-specific build options
wasm {
    QMAKE_CXXFLAGS_RELEASE += -Os
    QMAKE_LFLAGS_RELEASE += --llvm-lto 3 -s ALLOW_MEMORY_GROWTH=1 -Os

    QMAKE_LFLAGS += \
        -sASSERTIONS=0 \
        -sSAFE_HEAP=0 \
        -sEXPORTED_RUNTIME_METHODS='["UTF16ToString","stringToUTF16","JSEvents","specialHTMLTargets","FS","callMain"]' \
        -sEXPORT_NAME=DealAnalyzer_entry \
        -sALLOW_MEMORY_GROWTH=1 \
        -sINITIAL_MEMORY=50MB \
        -sMAXIMUM_MEMORY=4GB
}


