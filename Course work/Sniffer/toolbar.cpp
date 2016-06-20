#include "toolbar.h"

ToolBar::ToolBar(QString name, QWidget* parent) : QToolBar(name,parent){
    qDebug() << QApplication::applicationDirPath();
    startIcon = new QIcon(":/Icons/start.png");
    stopIcon = new QIcon(":/Icons/stop.png");
    saveIcon = new QIcon(":/Icons/save.png");

    startAction = new QAction(*startIcon,"Start capture",NULL);
    stopAction = new QAction(*stopIcon,"Stop capture",NULL);
    saveAction = new QAction(*saveIcon,"Save",NULL);

    this->addAction(startAction);
    this->addAction(stopAction);
    this->addSeparator();
    this->addAction(saveAction);
    this->setMovable(false);
}

QAction* ToolBar :: getStartAction(){
    return startAction;
}

QAction* ToolBar :: getStopAction(){
    return stopAction;
}

QAction* ToolBar :: getSaveAction(){
    return saveAction;
}

ToolBar::~ToolBar(){
    delete startIcon;
    delete stopIcon;
    delete saveIcon;
    delete startAction;
    delete stopAction;
    delete saveAction;

    startIcon = NULL;
    stopIcon = NULL;
    saveIcon = NULL;
    startAction = NULL;
    stopAction = NULL;
    saveAction = NULL;
}
