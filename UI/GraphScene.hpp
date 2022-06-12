#pragma once

#include <QObject>
#include <QGraphicsScene>
#include <QUuid>
#include <QMap>

class GraphicsFactory;
class NodeGraphicsItem;
class ConnectionGraphicsItem;

#include "Common/HelperTypes.hpp"
#include "Core/Connection.hpp"
#include "Core/Node.hpp"
#include "Controller/GraphEditor.hpp"
#include "NodeGraphicsItem.hpp"
#include "GraphicsFactory.hpp"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphScene(GraphicsFactory *factory, NodeGraph *graph, GraphEditor *editor);
    
    QMap<QUuid, NodeGraphicsItem*> const & getGraphSelection() const;
    NodeGraphicsItem* getNodeItem(QUuid nodeId) const;
    ConnectionGraphicsItem* getConnectionItem(QUuid connectionId) const;
    
public slots:
    void nodeMoved(QUuid nodeId, QPointF position);
    void nodeCreated(NodeType type, QUuid uuid, Node *node, QPointF position);
    void nodeDeleted(QUuid uuid);
    void connectionMade(QUuid uuid, Connection *connection,
                        QUuid nodeIdA, QUuid nodeIdB,
                        PortID portIdA, PortID portIdB);
    void connectionRemoved(QUuid uuid);
    void nodeSelected(QUuid uuid);
    void nodeDeselected(QUuid uuid);
    void selectionCleared();
    
protected:
    GraphicsFactory *factory;
    QMap<QUuid, NodeGraphicsItem*> nodes;
    QMap<QUuid, ConnectionGraphicsItem*> connections;
    QMap<QUuid, NodeGraphicsItem*> selection;
};

