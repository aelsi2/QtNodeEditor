#pragma once

#include <QUndoStack>
#include <QClipboard>
#include <QUuid>
#include <QPointF>
#include <QSet>
#include <QMap>
#include <QVector>
#include <QObject>

#include "Core/NodeGraph.hpp"
#include "UndoCommands.hpp"
#include "Common/HelperTypes.hpp"
#include "Serialization.hpp"
#include "NodeChangeContext.hpp"

struct ConnectionDragAction
{
    enum class Type
    {
        New, ModifyExisting, None
    };
    Type type;
    QUuid existingId;
    static const ConnectionDragAction New;
    static const ConnectionDragAction None;
    static ConnectionDragAction ModifyExisting(QUuid uuid);
};

class GraphEditor : public QObject
{
    Q_OBJECT
public:
    GraphEditor(NodeGraph * nodeGraph, QUndoStack * undoStack, QClipboard * clipboard);
    
    NodeChangeContext beginNodeStateChange(QUuid nodeId) const;
    
    virtual bool getConnectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const;
    virtual ConnectionDragAction getDragAction(QUuid nodeId, PortID portId) const;
    
public slots:
    virtual void selectNode(QUuid nodeId);
    virtual void deselectNode(QUuid nodeId);
    virtual void clearSelection();
    
    virtual void moveSelection(QPointF delta);
    
    virtual QUuid createNode(NodeType type, QPointF position = QPointF(0, 0));
    virtual void deleteSelection();
    
    virtual void cutSelection();
    virtual void copySelection();
    virtual void pasteClipboard();
    virtual void duplicateSelection();
    
    virtual void connect(QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB);
    virtual void disconnect(QUuid connectionId);
    virtual void removeConnections(QVector<QUuid> const & connectionIds);
    
signals:
    void nodeSelected(QUuid nodeId);
    void nodeDeselected(QUuid nodeId);
    void selectionCleared();
    
protected:
    
    void getConnectionsAll(QSet<QUuid> const & nodes, QSet<QUuid> &connections);
    void getConnectionsBetween(QSet<QUuid> const & nodes, QSet<QUuid>  &connections);
    void getConnectionsBetweenAndAll(QSet<QUuid> const & nodes, QSet<QUuid> &connectionsBetween, QSet<QUuid> &additionalConnections);
    
    void beginMacro(QString const & name);
    void endMacro();
    void doCreateNode(Node const * prototype, QUuid uuid);
    void doCreateNode(NodeType type, QPointF position, QUuid uuid);
    void doCreateNode(NodeType type, QPointF position, QJsonValue const & data, QUuid uuid);
    void doCreateConnection(QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB);
    void doDeleteNode(QUuid uuid);
    void doDeleteConnection(QUuid uuid);
    void doMoveNode(QUuid uuid, QPointF newPos, bool relative = true);
    void setClipboard(QString const & text);
    QString getClipboard() const;
    
    NodeGraph * graph;
    QSet<QUuid> selection;
    
private:
    QUndoStack * undoStack;
    QClipboard * clipboard;
};

