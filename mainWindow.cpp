#include "mainWindow.h"

/* Displays */
#include "qlayout.h"

/* Components */
#include "qpushbutton.h"
#include "qtextedit.h"

/* Media */
#include "qmediaplayer.h"
#include "qaudioinput.h"
#include "qaudiooutput.h"
#include "qvideowidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Init Windows components */
    setWindowTitle("Text-To-Speech Application");
}

MainWindow::~MainWindow()
{}
