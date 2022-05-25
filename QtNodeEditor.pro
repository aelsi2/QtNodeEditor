QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/GraphEditor.cpp \
    Controller/Serialization.cpp \
    Controller/UndoCommands.cpp \
    Core/Connection.cpp \
    Core/HelperTypes.cpp \
    Core/Node.cpp \
    Core/NodeFactory.cpp \
    Utils/Port.cpp \
    Utils/PortPolicy.cpp \
    UI/ConnectionGraphicsItem.cpp \
    UI/NodeGraphScene.cpp \
    UI/NodeGraphicsItem.cpp \
    UI/PortGraphics.cpp \
    main.cpp \
    MainWindow.cpp \
    Core/NodeGraph.cpp

HEADERS += \
    Controller/GraphEditor.hpp \
    Controller/Serialization.hpp \
    Controller/UndoCommands.hpp \
    Core/Connection.hpp \
    Core/HelperTypes.hpp \
    Core/Node.hpp \
    Core/NodeFactory.hpp \
    Utils/Port.hpp \
    Utils/PortPolicy.hpp \
    MainWindow.hpp \
    Core/NodeGraph.hpp \
    UI/ConnectionGraphicsItem.hpp \
    UI/NodeGraphScene.hpp \
    UI/NodeGraphicsItem.hpp \
    UI/PortGraphics.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
