#ifndef PHOTOBASE_H
#define PHOTOBASE_H

#include <QObject>

class PhotoBase : public QObject
{
    Q_OBJECT
public:
    PhotoBase();
signals:
    void statusChanged(QString, int);
    void processStarted();

private:
};

#endif // PHOTOBASE_H
