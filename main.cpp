#include "MainWindow.hpp"

#include <QApplication>
#include "Controller/GraphEditor.hpp"

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

void nodeDeleted(QUuid uuid)
{
    qDebug() << "node deleted:" << uuid;
}

void connectionRemoved(QUuid uuid)
{
    qDebug() << "connection removed" << uuid;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    NodeFactory *factory = new NodeFactory();
    factory->addDelegate(0, new AbstractNodeDelegate());
    
    NodeGraph *graph = new NodeGraph(factory);
    QObject::connect(graph, &NodeGraph::nodeCreated, &NodeCreated);
    QObject::connect(graph, &NodeGraph::connectionMade, &ConnectionMade);
    QObject::connect(graph, &NodeGraph::nodeDeleted, &nodeDeleted);
    QObject::connect(graph, &NodeGraph::connectionRemoved, &connectionRemoved);
    
    QUndoStack *stack = new QUndoStack();
    GraphEditor *controller = new GraphEditor(graph, stack, QGuiApplication::clipboard());
    
    QUuid uuid = controller->createNode(0);
    QUuid uuid2 = controller->createNode(0, QPointF(5, 4.5));
    controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));
    //controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 1));
    //controller->performConnectAction(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 2));

    controller->selectNode(uuid);
    controller->selectNode(uuid2);
    controller->cutSelection();
    controller->pasteClipboard();
    stack->undo();
    stack->undo();
    
    MainWindow w;
    w.show();
    return a.exec();
}
