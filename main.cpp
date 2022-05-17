#include "MainWindow.hpp"

#include <QApplication>
#include "Controller/GraphController.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //I will set up proper tests later...
    //...probably
    NodeFactory *factory = new NodeFactory();
    factory->addDelegate(0, new AbstractNodeDelegate());
    NodeGraph *graph = new NodeGraph(factory);
    QUndoStack *stack = new QUndoStack();
    QClipboard *clipboard = QGuiApplication::clipboard();
    GraphController *controller = new GraphController(graph, stack, clipboard);
    QUuid uuid = controller->createNode(0);
    QUuid uuid2 = controller->createNode(0);
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 1));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 2));
    /*
    //controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 1));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 2));
    */
    qDebug() << graph->getNode(uuid)->hasConnections();
    controller->selectNode(uuid2);
    controller->deleteSelection();
    qDebug() << graph->getNode(uuid)->hasConnections();
    
    MainWindow w;
    w.show();
    return a.exec();
}
