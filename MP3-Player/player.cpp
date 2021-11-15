#include "player.h"
#include "ui_player.h"
#include <QMessageBox>
#include <QMediaService>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QtWidgets>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    player->setAudioRole(QAudio::VideoRole);
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
    //Icons

    //https://www.flaticon.com/de/suche?word=lupe
    QIcon* suche = new QIcon(":/Bilder/lupe.png");
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
    //QIcon* pause = new QIcon(":/Bilder/pause.png");

    QSize iconSize_zurueckspulen_vorspulen;
    iconSize_zurueckspulen_vorspulen.setWidth(60);
    iconSize_zurueckspulen_vorspulen.setHeight(60);

    QSize iconSize;
    iconSize.setWidth(52);
    iconSize.setHeight(52);

    QSize iconSize_suche;
    iconSize_suche.setWidth(28);
    iconSize_suche.setHeight(28);

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
    ui->suche_starten_button->setIcon(*suche);
    ui->suche_starten_button->setIconSize(iconSize_suche);

    //ToolTips
    ui->wiedergabe_pause_button->setToolTip("Wiedergabe/Pause");
    ui->zurueckspulen_button->setToolTip("Zurückspulen");
    ui->vorspulen_button->setToolTip("Vorspulen");
    ui->zufall_button->setToolTip("zufällige Wiedergabe");
    ui->zurueckspringen_button->setToolTip("Zurück");
    ui->weiter_button->setToolTip("weiter");
    ui->stopp_button->setToolTip("Stopp");
    ui->stumm_button->setToolTip("Stumm schalten");


    connect(ui->menuSuche, SIGNAL (aboutToShow()), this, SLOT (suche()));
    connect(ui->suche_starten_button, SIGNAL (clicked()), this, SLOT (suche_starten()));
    connect(ui->suche_schliessen_button, SIGNAL(clicked()), this, SLOT (suche_beenden()));

    connect(ui->menuLieder_hinzuf_gen, SIGNAL (aboutToShow()), this, SLOT (oeffnen()));

    ui->widget->setVisible(false);   
}

void Player::oeffnen()
{
    QFileDialog hinzufuegen(this);
    hinzufuegen.setAcceptMode(QFileDialog::AcceptOpen);
    hinzufuegen.setWindowTitle(tr("Musik hinzufügen"));
    hinzufuegen.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0,QDir::homePath()));
    if (hinzufuegen.exec() == QDialog::Accepted)
        addToPlaylist(hinzufuegen.selectedUrls());
}


static bool isPlaylist(const QUrl &url)
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void Player::addToPlaylist(const QList<QUrl> &urls)
{
    for (auto &url: urls) {
        if (isPlaylist(url))
            playlist->load(url);
        else
            playlist->addMedia(url);
    }
}

void Player::suche()
{
    ui->widget->setVisible(true);
    ui->suche_schliessen_button->setVisible(false);
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
    bool keine_eingabe = false;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            bool treffer = false;
            for(int j = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem *eintrag = ui->tableWidget->item(i, j);
                QTableWidgetItem *eintrag2 = ui->tableWidget->item(i,j+1);
                QTableWidgetItem *eintrag3 = ui->tableWidget->item(i,j+2);

                //Eingabe cases

                //Fehlermeldung wenn kein Feld ausgefüllt wurde


                if(ui->album_lineEdit->text().isEmpty() && ui->titel_lineEdit->text().isEmpty() && ui->kuenstler_lineEdit->text().isEmpty())
                {
                    keine_eingabe = true;
                }


                if(!ui->titel_lineEdit->text().isEmpty() && !ui->kuenstler_lineEdit->text().isEmpty())
                {
                    //alle 3 Felder ausgefüllt

                    if(!ui->album_lineEdit->text().isEmpty())
                    {
                        if( eintrag->text() == titel and eintrag2->text() == kuenstler and eintrag3->text() == album)
                        {
                            treffer = true;
                            break;
                        }
                    }

                    //Titel und Künstler ausgefüllt

                    if(ui->album_lineEdit->text().isEmpty())
                    {
                        if( eintrag->text()== titel and eintrag2->text() == kuenstler )
                        {
                            treffer = true;
                            break;
                        }
                    }
                }



                if(!ui->titel_lineEdit->text().isEmpty() || !ui->kuenstler_lineEdit->text().isEmpty())
                {
                    //entweder Titel oder Künstler ausgefüllt

                    if(ui->album_lineEdit->text().isEmpty())
                    {
                        if( eintrag->text().contains(titel + kuenstler))
                        {
                            treffer = true;
                            break;
                        }
                    }

                    //Album ebenfalls ausgefüllt
                    if(!ui->album_lineEdit->text().isEmpty())
                    {
                        //Titel und Album ausgefüllt
                        if(!ui->titel_lineEdit->text().isEmpty())
                        {
                            if(eintrag->text() == titel && eintrag3->text() == album)
                            {
                                treffer = true;
                                break;
                            }
                        }
                        //Künstler und Album ausgefüllt
                        else
                        {
                            if(eintrag->text() == kuenstler && eintrag2->text() == album)
                            {
                                treffer = true;
                                break;
                            }
                        }
                    }

                }

                //Nur Album ausgefüllt

                if(ui->titel_lineEdit->text().isEmpty() && ui->kuenstler_lineEdit->text().isEmpty())
                {
                    if(eintrag->text().contains(album))
                    {
                        treffer = true;
                        break;
                    }
                }



            }
            ui->tableWidget->setRowHidden( i, !treffer);
            kein_treffer.append(*ui->tableWidget->item( i, !treffer));
        }

    //Fehlermeldung, falls der Nutzer versucht die Suche zu starten ohne ein Suchkriterium eingegeben zu haben

    if(keine_eingabe == true)
    {
        QMessageBox fehlermeldung;
        fehlermeldung.critical(0, "Fehler", "Sie müssen mindestens 1 Feld für die Suche ausfüllen.");
    }

    ui->suche_schliessen_button->setVisible(true);

}

Player::~Player()
{
    delete ui;
}

