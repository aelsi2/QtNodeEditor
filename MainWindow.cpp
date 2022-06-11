#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addItem(new NodeGraphicsItem(nullptr, nullptr, QUuid::createUuid()));
    GraphView *view = new GraphView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}

