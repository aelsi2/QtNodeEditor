#pragma once

#include <QGuiApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QAction>
#include <QMenuBar>
#include <QMenu>

#include "Controller/GraphEditor.hpp"
#include "UI/GraphView.hpp"
#include "UI/GraphScene.hpp"
#include "Core/NodeGraph.hpp"

class GraphEditorWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    GraphEditorWindow(QWidget *parent = nullptr);
    
private:
    void createActions();
    void createMenus();
    
    QAction *undoAction;
    QAction *redoAction;
    QAction *copySelectionAction;
    QAction *cutSelectionAction;
    QAction *pasteClipboardAction;
    //QAction *duplicateAction;
    QAction *deleteSelectionAction;
    QAction *addNodeAction;
    
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *nodeMenu;
    
    QUndoStack undoStack;
    
    GraphView *graphView;
    
    GraphicsFactory *graphicsFactory;
    GraphScene *graphScene;
    
    GraphEditor *editor;
    NodeFactory *nodeFactory;
    NodeGraph *graph;
};
