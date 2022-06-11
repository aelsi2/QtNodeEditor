#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QUuid>
#include "NodeGraphicsItem.hpp"

#include "Core/HelperTypes.hpp"

class GraphView : public QGraphicsView
{
    Q_OBJECT
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
    void BeginMove();
    void BeginConnection(QUuid nodeId, PortID portId);
    void BeginAddNode(QPointF pos);
    
private slots:
    void syncSelection();
     
protected:
    NodeCollection selection;
    DragState dragState;
};

