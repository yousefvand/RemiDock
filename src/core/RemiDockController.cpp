#include "RemiDockController.h"

#include <QtMath>

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QSet>
#include <QStandardPaths>

#include <algorithm>

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
    m_dockFrameVisible = m_settings.value("Dock/frameVisible", false).toBool();
    m_systemIconTheme = detectSystemIconTheme();
    m_iconTheme = m_settings.value("Icons/theme", QStringLiteral("System")).toString().trimmed();

    if (m_iconTheme.isEmpty() || m_iconTheme.compare(QStringLiteral("system"), Qt::CaseInsensitive) == 0)
        m_iconTheme = QStringLiteral("System");

    applyIconTheme();

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
bool RemiDockController::dockFrameVisible() const { return m_dockFrameVisible; }
QString RemiDockController::iconTheme() const { return m_iconTheme; }
int RemiDockController::iconThemeRevision() const { return m_iconThemeRevision; }

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

bool RemiDockController::isSystemIconTheme(const QString &value) const
{
    return value.trimmed().isEmpty()
        || value.compare(QStringLiteral("system"), Qt::CaseInsensitive) == 0;
}

QStringList RemiDockController::iconThemeSearchPaths() const
{
    QStringList iconDirs = QIcon::themeSearchPaths();

    const QStringList dataLocations = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const QString &dataLocation : dataLocations) {
        iconDirs << dataLocation + QStringLiteral("/icons");
    }

    iconDirs << QDir::homePath() + QStringLiteral("/.icons");
    iconDirs << QStringLiteral("/usr/share/icons");
    iconDirs << QStringLiteral("/usr/local/share/icons");

    QStringList cleaned;
    QSet<QString> seen;
    for (const QString &path : iconDirs) {
        const QString absolutePath = QDir(path).absolutePath();
        const QString key = absolutePath.toLower();
        if (!QDir(absolutePath).exists() || seen.contains(key))
            continue;
        seen.insert(key);
        cleaned << absolutePath;
    }

    return cleaned;
}

QString RemiDockController::detectSystemIconTheme() const
{
    const QString qtTheme = QIcon::themeName().trimmed();
    if (!qtTheme.isEmpty() && qtTheme.compare(QStringLiteral("hicolor"), Qt::CaseInsensitive) != 0)
        return qtTheme;

    const QString kdeGlobalsPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation)
        + QStringLiteral("/kdeglobals");

    QSettings kdeGlobals(kdeGlobalsPath, QSettings::IniFormat);
    kdeGlobals.beginGroup(QStringLiteral("Icons"));
    const QString kdeTheme = kdeGlobals.value(QStringLiteral("Theme")).toString().trimmed();
    kdeGlobals.endGroup();

    if (!kdeTheme.isEmpty())
        return kdeTheme;

    return qtTheme.isEmpty() ? QStringLiteral("hicolor") : qtTheme;
}

void RemiDockController::applyIconTheme()
{
    const QStringList searchPaths = iconThemeSearchPaths();
    if (!searchPaths.isEmpty())
        QIcon::setThemeSearchPaths(searchPaths);

    // hicolor is the freedesktop base fallback. Many third-party themes only
    // contain part of the icon set and rely on hicolor/parent themes for the rest.
    QIcon::setFallbackThemeName(QStringLiteral("hicolor"));

    if (isSystemIconTheme(m_iconTheme)) {
        m_systemIconTheme = detectSystemIconTheme();
        if (!m_systemIconTheme.isEmpty())
            QIcon::setThemeName(m_systemIconTheme);
        return;
    }

    QIcon::setThemeName(m_iconTheme);
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


void RemiDockController::setDockFrameVisible(bool value)
{
    if (m_dockFrameVisible == value)
        return;

    m_dockFrameVisible = value;
    save();
    emit dockFrameVisibleChanged();
}

void RemiDockController::setIconTheme(const QString &value)
{
    QString cleaned = value.trimmed();

    if (isSystemIconTheme(cleaned))
        cleaned = QStringLiteral("System");

    if (m_iconTheme == cleaned)
        return;

    m_iconTheme = cleaned;
    applyIconTheme();

    save();
    ++m_iconThemeRevision;
    emit iconThemeChanged();
    emit iconThemeRevisionChanged();
}

QStringList RemiDockController::availableIconThemes() const
{
    const QStringList iconDirs = iconThemeSearchPaths();

    QSet<QString> seen;
    QStringList themes;

    seen.insert(QStringLiteral("system"));

    for (const QString &iconDirPath : iconDirs) {
        QDir iconDir(iconDirPath);

        if (!iconDir.exists())
            continue;

        const QFileInfoList entries = iconDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QFileInfo &entry : entries) {
            const QString name = entry.fileName();
            const QString indexThemePath = entry.absoluteFilePath() + QStringLiteral("/index.theme");

            if (!QFile::exists(indexThemePath))
                continue;

            const QString key = name.toLower();
            if (seen.contains(key))
                continue;

            seen.insert(key);
            themes.append(name);
        }
    }

    themes.sort(Qt::CaseInsensitive);
    themes.prepend(QStringLiteral("System"));
    return themes;
}

int RemiDockController::iconThemeIndex(const QString &theme) const
{
    const QStringList themes = availableIconThemes();
    const QString wanted = isSystemIconTheme(theme) ? QStringLiteral("System") : theme.trimmed();

    for (int i = 0; i < themes.size(); ++i) {
        if (themes.at(i).compare(wanted, Qt::CaseInsensitive) == 0)
            return i;
    }

    return 0;
}

void RemiDockController::showAboutQtDialog()
{
    auto *app = qobject_cast<QApplication *>(QApplication::instance());
    if (app)
        app->setQuitOnLastWindowClosed(false);

    // Show Qt's standard About Qt dialog in the main process.
    // Using a detached helper process can make the dialog appear as a full-screen
    // top-level Wayland window on some desktops and may leave stale state before
    // opening the QML About RemiDock dialog.
    QMessageBox::aboutQt(nullptr, tr("About Qt"));

    if (app)
        app->setQuitOnLastWindowClosed(false);
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
    m_settings.setValue("Dock/frameVisible", m_dockFrameVisible);
    m_settings.setValue("Icons/theme", m_iconTheme);
}
