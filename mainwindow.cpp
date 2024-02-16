#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widget.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowIcon(QIcon("://music.png"));
    setWindowTitle("MiniMusicPlayer");


    Widget *w = new Widget(this);
    setCentralWidget(w);

    setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settings_triggered() {
    SettingsDialog *dialog = new SettingsDialog();
    dialog->show();
}

