#pragma once

#include <QObject>
#include <QTimer>

class MediaMonitor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged)

public:
    explicit MediaMonitor(QObject *parent = nullptr);

    bool playing() const;

public slots:
    void refresh();

signals:
    void playingChanged();

private:
    void setPlaying(bool value);

    bool m_playing = false;
    QTimer m_timer;
};
