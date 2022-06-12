#include "AddNodeWindow.hpp"

AddNodeWindow::AddNodeWindow(GraphEditor *grapheditor, QMap<QString, NodeType> const &nodes) : editor(grapheditor)
{
    setWindowFlag(Qt::Popup);
    nodeList = new QListWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(nodeList);
    connect(nodeList, &QListWidget::itemClicked, this, &AddNodeWindow::itemClicked);

    makeItems(nodes);
    setLayout(layout);
}

void AddNodeWindow::setPosition(QPoint mousePos, QPointF scenePos)
{
    move(mousePos);
    this->scenePosition = scenePos;
}

void AddNodeWindow::makeItems(QMap<QString, NodeType> const &nodes)
{
    for (auto i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        auto item = new QListWidgetItem(i.key(), nodeList);
        item->setData(Qt::UserRole, QVariant(i.value()));
    }
}

void AddNodeWindow::itemClicked(QListWidgetItem *item)
{
    close();
    NodeType type = item->data(Qt::UserRole).toInt();
    editor->createNode(type, scenePosition);
}
