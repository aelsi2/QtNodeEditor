#include "MainWindow.hpp"

#include <QApplication>
#include "Controller/GraphController.hpp"

void NodeCreated(NodeType type, QUuid uuid, Node *node, QPointF position)
{
    qDebug() << "node created:" << uuid.toString() << "type:" << type  << "pos:" << position << "\n";
}
void ConnectionMade(QUuid uuid, Connection *connection,
    QUuid firstNode, QUuid secondNode,
    PortID firstPort, PortID secondPort)
{
    qDebug() << "connection made:" << uuid << "\nA:" << firstNode.toString() << "B:" << secondNode.toString() << "\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //I will set up proper tests later...
    //...probably
    NodeFactory *factory = new NodeFactory();
    factory->addDelegate(0, new AbstractNodeDelegate());
    
    NodeGraph *graph = new NodeGraph(factory);
    QObject::connect(graph, &NodeGraph::nodeCreated, &NodeCreated);
    QObject::connect(graph, &NodeGraph::connectionMade, &ConnectionMade);
    
    QUndoStack *stack = new QUndoStack();
    GraphController *controller = new GraphController(graph, stack, QGuiApplication::clipboard());
    
    QUuid uuid = controller->createNode(0);
    QUuid uuid2 = controller->createNode(0, QPointF(5, 4.5));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 1));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 2));

    controller->selectNode(uuid);
    controller->selectNode(uuid2);
    controller->cutSelectionToClipboard();
    controller->pasteClipboard();
    controller->pasteClipboard();
    controller->deleteSelection();
    
    MainWindow w;
    w.show();
    return a.exec();
}
