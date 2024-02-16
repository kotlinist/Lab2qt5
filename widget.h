#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_add_clicked();
    void btnPlayPauseOnClick(QMediaPlayer &mp);

private:
    Ui::Widget *ui;
    QStandardItemModel *m_playListModel;
    QMediaPlayer       *m_player;
    QMediaPlaylist     *m_playlist;
    qint64 trackDuration;
};
#endif // WIDGET_H
