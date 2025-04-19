#include "mainWindow.h"

#include "qpushbutton.h"
#include "qtextedit.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Init Windows components */
    setWindowTitle("Text-To-Speech Application");
}

mainWindow::~mainWindow()
{}
