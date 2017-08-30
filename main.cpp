#include "node.h"
#include <QApplication>
#include<QGraphicsView>
#include<QGraphicsScene>
#include"diagramwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    diagramwindow view;


    view.show();
    return a.exec();
}
