#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QUuid>

#include "Core/HelperTypes.hpp"

class GraphView : public QGraphicsView
{
public:
    struct DragState {
        enum class Type {
            Idle, NodeMove, RubberBand, Connection, Pan
        };
        Type type;
        QPointF startPos;
    };
    
    GraphView(QGraphicsScene *scene);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    
    void Select(QUuid nodeId);
    void BeginMove();
    void BeginConnection(QUuid nodeId, PortID portId);
    void BeginAddNode(QPointF pos);
    
protected:
    
    DragState dragState;
};

