#include <QApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QToolBar>
#include <QAction>
#include <QFont>

#include "GUIHandler.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    GUIHandler gui;
    gui.start();
    
    return app.exec();
}
