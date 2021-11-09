#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QTableWidgetItem>

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

    void suche();
    void suche_starten();
    void suche_beenden();

private:
    Ui::Player *ui;
    QList<QTableWidgetItem> kein_treffer;

};
#endif // PLAYER_H
