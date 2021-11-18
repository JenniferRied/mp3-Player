#ifndef DATEI_INFOS_H
#define DATEI_INFOS_H
#include <QString>
#endif // DATEI_INFOS_H

class DateiInfos {
public:
    DateiInfos(
        QString title,
        QString artist,
        QString album,
        QString length
        );
    DateiInfos(){};
private:
    //Variablen
        QString title;
        QString artist;
        QString album;
        QString length;
};
