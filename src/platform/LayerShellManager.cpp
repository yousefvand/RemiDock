#include "LayerShellManager.h"

#include <QGuiApplication>
#include <QMargins>
#include <QQuickWindow>
#include <QScreen>
#include <QTimer>

#include <LayerShellQt/Window>

#include "RemiDockController.h"

namespace {
QMargins edgeMargins(const QString &edge, int margin)
{
    if (edge == "bottom")
        return QMargins(0, 0, 0, margin);

    if (edge == "top")
        return QMargins(0, margin, 0, 0);

    if (edge == "left")
        return QMargins(margin, 0, 0, 0);

    if (edge == "right")
        return QMargins(0, 0, margin, 0);

    return QMargins();
}
}

LayerShellManager::LayerShellManager(RemiDockController *controller, QObject *parent)
    : QObject(parent),
      m_controller(controller)
{
    connect(m_controller, &RemiDockController::layerShellNeedsReconfigure, this, [this]() {
        QTimer::singleShot(0, this, &LayerShellManager::apply);
    });
}

void LayerShellManager::setWindow(QQuickWindow *window)
{
    m_window = window;
}

void LayerShellManager::apply()
{
    if (!m_window || !m_controller)
        return;

    const QString edge = m_controller->edge();

    m_window->setColor(Qt::transparent);

    /*
     * Do not resize the layer-shell surface to the whole screen.
     * The QML Window is already bound to the real dock size.  Keeping the
     * surface small prevents the annoying black/opaque background rectangle
     * when the compositor/theme does not blend the transparent area perfectly.
     */
    m_window->resize(qMax(1, m_window->width()), qMax(1, m_window->height()));

    auto *layerWindow = LayerShellQt::Window::get(m_window);
    if (!layerWindow)
        return;

    LayerShellQt::Window::Anchors anchors;

    if (edge == "bottom")
        anchors = LayerShellQt::Window::AnchorBottom;
    else if (edge == "top")
        anchors = LayerShellQt::Window::AnchorTop;
    else if (edge == "left")
        anchors = LayerShellQt::Window::AnchorLeft;
    else if (edge == "right")
        anchors = LayerShellQt::Window::AnchorRight;
    else
        anchors = LayerShellQt::Window::AnchorBottom;

    layerWindow->setLayer(m_controller->dockMode() == QStringLiteral("dodge")
                              ? LayerShellQt::Window::LayerBottom
                              : LayerShellQt::Window::LayerTop);
    layerWindow->setScope(QStringLiteral("remidock"));
    layerWindow->setAnchors(anchors);
    layerWindow->setMargins(edgeMargins(edge, 8));
    layerWindow->setExclusiveZone(0);
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);

    if (!m_window->isVisible())
        m_window->show();
}
