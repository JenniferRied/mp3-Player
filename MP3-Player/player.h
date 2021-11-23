#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QMap>
#include "datei_info.h"

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
    void lautstaerke_slider(int);
    void stummschalten();
    void naechstes_lied();
    void vorheriges_lied();
    void zufallslied();
    void lied_ausgewahlt(int zeile, int spalte);
    void loeschen(int);
    void customcontextmenu(const QPoint& pos);

private:
    Ui::Player *ui;

    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer* player = nullptr;
    bool wird_wiedergeben = false;
    QIcon* pause;
    QIcon* wiedergabe_Icon;
    int letzte_id;
    int pos;

    void lied_hinzufuegen(QUrl);
    void liste_tempplayer_leeren();
    bool ist_treffer(const QString& zeilenInhalt, const QString& suchVorgabe);
    void tabellenansicht();
    Datei_info* lied_erstellen(QString url);

    QList<QMediaPlayer*> liste_tempplayer;
    QMap<QString,Datei_info*> Liedersammlung;
};
#endif // PLAYER_H
