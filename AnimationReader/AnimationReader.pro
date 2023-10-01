QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AnimDefinitions/C_StateNode.cpp \
    Encoder/C_DefinitionEncoder.cpp \
    Encoder/hash.cpp \
    Interface/C_DefInterface.cpp \
    Interface/C_TableBehavior.cpp \
    main.cpp \
    mainwindow.cpp \
    nodedialog.cpp

HEADERS += \
    AnimDefinitions/AnimDefHandler.h \
    AnimDefinitions/BinaryIO.h \
    AnimDefinitions/C_AnimNodes.h \
    AnimDefinitions/C_DefStream.h \
    AnimDefinitions/C_DefinitionEncoder.h \
    AnimDefinitions/C_StateNode.h \
    ColorItemDelegate.h \
    DefsTableUtils.h \
    DefsTreeWidgetItem.h \
    Encoder/C_DefinitionEncoder.h \
    Encoder/JsonHandler.h \
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

RESOURCES += \
    Resources.qrc
