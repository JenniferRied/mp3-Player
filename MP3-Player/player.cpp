#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);

    //https://www.flaticon.com/de/kostenloses-icon/wiedergabetaste_149668?term=play&page=1&position=2&page=1&position=2&related_id=149668&origin=search
    QIcon* wiedergabe = new QIcon(":/Bilder/wiedergabetaste.png");
    //https://www.flaticon.com/premium-icon/backward_2938636?term=backward&page=1&position=40&page=1&position=40&related_id=2938636&origin=search
    QIcon* zurueckspulen = new QIcon(":/Bilder/backward.png");
    QIcon* vorspulen = new QIcon(":/Bilder/forward.png");
    //https://www.flaticon.com/search?word=random&order_by=4&type=icon
    QIcon* zufall = new QIcon(":/Bilder/shuffle-arrows.png");
    //https://www.flaticon.com/search/2?word=next&order_by=4&type=icon
    QIcon* zurueck = new QIcon(":/Bilder/last.png");
    QIcon* weiter = new QIcon(":/Bilder/next-button.png");
    //https://www.flaticon.com/free-icon/stop_727260?term=stop&page=1&position=27&page=1&position=27&related_id=727260&origin=search
    QIcon* stopp = new QIcon(":/Bilder/stop.png");
    //https://www.flaticon.com/search?word=silent&order_by=4&type=icon
    QIcon* stumm = new QIcon(":/Bilder/silent.png");
    //https://www.flaticon.com/search?word=pause&order_by=4&type=icon
    QIcon* pause = new QIcon(":/Bilder/pause.png");

    ui->wiedergabe_pause_button->setIcon(*wiedergabe);
    ui->zurueckspulen_button->setIcon(*zurueckspulen);
    ui->vorspulen_button->setIcon(*vorspulen);
    ui->zufall_button->setIcon(*zufall);
    ui->zurueckspringen_button->setIcon(*zurueck);
    ui->weiter_button->setIcon(*weiter);
    ui->stopp_button->setIcon(*stopp);
    ui->stumm_button->setIcon(*stumm);

}

Player::~Player()
{
    delete ui;
}

