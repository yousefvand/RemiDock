#pragma once

#include <QQuickImageProvider>

class IconImageProvider : public QQuickImageProvider
{
public:
    IconImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};
