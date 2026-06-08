#include "RemiDockController.h"

#include <QtMath>

RemiDockController::RemiDockController(QObject *parent)
    : QObject(parent),
      m_settings("remisa", "RemiDock")
{
    m_iconSize = m_settings.value("Dock/iconSize", 48).toInt();
    m_dockHeight = m_settings.value("Dock/dockHeight", 82).toInt();
    m_autoHide = m_settings.value("Dock/autoHide", false).toBool();
    m_edge = m_settings.value("Dock/edge", "bottom").toString();
    m_musicDanceEnabled = m_settings.value("Dock/musicDanceEnabled", false).toBool();
    m_dockMode = m_settings.value("Dock/dockMode", "always").toString();
    m_hoverAnimation = m_settings.value("Dock/hoverAnimation", "simple").toString();

    if (!isValidEdge(m_edge))
        m_edge = "bottom";

    if (!isValidDockMode(m_dockMode))
        m_dockMode = "always";

    if (!isValidHoverAnimation(m_hoverAnimation))
        m_hoverAnimation = "simple";
}

int RemiDockController::iconSize() const { return m_iconSize; }
int RemiDockController::dockHeight() const { return m_dockHeight; }
bool RemiDockController::autoHide() const { return m_autoHide; }
QString RemiDockController::edge() const { return m_edge; }
bool RemiDockController::editMode() const { return m_editMode; }
bool RemiDockController::settingsVisible() const { return m_settingsVisible; }
bool RemiDockController::musicDanceEnabled() const { return m_musicDanceEnabled; }
QString RemiDockController::dockMode() const { return m_dockMode; }
QString RemiDockController::hoverAnimation() const { return m_hoverAnimation; }

bool RemiDockController::isValidEdge(const QString &value) const
{
    return value == "bottom" || value == "top" || value == "left" || value == "right";
}

bool RemiDockController::isValidDockMode(const QString &value) const
{
    return value == "always" || value == "hover" || value == "dodge";
}

bool RemiDockController::isValidHoverAnimation(const QString &value) const
{
    return value == "simple" || value == "zoom" || value == "fire" || value == "fade";
}

void RemiDockController::setIconSize(int value)
{
    value = qBound(24, value, 96);

    if (m_iconSize == value)
        return;

    m_iconSize = value;
    save();
    emit iconSizeChanged();
}

void RemiDockController::setDockHeight(int value)
{
    value = qBound(48, value, 160);

    if (m_dockHeight == value)
        return;

    m_dockHeight = value;
    save();
    emit dockHeightChanged();
}

void RemiDockController::setAutoHide(bool value)
{
    if (m_autoHide == value)
        return;

    m_autoHide = value;
    save();
    emit autoHideChanged();
}

void RemiDockController::setEdge(const QString &value)
{
    if (!isValidEdge(value))
        return;

    if (m_edge == value)
        return;

    m_edge = value;
    save();
    emit edgeChanged();
    emit layerShellNeedsReconfigure();
}

void RemiDockController::setEditMode(bool value)
{
    if (m_editMode == value)
        return;

    m_editMode = value;
    emit editModeChanged();
}

void RemiDockController::setSettingsVisible(bool value)
{
    if (m_settingsVisible == value)
        return;

    m_settingsVisible = value;
    emit settingsVisibleChanged();
}

void RemiDockController::setMusicDanceEnabled(bool value)
{
    if (m_musicDanceEnabled == value)
        return;

    m_musicDanceEnabled = value;
    save();
    emit musicDanceEnabledChanged();
}


void RemiDockController::setDockMode(const QString &value)
{
    if (!isValidDockMode(value))
        return;

    if (m_dockMode == value)
        return;

    m_dockMode = value;
    save();
    emit dockModeChanged();
    emit layerShellNeedsReconfigure();
}


void RemiDockController::setHoverAnimation(const QString &value)
{
    if (!isValidHoverAnimation(value))
        return;

    if (m_hoverAnimation == value)
        return;

    m_hoverAnimation = value;
    save();
    emit hoverAnimationChanged();
}

void RemiDockController::cycleEdge()
{
    if (m_edge == "bottom")
        setEdge("left");
    else if (m_edge == "left")
        setEdge("top");
    else if (m_edge == "top")
        setEdge("right");
    else
        setEdge("bottom");
}

void RemiDockController::setEdgeFromDrag(qreal dx, qreal dy)
{
    const qreal threshold = 18.0;

    if (qAbs(dx) < threshold && qAbs(dy) < threshold)
        return;

    if (qAbs(dx) > qAbs(dy))
        setEdge(dx < 0 ? "left" : "right");
    else
        setEdge(dy < 0 ? "top" : "bottom");
}

void RemiDockController::openSettings()
{
    setSettingsVisible(true);
}

void RemiDockController::closeSettings()
{
    setSettingsVisible(false);
}

void RemiDockController::toggleSettings()
{
    setSettingsVisible(!m_settingsVisible);
}

void RemiDockController::toggleEditMode()
{
    setEditMode(!m_editMode);
}

void RemiDockController::toggleMusicDance()
{
    setMusicDanceEnabled(!m_musicDanceEnabled);
}

void RemiDockController::save()
{
    m_settings.setValue("Dock/iconSize", m_iconSize);
    m_settings.setValue("Dock/dockHeight", m_dockHeight);
    m_settings.setValue("Dock/autoHide", m_autoHide);
    m_settings.setValue("Dock/edge", m_edge);
    m_settings.setValue("Dock/musicDanceEnabled", m_musicDanceEnabled);
    m_settings.setValue("Dock/dockMode", m_dockMode);
    m_settings.setValue("Dock/hoverAnimation", m_hoverAnimation);
}
