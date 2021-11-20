#ifndef DATEI_INFO_H
#define DATEI_INFO_H
#include <QString>
#include <QUrl>

class Datei_info
{
public:
    Datei_info(
        QString title,
        QString artist,
        QString album,
        QString length,
        int id,
        QUrl url
        );
    Datei_info();

    QString getTitle() const;
    void setTitle(const QString &value);

    QString getArtist() const;
    void setArtist(const QString &value);

    QString getAlbum() const;
    void setAlbum(const QString &value);

    QString getLength() const;
    void setLength(const QString &value);

    int getId() const;
    void setId(int value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

private:
    //Variablen
    QString title;
    QString artist;
    QString album;
    QString length;
    int id;
    QUrl url;
};

#endif // DATEI_INFO_H
