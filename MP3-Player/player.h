#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMediaPlaylist>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();

    void addToPlaylist(const QList<QUrl> &urls);

public slots:
    void wiedergabe();
    void stopp();
    void suche();
    void suche_starten();
    void suche_beenden();
    void oeffnen();

private:
    Ui::Player *ui;
    QList<QTableWidgetItem> kein_treffer;

    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer* player = nullptr;
    bool wird_wiedergeben = false;
    QIcon* pause;
    QIcon* wiedergabe_Icon;
};
#endif // PLAYER_H
