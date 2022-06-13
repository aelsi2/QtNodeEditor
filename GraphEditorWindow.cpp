#include "GraphEditorWindow.hpp"

GraphEditorWindow::GraphEditorWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMap<QString, NodeType> nodeNames{{"Node", 0}};
    nodeFactory = new NodeFactory();
    graphicsFactory = new GraphicsFactory();
    nodeFactory->addDelegate(0,
                             [](NodeType type, NodeGraph *graph, QPointF position, QUuid uuid)
                             {
                                 return new Node(graph, type, position, uuid);
                             });
    graphicsFactory->addNodeDelegate(0,
                            [](NodeType type, QPointF position, Node *node, QUuid uuid)
                            {
                                auto nodeItem = new NodeGraphicsItem(node, uuid);
                                nodeItem->setPos(position);
                                return nodeItem;
                            });
    
    graph = new NodeGraph(nodeFactory);
    editor = new GraphEditor(graph, &undoStack, QGuiApplication::clipboard());
    graphScene = new GraphScene(graphicsFactory, graph, editor);
    graphView = new GraphView(graphScene, editor, nodeNames);
    setCentralWidget(graphView);
    createActions();
    createMenus();
}

void GraphEditorWindow::createActions()
{
    undoAction = new QAction("Undo", this);
    undoAction->setShortcuts(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, &undoStack, &QUndoStack::undo);
    redoAction = new QAction("Redo", this);
    redoAction->setShortcuts(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, &undoStack, &QUndoStack::redo);
    copySelectionAction = new QAction("Copy", this);
    copySelectionAction->setShortcuts(QKeySequence::Copy);
    connect(copySelectionAction, &QAction::triggered, editor, &GraphEditor::copySelection);
    cutSelectionAction = new QAction("Cut", this);
    cutSelectionAction->setShortcuts(QKeySequence::Cut);
    connect(cutSelectionAction, &QAction::triggered, editor, &GraphEditor::cutSelection);
    pasteClipboardAction = new QAction("Paste", this);
    pasteClipboardAction->setShortcuts(QKeySequence::Paste);
    connect(pasteClipboardAction, &QAction::triggered, editor, &GraphEditor::pasteClipboard);
    deleteSelectionAction = new QAction("Delete", this);
    deleteSelectionAction->setShortcuts(QKeySequence::Delete);
    connect(deleteSelectionAction, &QAction::triggered, editor, &GraphEditor::deleteSelection);
    addNodeAction = new QAction("Add Node", this);
    addNodeAction->setShortcuts(QKeySequence::SelectAll);
    connect(addNodeAction, &QAction::triggered, graphView, &GraphView::showAddNodeWindow);
}

void GraphEditorWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    editMenu = menuBar()->addMenu("Edit");
    nodeMenu = menuBar()->addMenu("Node");
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(copySelectionAction);
    editMenu->addAction(cutSelectionAction);
    editMenu->addAction(pasteClipboardAction);
    editMenu->addSeparator();
    editMenu->addAction(deleteSelectionAction);
    nodeMenu->addAction(addNodeAction);
    
}
