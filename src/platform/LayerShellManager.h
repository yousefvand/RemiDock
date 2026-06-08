#pragma once

#include <QObject>

class QQuickWindow;
class RemiDockController;

class LayerShellManager : public QObject
{
    Q_OBJECT

public:
    explicit LayerShellManager(RemiDockController *controller, QObject *parent = nullptr);

    void setWindow(QQuickWindow *window);
    Q_INVOKABLE void apply();

private:
    QQuickWindow *m_window = nullptr;
    RemiDockController *m_controller = nullptr;
};
