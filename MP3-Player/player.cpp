#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);

    //Icons

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
    //https://www.flaticon.com/search?word=stop&type=icon
    QIcon* stopp = new QIcon(":/Bilder/stop.png");
    //https://www.flaticon.com/search?word=silent&order_by=4&type=icon
    QIcon* stumm = new QIcon(":/Bilder/silent.png");
    //https://www.flaticon.com/search?word=pause&order_by=4&type=icon
    QIcon* pause = new QIcon(":/Bilder/pause.png");

    QSize iconSize_zurueckspulen_vorspulen;
    iconSize_zurueckspulen_vorspulen.setWidth(60);
    iconSize_zurueckspulen_vorspulen.setHeight(60);

    QSize iconSize;
    iconSize.setWidth(52);
    iconSize.setHeight(52);

    ui->wiedergabe_pause_button->setIcon(*wiedergabe);
    ui->wiedergabe_pause_button->setIconSize(iconSize);
    ui->zurueckspulen_button->setIcon(*zurueckspulen);
    ui->zurueckspulen_button->setIconSize(iconSize_zurueckspulen_vorspulen);
    ui->vorspulen_button->setIcon(*vorspulen);
    ui->vorspulen_button->setIconSize(iconSize_zurueckspulen_vorspulen);
    ui->zufall_button->setIcon(*zufall);
    ui->zufall_button->setIconSize(iconSize);
    ui->zurueckspringen_button->setIcon(*zurueck);
    ui->zurueckspringen_button->setIconSize(iconSize);
    ui->weiter_button->setIcon(*weiter);
    ui->weiter_button->setIconSize(iconSize);
    ui->stopp_button->setIcon(*stopp);
    ui->stopp_button->setIconSize(iconSize);
    ui->stumm_button->setIcon(*stumm);
    ui->stumm_button->setIconSize(iconSize);

    //ToolTips
    ui->wiedergabe_pause_button->setToolTip("Wiedergabe/Pause");
    ui->zurueckspulen_button->setToolTip("Zurückspulen");
    ui->vorspulen_button->setToolTip("Vorspulen");
    ui->zufall_button->setToolTip("zufällige Wiedergabe");
    ui->zurueckspringen_button->setToolTip("Zurück");
    ui->weiter_button->setToolTip("weiter");
    ui->stopp_button->setToolTip("Stopp");
    ui->stumm_button->setToolTip("Stumm schalten");


    ui->widget->setVisible(false);

}

Player::~Player()
{
    delete ui;
}

