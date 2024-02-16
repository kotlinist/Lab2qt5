#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDir>
#include <QTime>

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    m_playListModel = new QStandardItemModel(this);
    ui->playlistView->setModel(m_playListModel);
    // Устанавливаем заголовки таблицы
    m_playListModel->setHorizontalHeaderLabels(QStringList() << tr("Audio Track") << tr("File Path"));

    ui->playlistView->hideColumn(1);    // Скрываем колонку, в которой хранится путь к файлу
    ui->playlistView->verticalHeader()->setVisible(false);                 // Скрываем нумерацию строк
    ui->playlistView->horizontalHeader()->setVisible(false);
    ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows); // Включаем выделение строк
    ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Отключаем редактирование
    // Включаем подгонку размера последней видимой колонки к ширине TableView
    ui->playlistView->horizontalHeader()->setStretchLastSection(true);
    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);
    m_player->setVolume(100);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    trackDuration = 0;

    connect(ui->btnPrevious, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);
    connect(ui->btnNext, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(ui->btnPlayPause, &QPushButton::clicked, [this]() {
        btnPlayPauseOnClick(*m_player);
    });
    connect(ui->btnRemove, &QPushButton::clicked, [this]() {
        if(m_playListModel->rowCount()<=0 || ui->playlistView->selectionModel()->selectedRows().count() <= 0) return ;

        int row = ui->playlistView->selectionModel()->selectedRows().first().row();
        m_playlist->removeMedia(row);
        m_playListModel->removeRow(row);
        ui->currentTrack->setText("");
    });
    connect(ui->btnClearPlaylist, &QPushButton::clicked, [this]() {
        m_playlist->clear();
        m_playListModel->removeRows(0, m_playListModel->rowCount());
        ui->currentTrack->setText("");
    });

    connect(ui->playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        m_playlist->setCurrentIndex(index.row());
        m_player->play();
        ui->btnPlayPause->setIcon(*(new QIcon(":/buttons/pause.png")));
        ui->progressBar_2->setValue(0);
    });

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index) {
        ui->currentTrack->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString().replace(".mp3", ""));
        ui->duration->setText(QTime::fromMSecsSinceStartOfDay(m_player->duration()).toString());
    });

    connect(m_player, &QMediaPlayer::positionChanged, [this](qint64 pos) {
        ui->currentTime->setText(QTime::fromMSecsSinceStartOfDay(m_player->position()).toString());
        ui->progressBar_2->setValue(pos);
    });

    connect(m_player, &QMediaPlayer::durationChanged, [this](int duration) {
        trackDuration = duration;
        ui->duration->setText(QTime::fromMSecsSinceStartOfDay(duration).toString());
        ui->progressBar_2->setRange(0, duration);
    });

    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(on_btn_add_clicked()));
}

Widget::~Widget() {
    delete ui;
    delete m_playListModel;
    delete m_playlist;
    delete m_player;
}

void Widget::btnPlayPauseOnClick(QMediaPlayer &mp) {
    if(mp.state() == QMediaPlayer::State::PlayingState) {
        mp.pause();
        ui->btnPlayPause->setIcon(*(new QIcon(":/buttons/play.png")));
    } else {
        mp.play();
        ui->btnPlayPause->setIcon(*(new QIcon(":/buttons/pause.png")));
    }
}

void Widget::on_btn_add_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"),
                                                      QString(), tr("Audio Files (*.mp3)"));
    // Далее устанавливаем данные по именам и пути к файлам
    // в плейлист и таблицу отображающую плейлист
    foreach (QString filePath, files) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName().replace(".mp3", "")));
        items.append(new QStandardItem(filePath));
        m_playListModel->appendRow(items);
        m_playlist->addMedia(QUrl(filePath));
    }
}
