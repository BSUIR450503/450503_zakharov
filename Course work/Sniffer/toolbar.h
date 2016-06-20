#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QIcon>
#include <QString>
#include <QCoreApplication>
#include <QApplication>
#include <QDebug>

class ToolBar : public QToolBar
{
        Q_OBJECT
    public:
        ToolBar(QString,QWidget* parent = NULL);
        QAction* getStartAction();
        QAction* getStopAction();
        QAction* getSaveAction();
        ~ToolBar();
    private:
        QAction* startAction;
        QAction* stopAction;
        QAction* saveAction;
        QIcon* startIcon;
        QIcon* stopIcon;
        QIcon* saveIcon;
};

#endif // TOOLBAR_H
