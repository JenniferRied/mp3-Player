#include "player.h"
#include "datei_infos.h"
#include "ui_player.h"
#include <QMessageBox>
#include <QMediaService>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QtWidgets>
#include <iostream>
#include <QShortcut>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    player->setAudioRole(QAudio::VideoRole);
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    ui->aktuelle_wiedergabe_slider->setRange(0, player->duration()/ 1000);
    //Icons

    //https://www.flaticon.com/de/kostenloses-icon/wiedergabetaste_149668?term=play&page=1&position=2&page=1&position=2&related_id=149668&origin=search
    wiedergabe_Icon = new QIcon(":/Bilder/wiedergabetaste.png");
    //https://www.flaticon.com/premium-icon/backward_2938636?term=backward&page=1&position=40&page=1&position=40&related_id=2938636&origin=search
    QIcon* zurueckspulen = new QIcon(":/Bilder/backward.png");
    QIcon* vorspulen = new QIcon(":/Bilder/forward.png");
    //https://www.flaticon.com/search?word=random&order_by=4&type=icon
    QIcon* zufall = new QIcon(":/Bilder/shuffle-arrows.png");
    //https://www.flaticon.com/search/2?word=next&order_by=4&type=icon
    QIcon* zurueck = new QIcon(":/Bilder/last.png");
    QIcon* weiter = new QIcon(":/Bilder/next-button.png");
    //https://www.flaticon.com/search?word=stop&type=icon
    QIcon* stopp_Icon = new QIcon(":/Bilder/stop.png");
    //https://www.flaticon.com/search?word=silent&order_by=4&type=icon
    QIcon* stumm = new QIcon(":/Bilder/silent.png");
    //https://www.flaticon.com/search?word=pause&order_by=4&type=icon
    pause = new QIcon(":/Bilder/pause.png");

    QSize iconSize_zurueckspulen_vorspulen;
    iconSize_zurueckspulen_vorspulen.setWidth(60);
    iconSize_zurueckspulen_vorspulen.setHeight(60);

    QSize iconSize;
    iconSize.setWidth(52);
    iconSize.setHeight(52);

    QSize iconSize_suche;
    iconSize_suche.setWidth(28);
    iconSize_suche.setHeight(28);

    ui->wiedergabe_pause_button->setIcon(*wiedergabe_Icon);
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
    ui->stopp_button->setIcon(*stopp_Icon);
    ui->stopp_button->setIconSize(iconSize);
    ui->stumm_button->setIcon(*stumm);
    ui->stumm_button->setIconSize(iconSize);

    //Shortcuts
    QShortcut *skip = new QShortcut(QKeySequence(Qt::Key_Right), this);
    QShortcut *previous = new QShortcut(QKeySequence(Qt::Key_Left), this);
    QShortcut *pause_play = new QShortcut(QKeySequence(Qt::Key_Space), this);
    QShortcut *stop = new QShortcut(QKeySequence("Ctrl+X"), this);
    QShortcut *mute = new QShortcut(QKeySequence("Ctrl+M"), this);

    //ToolTips
    ui->wiedergabe_pause_button->setToolTip("Wiedergabe/Pause");
    ui->zurueckspulen_button->setToolTip("Zurückspulen");
    ui->vorspulen_button->setToolTip("Vorspulen");
    ui->zufall_button->setToolTip("zufällige Wiedergabe");
    ui->zurueckspringen_button->setToolTip("Zurück");
    ui->weiter_button->setToolTip("weiter");
    ui->stopp_button->setToolTip("Stopp");
    ui->stumm_button->setToolTip("Stumm schalten");


    connect(ui->menuSuche, &QAction::triggered, this, &Player::suche);
    connect(ui->titel_lineEdit, &QLineEdit::textChanged, this, &Player::suche_starten);
    connect(ui->kuenstler_lineEdit, &QLineEdit::textChanged, this, &Player::suche_starten);
    connect(ui->album_lineEdit, &QLineEdit::textChanged, this, &Player::suche_starten);
    connect(ui->suche_schliessen_button, SIGNAL(clicked()), this, SLOT (suche_beenden()));
    connect(ui->wiedergabe_pause_button, SIGNAL(clicked()),this, SLOT(wiedergabe()));
    connect(pause_play, SIGNAL(activated()), this, SLOT(wiedergabe()));
    connect(ui->stopp_button,SIGNAL(clicked()),this, SLOT(stopp()));
    connect(stop, SIGNAL(activated()), this, SLOT(stopp()));
    connect(ui->menuLieder_hinzuf_gen, &QAction::triggered, this, &Player::oeffnen);
    connect(player,&QMediaPlayer::positionChanged, this, &Player::geaenderte_zeit);
    connect(player, &QMediaPlayer::durationChanged, this, &Player::neues_lied);
    connect(ui->aktuelle_wiedergabe_slider, &QSlider::sliderMoved,this, &Player::geaenderte_position);
    connect(ui->lautstaerke_slider, &QSlider::sliderMoved, this, &Player::lautstaerke_slider);
    connect(ui->stumm_button, SIGNAL(clicked()), this, SLOT(stummschalten()));
    connect(mute, SIGNAL(activated()), this, SLOT(stummschalten()));
    connect(ui->weiter_button, SIGNAL(clicked()), this, SLOT(naechstes_lied()));
    connect(skip, SIGNAL(activated()), this, SLOT(naechstes_lied()));
    connect(ui->zurueckspringen_button, SIGNAL(clicked()), this, SLOT(vorheriges_lied()));
    connect(previous, SIGNAL(activated()), this, SLOT(vorheriges_lied()));

    ui->widget->setVisible(false);
}

void Player::oeffnen()
{
    QFileDialog hinzufuegen(this);
    hinzufuegen.setAcceptMode(QFileDialog::AcceptOpen);
    hinzufuegen.setWindowTitle(tr("Musik hinzufügen"));
    hinzufuegen.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0,QDir::homePath()));
    if (hinzufuegen.exec() == QDialog::Accepted)
        hinzufugen_zur_playlist(hinzufuegen.selectedUrls());
}


static bool ist_playlist(const QUrl &url)
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void Player::hinzufugen_zur_playlist(const QList<QUrl> &urls)
{
    for (auto &url: urls) {
        if (ist_playlist(url))
            playlist->load(url);
        else
            playlist->addMedia(url);
    }
}

void Player::stopp()
{
    player->stop();
    if(wird_wiedergeben)
    {
        ui->wiedergabe_pause_button->setIcon(*wiedergabe_Icon);
        wird_wiedergeben = false;
    }
}

void Player::wiedergabe()
{
    if(!wird_wiedergeben)
    {
        ui->wiedergabe_pause_button->setIcon(*pause);
        player->play();
        wird_wiedergeben = true;
    }else{
        ui->wiedergabe_pause_button->setIcon(*wiedergabe_Icon);
        player->pause();
        wird_wiedergeben = false;
    }
}

void Player::geaenderte_position(int sekunden)
{
    player->setPosition(sekunden * 1000);
}


void Player::neues_lied(qint64 millisekunden)
{
    ui->aktuelle_wiedergabe_slider->setMaximum(millisekunden / 1000);
}

void Player::geaenderte_zeit(qint64 progress)
{
    if(!ui->aktuelle_wiedergabe_slider->isSliderDown())
    {
        ui->aktuelle_wiedergabe_slider->setValue(progress / 1000);
    }
    int dauer = player->duration() / 1000;
    QTime laenge((dauer/3600) % 60, (dauer/60) % 60, dauer % 60, (dauer *1000) % 1000);
    ui->dauer_label->setText(laenge.toString("mm:ss"));
    int gespielt = progress / 1000;
    QTime spielte((gespielt/3600) % 60, (gespielt/60) % 60, gespielt % 60, (gespielt *1000) % 1000);
    ui->bisherige_dauer_label->setText(spielte.toString("mm:ss"));
}

void Player::lautstaerke_slider(int position)
{
    player->setVolume(position);
}

void Player::stummschalten()
{
    if(ui->lautstaerke_slider->value() != 0)
    {
        player->setVolume(0);
        ui->lautstaerke_slider->setValue(0);
    }
    else
    {
        player->setVolume(100);
        ui->lautstaerke_slider->setValue(100);
    }
}

void Player::vorheriges_lied()
{
    playlist->previous();
    if(!wird_wiedergeben)
    {
        ui->wiedergabe_pause_button->setIcon(*pause);
        player->play();
        wird_wiedergeben = true;
    }
}

void Player::naechstes_lied()
{
    playlist->next();
    if(!wird_wiedergeben)
    {
        ui->wiedergabe_pause_button->setIcon(*pause);
        player->play();
        wird_wiedergeben = true;
    }
}

void Player::suche()
{
    ui->widget->setVisible(true);
}

void Player::suche_beenden()
{
    ui->titel_lineEdit->clear();
    ui->kuenstler_lineEdit->clear();
    ui->album_lineEdit->clear();
    ui->widget->setVisible(false);
}

void Player::suche_starten()
{
    QString titel = ui->titel_lineEdit->text();
    QString kuenstler = ui->kuenstler_lineEdit->text();
    QString album = ui->album_lineEdit->text();

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        bool treffer = true;
        treffer &= ist_treffer(ui->tableWidget->item(i, 0)->text(), titel);
        treffer &= ist_treffer(ui->tableWidget->item(i, 1)->text(), kuenstler);
        treffer &= ist_treffer(ui->tableWidget->item(i, 2)->text(), album);

        ui->tableWidget->setRowHidden( i, !treffer);
        kein_treffer.append(*ui->tableWidget->item( i, !treffer));
     }

    ui->suche_schliessen_button->setVisible(true);
}

bool Player::ist_treffer(const QString &zeilenInhalt, const QString &suchVorgabe)
{
    return suchVorgabe.isEmpty() || zeilenInhalt.contains(suchVorgabe);
}

Player::~Player()
{
    delete ui;
}
