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

    void hinzufugen_zur_playlist(const QList<QUrl> &urls);

public slots:
    void wiedergabe();
    void stopp();
    void suche();
    void suche_starten();
    void suche_beenden();
    void oeffnen();
    void geaenderte_position(int sekunden);
    void neues_lied(qint64 millisekunden);
    void geaenderte_zeit(qint64 progress);

private:
    Ui::Player *ui;
    QList<QTableWidgetItem> kein_treffer;

    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer* player = nullptr;
    bool wird_wiedergeben = false;
    QIcon* pause;
    QIcon* wiedergabe_Icon;

    bool ist_treffer(const QString& zeilenInhalt, const QString& suchVorgabe);
};
#endif // PLAYER_H
