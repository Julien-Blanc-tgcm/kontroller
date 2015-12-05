#ifndef KODIFILE_H
#define KODIFILE_H

#include <QObject>

class KodiFile : public QObject
{
    Q_OBJECT

private:
    QString file_;
    QString filetype_;
    QString label_;
    QString type_;
public:
    explicit KodiFile(QObject *parent = 0);

public:
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString filetype READ filetype WRITE setFiletype NOTIFY filetypeChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)


    QString file() const;
    QString filetype() const;
    QString label() const;
    QString type() const;
signals:

    void fileChanged(QString file);
    void filetypeChanged(QString filetype);
    void labelChanged(QString label);
    void typeChanged(QString type);

public slots:
    void setFile(QString file);
    void setFiletype(QString filetype);
    void setLabel(QString label);
    void setType(QString type);

};

#endif // KODIFILE_H
