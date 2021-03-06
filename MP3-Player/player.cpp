#include "player.h"
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
    QIcon* zurueckspulen_icon = new QIcon(":/Bilder/backward.png");
    QIcon* vorspulen_icon = new QIcon(":/Bilder/forward.png");
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
    ui->zurueckspulen_button->setIcon(*zurueckspulen_icon);
    ui->zurueckspulen_button->setIconSize(iconSize_zurueckspulen_vorspulen);
    ui->vorspulen_button->setIcon(*vorspulen_icon);
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
    QShortcut *random = new QShortcut(QKeySequence("Ctrl+R"), this);
    QShortcut *quieter = new QShortcut(QKeySequence("Down"),this);
    QShortcut *louder = new QShortcut(QKeySequence("Up"),this);


    //ToolTips
    ui->wiedergabe_pause_button->setToolTip("Wiedergabe/Pause");
    ui->zurueckspulen_button->setToolTip("Zur??ckspulen");
    ui->vorspulen_button->setToolTip("Vorspulen");
    ui->zufall_button->setToolTip("zuf??llige Wiedergabe");
    ui->zurueckspringen_button->setToolTip("Zur??ck");
    ui->weiter_button->setToolTip("weiter");
    ui->stopp_button->setToolTip("Stopp");
    ui->stumm_button->setToolTip("Stumm schalten");

    //Verkn??pfung der Oberfl??chenelemente und Shortcuts mit den Funktionen

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
    connect(quieter, SIGNAL(activated()), this, SLOT(leiser()));
    connect(louder, SIGNAL(activated()), this, SLOT(lauter()));
    connect(ui->stumm_button, SIGNAL(clicked()), this, SLOT(stummschalten()));
    connect(mute, SIGNAL(activated()), this, SLOT(shortcut_stummschalten()));
    connect(ui->weiter_button, SIGNAL(clicked()), this, SLOT(naechstes_lied()));
    connect(skip, SIGNAL(activated()), this, SLOT(naechstes_lied()));
    connect(ui->zurueckspringen_button, SIGNAL(clicked()), this, SLOT(vorheriges_lied()));
    connect(previous, SIGNAL(activated()), this, SLOT(vorheriges_lied()));
    connect(ui->zufall_button,SIGNAL(clicked()), this, SLOT(zufallslied()));
    connect(random, SIGNAL(activated()), this, SLOT(shortcut_zufallslied()));
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,this, &Player::lied_ausgewahlt);
    connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(customcontextmenu(QPoint)));
    connect(ui->tableWidget->verticalHeader(), &QHeaderView::sectionMoved, this, &Player::tabelle_dnd);
    connect(ui->tableWidget->horizontalHeader(), &QHeaderView::sectionClicked, this, &Player::sortieren);
    connect(ui->vorspulen_button, SIGNAL(clicked()),this, SLOT(vorspulen()));
    connect(ui->zurueckspulen_button, SIGNAL(clicked()),this, SLOT(zurueckspulen()));

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->widget->setVisible(false);
    daten_laden();
    tabellenansicht();
}

//Hier wird nach Bet??tigung der Taste Pfeil runter der Wert des Lautst??rke-Sliders um 1 verringert

void Player::leiser()
{
    int position = ui->lautstaerke_slider->value();
    ui->lautstaerke_slider->setSliderDown(true);
    ui->lautstaerke_slider->setSliderPosition(position-1);
    ui->lautstaerke_slider->setSliderDown(false);
}

//Hier wird nach Bet??tigung der Taste Pfeil hoch der Wert des Lautst??rke-Sliders um 1 erh??ht

void Player::lauter()
{
    int position = ui->lautstaerke_slider->value();
    ui->lautstaerke_slider->setSliderDown(true);
    ui->lautstaerke_slider->setSliderPosition(position+1);
    ui->lautstaerke_slider->setSliderDown(false);
}

//Hier werden falls vohanden Lieder, die in der json Datei gespeichert wurden, in die Playliste hinzugef??gt

void Player::daten_laden()
{
    QList<QUrl> urls;
    QString dateiname = "Dateien.json";
    QFileInfo datei(dateiname);

    if (datei.exists())
    {
        QFile song_datei(dateiname);

        if (!song_datei.open(QIODevice::ReadOnly))
        {
            QMessageBox fehlermeldung;
            fehlermeldung.critical(0, "Fehler", "Ein Fehler beim ??ffnen der Datei ist aufgetreten.");
            return;
        }

        QJsonObject json = QJsonDocument::fromJson(song_datei.readAll()).object();
        QJsonArray jsonDateien = json["dateien"].toArray();

        for (int j = 0; j<jsonDateien.size(); j ++)
        {
            QJsonObject jsonSong = jsonDateien[j].toObject();
            QString line = jsonSong["url"].toString();
            QUrl url(line);
            urls.append(url);
        }
    }
   hinzufugen_zur_playlist(urls);
}

//Hier wird der Button durch den verwendeten Shortcut auf Checked oder nicht Checked gesetzt

void Player::shortcut_zufallslied()
{
    if(!ui->zufall_button->isChecked())
    {
        ui->zufall_button->setChecked(true);
    }
    else
    {
        ui->zufall_button->setChecked(false);
    }

    zufallslied();
}

//Hier wird der Button durch den verwendeten Shortcut auf Checked oder nicht Checked gesetzt

void Player::shortcut_stummschalten()
{
    if(!ui->stumm_button->isChecked())
    {
        ui->stumm_button->setChecked(true);
    }
    else
    {
        ui->stumm_button->setChecked(false);
    }

    stummschalten();
}

//Hier wird das ContextMenu, das beim rechtsklick auf das tableWidget erscheint erstellt

void Player::customcontextmenu(const QPoint &pos)
{
    QMenu custommenu;
    QAction *u = custommenu.addAction("Aus der Playlist entfernen");
    QAction *a = custommenu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    if (a == u)
    {
        QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();

        if (selection.count() > 0) {
            QModelIndex index = selection.at(0);
            int row = index.row();
            loeschen(row);
        }
    }
}

//Diese Funktion l??scht einen Eintrag an der ausgew??hlten Position aus der Playliste gel??scht

void Player::loeschen(int pos)
{
    playlist->removeMedia(pos);
    daten_speichern();
    tabellenansicht();
}

//das hier ist der ??ffnen-Dialog und ??bergibt die Liste der urls an hinzufugen_zur_playlist

void Player::oeffnen()
{
    QFileDialog hinzufuegen(this);
    hinzufuegen.setAcceptMode(QFileDialog::AcceptOpen);
    hinzufuegen.setWindowTitle(tr("Musik hinzuf??gen"));
    hinzufuegen.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0,QDir::homePath()));
    hinzufuegen.setFileMode(QFileDialog::ExistingFiles);
    if (hinzufuegen.exec() == QDialog::Accepted)
    {
        hinzufugen_zur_playlist(hinzufuegen.selectedUrls());
    }
}

//hier wird ??berpr??ft, ob die Datei existiert und eine mp3-Datei ist

static bool ist_playlist(const QUrl &url)
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());

    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

//hier wird ??berpr??ft, ob es sich um eine mp3-Datei handelt und einzelt der Playlist hinzugef??gt

void Player::hinzufugen_zur_playlist(const QList<QUrl> &urls)
{
    liste_tempplayer_leeren();

    for (auto &url: urls) {
        const QFileInfo fileInfo(url.toLocalFile());
        QString Name = fileInfo.fileName();

        //??berpr??ft ob ausgew??hlte Datei mp3 Datei ist
        if(!Name.contains(".mp3"))
        {
            QMessageBox fehlermeldung;
            fehlermeldung.critical(0, "Fehler", "Die ausgew??hlte Datei ist keine MP3-Datei!");
        }
        else
        {
            if (ist_playlist(url))
            {
                playlist->load(url);
            }
            else
            {
                playlist->addMedia(url);
            }

            lied_hinzufuegen(url);
        }
    }
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    nur_wiedergabe();
}

//hier wird ein tempur??rer Player f??r die url erstellt, um dessen Daten zu erhalten

void Player::lied_hinzufuegen(QUrl url)
{
    QMediaPlayer *tempplayer = new QMediaPlayer(this);
    tempplayer->setMedia(url);
    tempplayer->play();
    tempplayer->setMuted(true);
    liste_tempplayer.append(tempplayer);
    QStringList list = url.toString().split(QLatin1Char('/'));
    QString dateiname = list[list.size() - 1];
    Datei_info* info = lied_erstellen(url.toString());
    info->setTitle(dateiname);

    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(tempplayer, QOverload<>::of(&QMediaPlayer::metaDataChanged), [=]() {
        QObject::disconnect(*conn);
        if (tempplayer->isMetaDataAvailable()) {
            QString titel = tempplayer->metaData(QMediaMetaData::Title).toString();
            QString album = tempplayer->metaData(QMediaMetaData::AlbumTitle).toString();
            QString artist = tempplayer->metaData(QMediaMetaData::ContributingArtist).toString();
            Datei_info* info = lied_erstellen(url.toString());

            if(!titel.isEmpty())
            {
                info->setTitle(titel);
            }
            if(!artist.isEmpty())
            {
                info->setArtist(artist);
            }
            if(!album.isEmpty())
            {
                info->setAlbum(album);
            }
            if(!url.isEmpty())
            {
                info->setUrl(url);
            }

            tabellenansicht();
            daten_speichern();
        }
    });

    auto conne = std::make_shared<QMetaObject::Connection>();
    *conne = connect(tempplayer, &QMediaPlayer::durationChanged, [=](int dur) {
        QObject::disconnect(*conne);

        dur /= 1000;
        QTime laenge((dur/3600) % 60, (dur/60) % 60, dur % 60, (dur *1000) % 1000);
        QString lang = laenge.toString("mm:ss");
        lied_erstellen(url.toString())->setLength(lang);

        tabellenansicht();
        tempplayer->stop();
    });
}

//die tempur??ren Player werden gel??scht und dessen liste geleert

void Player::liste_tempplayer_leeren()
{
    for(QMediaPlayer *tempplayer: liste_tempplayer)
    {
        tempplayer->deleteLater();
    }
    liste_tempplayer.clear();
}

//das aktuelle Lied wird gestoppt und das Button Icon angepasst

void Player::stopp()
{
    player->stop();
    if(wird_wiedergeben)
    {
        ui->wiedergabe_pause_button->setIcon(*wiedergabe_Icon);
        wird_wiedergeben = false;
    }
}

/*das Lied wird wiedergegeben wenn es pausiert ist und anders herum, ausschlaggebend ist der
  boolien wird_wiedergegeben */

void Player::wiedergabe()
{
    if(!wird_wiedergeben)
    {
        nur_wiedergabe();
    }else{
        ui->wiedergabe_pause_button->setIcon(*wiedergabe_Icon);
        player->pause();
        wird_wiedergeben = false;
    }
}

//Der Slider der Wiedergabe wurde bewegt, deswegen muss die Position des Players ge??ndert werden

void Player::geaenderte_position(int sekunden)
{
    player->setPosition(sekunden * 1000);
}

//Der Slider wird auf das neue Maximum des neuen Liedes gesetzt, sowie das Label auf den Titel mit K??nstler

void Player::neues_lied(qint64 millisekunden)
{
    ui->aktuelle_wiedergabe_slider->setMaximum(millisekunden / 1000);
    QString url = player->currentMedia().canonicalUrl().toString();
    QString titel = Liedersammlung[url]->getTitle();
    QString kuenstler = Liedersammlung[url]->getArtist();
    ui->aktuelles_lied->setText(titel + " - " + kuenstler);
}

//Hier werden die Labels mit der gespielten und gesamt Zeit gesetzt

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

//Hier wird die Lautst??rke auf den Wert der Position des Sliders gesetzt

void Player::lautstaerke_slider(int position)
{
    player->setVolume(position);
    lautstaerke = ui->lautstaerke_slider->value();
}

/*Hier wird die Lautst??rker auf 0 gesetzt, sofern sie vorher gr????er als 0 war. Ist die Laustst??rke bereits
0, so wird die Lautst??rke auf die vorherige Lautst??rke gesetzt*/

void Player::stummschalten()
{

    if(ui->lautstaerke_slider->value() != 0)
    {
        player->setVolume(0);
        ui->lautstaerke_slider->setValue(0);
    }
    else
    {
        player->setVolume(lautstaerke);
        ui->lautstaerke_slider->setValue(lautstaerke);
    }

    if(ui->stumm_button->isChecked())
    {
        ui->stumm_button->setStyleSheet("background-color: #0099FF");
    }
    else
    {
        ui->stumm_button->setStyleSheet("background-color: white");
    }
}

//Hier wird das vorherige Lied abgespielt und wiedergegeben

void Player::vorheriges_lied()
{
    playlist->previous();
    if(!wird_wiedergeben)
    {
        nur_wiedergabe();
    }
}

//Hier wird das n??chste Lied abgespielt und wiedergegeben

void Player::naechstes_lied()
{
    playlist->next();
    if(!wird_wiedergeben)
    {
        nur_wiedergabe();
    }
}

//Hier wird der Playbackmode falls der Button Checked ist auf Random gesetzt andernfalls auf Loop

void Player::zufallslied()
{
    if(ui->zufall_button->isChecked())
    {
        ui->zufall_button->setStyleSheet("background-color: #0099FF");
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
    else
    {
        ui->zufall_button->setStyleSheet("background-color: white");
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }

}

//Diese Funktion macht die Suchzeile sichtbar

void Player::suche()
{
    ui->widget->setVisible(true);
}

//Beim Aufruf dieser Funktion wird die Suchzeile geschlossen und die eingegebenen Werte aus den Feldern gel??scht

void Player::suche_beenden()
{
    ui->titel_lineEdit->clear();
    ui->kuenstler_lineEdit->clear();
    ui->album_lineEdit->clear();
    ui->widget->setVisible(false);
}

//gibt es eine ??nderung in der Sucheingabe, wird diese Funktion aufrufen, die den Text herausholt und mit den Tabelleneintr??gen vergleicht

void Player::suche_starten()
{
    QString titel = ui->titel_lineEdit->text();
    QString kuenstler = ui->kuenstler_lineEdit->text();
    QString album = ui->album_lineEdit->text();

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        bool treffer = true;
        treffer &= ist_treffer(ui->tableWidget->item(i, 0)->text(), titel);
        treffer &= ist_treffer(ui->tableWidget->item(i, 1)->text(), album);
        treffer &= ist_treffer(ui->tableWidget->item(i, 2)->text(), kuenstler);
        ui->tableWidget->setRowHidden( i, !treffer);
     }
}

//??berpr??fung ob es einen Sucheintag gibt oder ob der zeileninhalt mit der Sucheingabe ??bereinstimmt

bool Player::ist_treffer(const QString &zeilenInhalt, const QString &suchVorgabe)
{
    return suchVorgabe.isEmpty() || zeilenInhalt.contains(suchVorgabe);
}

/*hier wird die Tabelle erstellt, dabei gibt es eine unsichtbare Spalte mit den Urls
  existiert in der Map kein Eintrag mit der url so wird ein leeres DAtei_info-Objekt genutzt*/

void Player::tabellenansicht()
{
    Datei_info leer("", "", "", "", QUrl(""));

    ui->tableWidget->setRowCount(playlist->mediaCount());
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("Titel"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("Album"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("K??nstler"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("L??nge"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("Url"));
    ui->tableWidget->hideColumn(4);

    for (int lieder_nr = 0; lieder_nr < playlist->mediaCount(); lieder_nr++)
    {
        QMediaContent localMedia = playlist->media(lieder_nr);

        Datei_info *Daten = Liedersammlung[localMedia.canonicalUrl().toString()];
        if (Daten == nullptr) {
            Daten = &leer;
        }

        QTableWidgetItem *id = new QTableWidgetItem(QString::number(lieder_nr + 1));
        ui->tableWidget->setVerticalHeaderItem(lieder_nr, id);
        QTableWidgetItem *titel = new QTableWidgetItem(Daten->getTitle());
        ui->tableWidget->setItem(lieder_nr,0,titel);
        QTableWidgetItem *album = new QTableWidgetItem(Daten->getAlbum());
        ui->tableWidget->setItem(lieder_nr,1,album);
        QTableWidgetItem *kuenstler = new QTableWidgetItem(Daten->getArtist());
        ui->tableWidget->setItem(lieder_nr,2,kuenstler);
        QTableWidgetItem *laenge = new QTableWidgetItem(Daten->getLength());
        laenge->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(lieder_nr,3,laenge);
        QTableWidgetItem *url = new QTableWidgetItem(Daten->getUrl().toString());
        ui->tableWidget->setItem(lieder_nr,4,url);
    }

    //Zeile der L??nge kleiner als der Rest
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(1);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget->setColumnWidth(3,90);
    //Drag and Drop erm??glichen
    ui->tableWidget->verticalHeader()->setSectionsMovable(true);
    ui->tableWidget->verticalHeader()->setDragEnabled(true);
    ui->tableWidget->verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
}

/*wird nach einer tabellen Spalte sortiert, wird die Playlist einmal geleert und
mit den Urls aus der Tabelle in der richtigen Reihenfolge wieder gef??llt*/

void Player::sortieren(int i)
{
    Q_UNUSED(i);
    playlist->clear();

    for (int zeile = 0; zeile < ui->tableWidget->rowCount(); zeile++)
    {
        QString url_string = ui->tableWidget->item(zeile,4)->text();
        QUrl url(url_string);

        if (ist_playlist(url))
        {
            playlist->load(url);
        }
        else
        {
            playlist->addMedia(url);
        }
    }
}

//wird ein Element der Tabelle ??ber Drag and Drop verschoben wird diese auch in der Playlist verschoben

void Player::tabelle_dnd(int logischer_index, int alter_index, int neuer_index)
{
    Q_UNUSED(logischer_index);
    playlist->moveMedia(alter_index, neuer_index);
}

/*hier wird geschaut, ob zu der url schon ein Eintag in der Map existiert und wenn nicht wird ein
  eintrag mit "-" f??r alle Attribute erstellt */

Datei_info *Player::lied_erstellen(QString url)
{
    Datei_info* info = Liedersammlung[url];

    if (info == nullptr) {
        info = new Datei_info();
        info->setTitle("-");
        info->setArtist("-");
        info->setAlbum("-");
        info->setLength("-");
        Liedersammlung[url] = info;
    }

    return info;
}

//bei doppelklick auf eine Zeile wird diese Zeile als aktuell wiederzugebenes Lied gesetzt

void Player::lied_ausgewahlt(int zeile, int spalte)
{
    Q_UNUSED(spalte);
    playlist->setCurrentIndex(zeile);
    nur_wiedergabe();
}

//Hier werden die Daten in der json Datei gespeichert

QJsonObject Player::json_erstellen()
{
    QJsonArray json_dateien;

    for (int i = 0; i < playlist->mediaCount(); i++)
    {
        QUrl url = playlist->media(i).canonicalUrl();
        QJsonObject json_datei;
        json_datei["url"] = url.toString();
        json_dateien.append(json_datei);
    }

    QJsonObject json;
    json["dateien"] = json_dateien;

    return json;
}

//In dieser Funktion wird die json Datei erstellt

void Player::daten_speichern()
{
    QString dateiname = "Dateien.json";
    QFile song_datei(dateiname);

    if (!song_datei.open(QIODevice::WriteOnly))
    {
        QMessageBox fehlermeldung;
        fehlermeldung.critical(0, "Fehler", "Ein Fehler beim ??ffnen der Datei ist aufgetreten.");
        return;
    }

    QJsonObject json = json_erstellen();

    song_datei.write(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

//das aktuell wiedergegebene Lied wird um 5sek nach vorne gespult

void Player::vorspulen()
{
    int position = player->position();
    player->setPosition(position + 5000);
}

//das aktuell wiedergegebene Lied wird um 5sek zur??ck gespult

void Player::zurueckspulen()
{
    int position = player->position();
    player->setPosition(position - 5000);
}

//die Playlist wird wiedergegeben egal in welchen Modus sie ist

void Player::nur_wiedergabe()
{
    ui->wiedergabe_pause_button->setIcon(*pause);
    player->play();
    wird_wiedergeben = true;
}

//Der Destruktur der Klasse

Player::~Player()
{
    delete ui;
}
