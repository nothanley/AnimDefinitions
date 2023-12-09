QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/Core/include
INCLUDEPATH += $$PWD/Core/src

SOURCES += \
    Core/include/compression/zcompress.cpp \
    Core/include/compression/zlib/adler32.c \
    Core/include/compression/zlib/compress.c \
    Core/include/compression/zlib/crc32.c \
    Core/include/compression/zlib/deflate.c \
    Core/include/compression/zlib/gzclose.c \
    Core/include/compression/zlib/gzlib.c \
    Core/include/compression/zlib/gzread.c \
    Core/include/compression/zlib/gzwrite.c \
    Core/include/compression/zlib/infback.c \
    Core/include/compression/zlib/inffast.c \
    Core/include/compression/zlib/inflate.c \
    Core/include/compression/zlib/inftrees.c \
    Core/include/compression/zlib/trees.c \
    Core/include/compression/zlib/uncompr.c \
    Core/include/compression/zlib/zutil.c \
    Core/src/BinaryIO.cpp \
    Core/src/Definition.cpp \
    Core/src/DefsEncoder.cpp \
    Core/src/DefsReader.cpp \
    Core/src/StateNode.cpp \
    Core/src/main.cpp \
    Encoder/dtmpserializer.cpp \
    Interface/C_DefInterface.cpp \
    Encoder/hash.cpp \
    Interface/C_TableBehavior.cpp \
    Widget/DefsTableUtils.cpp \
    main.cpp \
    mainwindow.cpp \
    nodedialog.cpp

HEADERS += \
    Core/include/compression/zcompress.h \
    Core/include/compression/zlib/crc32.h \
    Core/include/compression/zlib/deflate.h \
    Core/include/compression/zlib/gzguts.h \
    Core/include/compression/zlib/inffast.h \
    Core/include/compression/zlib/inffixed.h \
    Core/include/compression/zlib/inflate.h \
    Core/include/compression/zlib/inftrees.h \
    Core/include/compression/zlib/trees.h \
    Core/include/compression/zlib/zconf.h \
    Core/include/compression/zlib/zlib.h \
    Core/include/compression/zlib/zutil.h \
    Core/src/AnimNode.h \
    Core/src/BinaryIO.h \
    Core/src/Definition.h \
    Core/src/DefsEncoder.h \
    Core/src/DefsReader.h \
    Core/src/StateNode.h \
    DefsTableUtils.h \
    DefsTreeWidgetItem.h \
    Encoder/JsonHandler.h \
    Encoder/C_DefinitionEncoder.h \
    Encoder/dtmpserializer.h \
    Encoder/hash.h \
    Hash/hash.h \
    Interface/C_DefInterface.h \
    Interface/C_SyncedTableWidgetItem.h \
    Interface/C_TableBehavior.h \
    Interface/ColorItemDelegate.h \
    Interface/DefsTreeWidgetItem.h \
    Interface/LabelTreeItemDelegate.h \
    Interface/TypeHandler.h \
    LabelTreeItemDelegate.h \
    Widget/C_SyncedTableWidgetItem.h \
    Widget/ColorItemDelegate.h \
    Widget/DefsTableUtils.h \
    Widget/DefsTreeWidgetItem.h \
    Widget/LabelTreeItemDelegate.h \
    mainwindow.h \
    nodedialog.h

FORMS += \
    mainwindow.ui \
    nodedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:RC_ICONS += graphics/editor-icon.ico

RESOURCES += \
    Resources.qrc

