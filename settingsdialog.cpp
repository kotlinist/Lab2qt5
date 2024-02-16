#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QStyleFactory>
#include <QSettings>
#include <QComboBox>
#include <QFile>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    setWindowIcon(QIcon("://music.png"));
    setWindowTitle("Настройки");

    ui->themeComboBox->addItems(QStringList() << QStyleFactory::keys()<<"MyStyle"<<"Ubuntu"<<"MaterialDark"<<"Aqua");
    connect(ui->themeComboBox, &QComboBox::currentTextChanged, [](QString theme) {
        qApp->setStyleSheet("");
        if(QStyleFactory::keys().contains(theme)) {
            QApplication::setStyle(QStyleFactory::create(theme));
        } else {
            QFile stylefile(":/themes/"+theme+".qss");
            if (stylefile.open(QFile::ReadOnly)) {
                QString strCSS=QLatin1String(stylefile.readAll());
                qApp->setStyleSheet(strCSS);
                stylefile.close();
            }
        }
    });
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::on_pushButton_2_clicked() {
    QApplication::setStyle(QStyleFactory::create("Fusion"));
}


void SettingsDialog::on_pushButton_3_clicked() {
    SettingsDialog::close();
}

