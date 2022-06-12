#include "GraphEditorWindow.hpp"

#include <QMap>
#include <QApplication>
#include "Controller/GraphEditor.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphEditorWindow w;
    w.show();
    return a.exec();
}
