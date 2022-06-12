QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/GraphEditor.cpp \
    Controller/NodeChangeContext.cpp \
    Controller/Serialization.cpp \
    Controller/UndoCommands.cpp \
    Core/Connection.cpp \
    Common/HelperTypes.cpp \
    Core/Node.cpp \
    Core/NodeFactory.cpp \
    GraphEditorWindow.cpp \
    UI/AddNodeWindow.cpp \
    UI/PortGraphics.cpp \
    UI/CirclePort.cpp \
    UI/GraphScene.cpp \
    UI/GraphView.cpp \
    UI/GraphicsFactory.cpp \
    Utils/Port.cpp \
    Utils/PortPolicy.cpp \
    UI/ConnectionGraphicsItem.cpp \
    UI/NodeGraphicsItem.cpp \
    main.cpp \
    Core/NodeGraph.cpp

HEADERS += \
    Controller/GraphEditor.hpp \
    Controller/NodeChangeContext.hpp \
    Controller/Serialization.hpp \
    Controller/UndoCommands.hpp \
    Core/Connection.hpp \
    Common/HelperTypes.hpp \
    Core/Node.hpp \
    Core/NodeFactory.hpp \
    GraphEditorWindow.hpp \
    UI/AddNodeWindow.hpp \
    UI/CirclePort.hpp \
    UI/GraphScene.hpp \
    UI/GraphView.hpp \
    UI/GraphicsFactory.hpp \
    Utils/Port.hpp \
    Utils/PortPolicy.hpp \
    Core/NodeGraph.hpp \
    UI/ConnectionGraphicsItem.hpp \
    UI/NodeGraphicsItem.hpp \
    UI/PortGraphics.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
