#include "MainWindow.hpp"

#include <QApplication>
#include "Controller/GraphEditor.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    MainWindow w;
    w.show();
    return a.exec();
}
