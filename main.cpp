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
    GraphEditor *editor = new GraphEditor(graph, stack, QGuiApplication::clipboard());
    
    QUuid uuid = editor->createNode(0);
    QUuid uuid2 = editor->createNode(0, QPointF(5, 4.5));
    QUuid uuid3 = editor->createNode(0, QPointF(5, 6.3));
    editor->connect(uuid, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));
    editor->connect(uuid3, PortID(PortDirection::IN, 0), uuid2, PortID(PortDirection::OUT, 0));

    editor->selectNode(uuid);
    editor->selectNode(uuid2);
    editor->selectNode(uuid3);
    editor->cutSelection();
    editor->pasteClipboard();
    stack->undo();
    stack->undo();
    
    MainWindow w;
    w.show();
    return a.exec();
}
