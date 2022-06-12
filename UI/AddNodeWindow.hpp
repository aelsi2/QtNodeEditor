#pragma once

#include <Common/HelperTypes.hpp>
#include <QMap>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QPointF>
#include "Controller/GraphEditor.hpp"

class AddNodeWindow : public QWidget
{
public:
    AddNodeWindow(GraphEditor *grapheditor, QMap<QString, NodeType> const &nodes);
    void setPosition(QPoint mousePos, QPointF scenePos);
private:
    QPointF scenePosition;
    GraphEditor *editor;
    QListWidget *nodeList;
    void makeItems(QMap<QString, NodeType> const &nodes);
private slots:
    void itemClicked(QListWidgetItem *item);
};

