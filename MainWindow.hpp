#pragma once

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "UI/NodeGraphicsItem.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
