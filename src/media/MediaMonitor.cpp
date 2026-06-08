#include "MediaMonitor.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusReply>
#include <QStringList>
#include <QVariant>

MediaMonitor::MediaMonitor(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &MediaMonitor::refresh);
    m_timer.setInterval(1000);
    m_timer.start();

    refresh();
}

bool MediaMonitor::playing() const
{
    return m_playing;
}

void MediaMonitor::setPlaying(bool value)
{
    if (m_playing == value)
        return;

    m_playing = value;
    emit playingChanged();
}

void MediaMonitor::refresh()
{
    QDBusConnection bus = QDBusConnection::sessionBus();

    if (!bus.isConnected()) {
        setPlaying(false);
        return;
    }

    QDBusConnectionInterface *iface = bus.interface();

    if (!iface) {
        setPlaying(false);
        return;
    }

    QDBusReply<QStringList> servicesReply = iface->registeredServiceNames();

    if (!servicesReply.isValid()) {
        setPlaying(false);
        return;
    }

    const QStringList services = servicesReply.value();

    for (const QString &service : services) {
        if (!service.startsWith(QStringLiteral("org.mpris.MediaPlayer2.")))
            continue;

        QDBusInterface properties(
            service,
            QStringLiteral("/org/mpris/MediaPlayer2"),
            QStringLiteral("org.freedesktop.DBus.Properties"),
            bus
        );

        if (!properties.isValid())
            continue;

        QDBusReply<QVariant> statusReply = properties.call(
            QStringLiteral("Get"),
            QStringLiteral("org.mpris.MediaPlayer2.Player"),
            QStringLiteral("PlaybackStatus")
        );

        if (!statusReply.isValid())
            continue;

        if (statusReply.value().toString() == QStringLiteral("Playing")) {
            setPlaying(true);
            return;
        }
    }

    setPlaying(false);
}
