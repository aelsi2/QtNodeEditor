QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/GraphController.cpp \
    Controller/Serialization.cpp \
    Core/ConnectAction.cpp \
    Core/Connection.cpp \
    Core/Node.cpp \
    Core/NodeFactory.cpp \
    Controller/NodeGraphUndoCommands.cpp \
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
    Controller/GraphController.hpp \
    Controller/Serialization.hpp \
    Core/ConnectAction.hpp \
    Core/Connection.hpp \
    Core/Node.hpp \
    Core/NodeFactory.hpp \
    Controller/NodeGraphUndoCommands.hpp \
    Core/NodeType.hpp \
    Core/PortUtils.hpp \
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
