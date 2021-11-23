#include "datei_info.h"

Datei_info::Datei_info()
{

}

Datei_info::Datei_info( QString title, QString artist, QString album, QString length, int id, QUrl url)
    :title(title), artist(artist), album(album), length(length), id(id), url(url)
{

}

QString Datei_info::getTitle() const
{
    return title;
}

void Datei_info::setTitle(const QString &value)
{
    title = value;
}

QString Datei_info::getArtist() const
{
    return artist;
}

void Datei_info::setArtist(const QString &value)
{
    artist = value;
}

QString Datei_info::getAlbum() const
{
    return album;
}

void Datei_info::setAlbum(const QString &value)
{
    album = value;
}

QString Datei_info::getLength() const
{
    return length;
}

void Datei_info::setLength(const QString &value)
{
    length = value;
}

int Datei_info::getId() const
{
    return id;
}

void Datei_info::setId(int value)
{
    id = value;
}

QUrl Datei_info::getUrl() const
{
    return url;
}

void Datei_info::setUrl(const QUrl &value)
{
    url = value;
}

void Datei_info::write(QJsonObject &json) const
{
    json["titel"] = title;
    json["artist"] = artist;
    json["ablum"] = album;
    json["url"] = url.toString();
}
