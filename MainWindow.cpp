#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addItem(new NodeGraphicsItem(nullptr, nullptr, QUuid::createUuid()));
    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{
}

