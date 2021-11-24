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

public slots:
    void leiser();
    void lauter();
    void shortcut_zufallslied();
    void shortcut_stummschalten();
    void customcontextmenu(const QPoint& pos);
    void loeschen(int);
    void oeffnen();
    void stopp();
    void wiedergabe();
    void geaenderte_position(int sekunden);
    void neues_lied(qint64 millisekunden);
    void geaenderte_zeit(qint64 progress);
    void lautstaerke_slider(int);
    void stummschalten();
    void vorheriges_lied();
    void naechstes_lied();
    void zufallslied();
    void suche();
    void suche_beenden();
    void suche_starten();
    void sortieren(int);
    void tabelle_dnd(int logic, int alt, int neu);
    void lied_ausgewahlt(int zeile, int spalte);
    void vorspulen();
    void zurueckspulen();

private:
    Ui::Player *ui;

    QMediaPlaylist *playlist = nullptr;
    QMediaPlayer* player = nullptr;
    bool wird_wiedergeben = false;
    QIcon* pause;
    QIcon* wiedergabe_Icon;
    int pos;
    int lautstaerke;

    void daten_laden();
    void hinzufugen_zur_playlist(const QList<QUrl> &urls);
    void lied_hinzufuegen(QUrl);
    void liste_tempplayer_leeren();
    bool ist_treffer(const QString& zeilenInhalt, const QString& suchVorgabe);
    void tabellenansicht();
    Datei_info* lied_erstellen(QString url);
    QJsonObject json_erstellen ();
    void daten_speichern();
    void nur_wiedergabe();

    QList<QMediaPlayer*> liste_tempplayer;
    QMap<QString,Datei_info*> Liedersammlung;
};
#endif // PLAYER_H
