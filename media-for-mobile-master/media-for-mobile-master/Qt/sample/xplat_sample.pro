TARGET = Xplat-Sample
TEMPLATE = app

QT += qml quick widgets gui_private

SOURCES += main.cpp \
    mptranscoder.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mptranscoder.h \
    picker.h

#############################################################
## Path to Media for Mobile C++ wrapper
##
## Paths to wrapper headers and library
##
MFM_WRAPPER_PATH = $$PWD/../wrapper

#############################################################
## Path to Media for Mobile package for current platform
##
MFM_PATH = /path/to/media_for_mobile

ios {
    INCLUDEPATH += $$MFM_WRAPPER_PATH/ios
    DEPENDPATH += $$MFM_WRAPPER_PATH/ios

    MFM_WRAPPER_LIB = $$MFM_WRAPPER_PATH/ios/_build/MediaPackC++/Build/Products/Debug-iphoneos

    LIBS += -L$$MFM_WRAPPER_LIB -lMediaPackC++ \
            -F$$MFM_PATH -framework MediaForMobile \
            -framework CoreMedia -framework CoreVideo -framework AVFoundation -framework AssetsLibrary

    OBJECTIVE_SOURCES += \
        ./ios/picker.mm \
        ./ios/FilePickerController.m

    OBJECTIVE_HEADERS += \
        ./ios/FilePickerController.h

    APP_NIB_FILES.files = ios/Cell.nib ios/FilePicker.nib
    APP_NIB_FILES.path = ./.
    QMAKE_BUNDLE_DATA += APP_NIB_FILES
}

android {
    QT += androidextras
    INCLUDEPATH += $$MFM_WRAPPER_PATH/android
    DEPENDPATH += $$MFM_WRAPPER_PATH/android

    SOURCES += onLoad.cpp \
               ./android/picker.cpp

    equals(ANDROID_ARCHITECTURE, x86) {
        EXTRA_LIBS_PATH = $$MFM_PATH/libs/x86
    } else {
        EXTRA_LIBS_PATH = $$MFM_PATH/libs/armeabi
    }

    ANDROID_EXTRA_LIBS += \
        $$EXTRA_LIBS_PATH/libippresample.so

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android/package_source_dir

    OTHER_FILES += \
        android/AndroidManifest.xml \
        android/src/org/qtproject/qt5/android/bindings/FilePicker.java
}
