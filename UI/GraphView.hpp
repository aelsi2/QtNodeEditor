#pragma once

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>
#include <QUuid>
#include <QMap>

class GraphScene;
class NodeGraphicsItem;
class ConnectionGraphicsItem;

#include "GraphScene.hpp"
#include "Common/HelperTypes.hpp"
#include "Controller/GraphEditor.hpp"
#include "ConnectionGraphicsItem.hpp"
#include "NodeGraphicsItem.hpp"
#include "AddNodeWindow.hpp"

class GraphView : public QGraphicsView
{
public:
    GraphView(GraphScene *scene, GraphEditor *editor, QMap<QString, NodeType> const & nodes);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    
    void select(QUuid nodeId);
    void addToSelection(QUuid nodeId);
    void deselect(QUuid nodeId);
    void clearSelection();
    
    void beginMove(QPointF pos);
    void beginConnection(NodeGraphicsItem *node, PortID portId);
    
    void setActions(
        QAction *deleteSelectionAction,
        QAction *copySelectionAction,
        QAction *cutSelectionAction,
        QAction *pasteSelectionAction);
    
public slots:
    void showAddNodeWindow();
    
protected:
    
    struct DragState {
        enum class Type {
            Idle, NodeMove, RubberBand, Connection, Pan
        } type;
        
        QPointF startScenePos;
        
        bool suspendConnection;
        ConnectionGraphicsItem *suspendedConnection;
        NodeGraphicsItem *connectionNodeA;
        PortID connectionPortA;
        
        void unsuspendConnection();
        
    } dragState; //C go brrrrr
    
    void doNodeMove(QMouseEvent *event);
    void doConnectionDrop(QPointF viewMousePos);
    
    void resetStateIfNothingPressed(QMouseEvent *event);
    
    GraphEditor *editor;
    
    AddNodeWindow *addNodeWindow;
    GraphScene *graphScene;
    QMap<QGraphicsItem*, QPointF> initialNodePositions;
};
