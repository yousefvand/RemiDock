#include "IconImageProvider.h"

#include <QFile>
#include <QIcon>
#include <QPainter>
#include <QUrl>

IconImageProvider::IconImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap IconImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const int width = requestedSize.width() > 0 ? requestedSize.width() : 64;
    const int height = requestedSize.height() > 0 ? requestedSize.height() : 64;

    QString iconId = id;

    const int revisionSeparator = iconId.indexOf(QLatin1Char('/'));
    if (revisionSeparator > 0) {
        bool revisionOk = false;
        iconId.left(revisionSeparator).toInt(&revisionOk);
        if (revisionOk)
            iconId = iconId.mid(revisionSeparator + 1);
    }
    if (iconId.startsWith(QStringLiteral("file://"))) {
        const QUrl url(iconId);
        if (url.isLocalFile())
            iconId = url.toLocalFile();
    }

    QPixmap pixmap;

    if (QFile::exists(iconId)) {
        pixmap.load(iconId);
        if (!pixmap.isNull())
            pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    if (pixmap.isNull()) {
        QIcon icon = QIcon::fromTheme(iconId);

        if (icon.isNull())
            icon = QIcon::fromTheme("application-x-executable");

        pixmap = icon.pixmap(width, height);
    }

    if (pixmap.isNull()) {
        pixmap = QPixmap(width, height);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::gray);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(4, 4, width - 8, height - 8, 12, 12);
    }

    if (size)
        *size = pixmap.size();

    return pixmap;
}
