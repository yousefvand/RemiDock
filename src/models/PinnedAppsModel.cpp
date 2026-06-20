#include "PinnedAppsModel.h"

#include <QDir>
#include <QFile>
#include <QModelIndex>
#include <QProcess>
#include <QProcessEnvironment>
#include <QSet>
#include <QTimer>
#include <QFileInfo>
#include <QTextStream>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>

namespace {
QString normalizedProcessName(const QString &value)
{
    QString name = QFileInfo(value.trimmed()).fileName().toLower();

    if (name.endsWith(QStringLiteral(".desktop")))
        name.chop(QStringLiteral(".desktop").size());

    if (name.startsWith(QStringLiteral("org.kde.")))
        name = name.mid(QStringLiteral("org.kde.").size());

    if (name.startsWith(QStringLiteral("org."))) {
        const QStringList parts = name.split('.');
        if (!parts.isEmpty())
            name = parts.last();
    }

    return name;
}

struct RunningSnapshot
{
    QSet<QString> names;
    QStringList commandLines;
};

RunningSnapshot collectRunningSnapshot()
{
    RunningSnapshot snapshot;
    QDir proc(QStringLiteral("/proc"));
    const QFileInfoList entries = proc.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &entry : entries) {
        bool ok = false;
        entry.fileName().toInt(&ok);
        if (!ok)
            continue;

        QFile comm(entry.absoluteFilePath() + QStringLiteral("/comm"));
        if (comm.open(QIODevice::ReadOnly | QIODevice::Text)) {
            const QString name = normalizedProcessName(QString::fromUtf8(comm.readAll()).trimmed());
            if (!name.isEmpty())
                snapshot.names.insert(name);
        }

        QFile cmdline(entry.absoluteFilePath() + QStringLiteral("/cmdline"));
        if (cmdline.open(QIODevice::ReadOnly)) {
            const QByteArray raw = cmdline.readAll();
            const QList<QByteArray> parts = raw.split('\0');
            QStringList arguments;

            for (const QByteArray &part : parts) {
                if (!part.isEmpty())
                    arguments << QString::fromUtf8(part);
            }

            if (!arguments.isEmpty()) {
                const QString executableName = normalizedProcessName(arguments.first());
                if (!executableName.isEmpty())
                    snapshot.names.insert(executableName);

                const QString fullLine = arguments.join(QLatin1Char(' ')).toLower();
                if (!fullLine.isEmpty())
                    snapshot.commandLines.append(fullLine);
            }
        }
    }

    return snapshot;
}

bool snapshotMatchesAnyKey(const RunningSnapshot &snapshot, const QStringList &keys)
{
    for (const QString &rawKey : keys) {
        const QString key = normalizedProcessName(rawKey);
        if (key.isEmpty())
            continue;

        if (snapshot.names.contains(key))
            return true;

        for (const QString &line : snapshot.commandLines) {
            if (line.contains(key))
                return true;
        }
    }

    return false;
}

bool startDetachedWithApplicationEnvironment(const QString &program, const QStringList &arguments)
{
    QProcess process;
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();

    /*
     * RemiDock itself is a layer-shell surface.  LayerShellQt enables that by
     * using the Qt Wayland shell-integration environment.  Child Qt/KDE
     * applications must not inherit it, otherwise they can also start as
     * layer-shell clients and appear undecorated/headless.
     */
    environment.remove(QStringLiteral("QT_WAYLAND_SHELL_INTEGRATION"));
    environment.remove(QStringLiteral("QT_QPA_PLATFORMTHEME_LAYER_SHELL"));
    environment.remove(QStringLiteral("LAYER_SHELL_QT_FORCE"));

    process.setProcessEnvironment(environment);
    process.setProgram(program);
    process.setArguments(arguments);
    return process.startDetached();
}

constexpr int kHandleSize = 28;
constexpr int kOuterPadding = 10;
constexpr int kHandleGap = 8;
constexpr int kItemSpacing = 8;
constexpr int kScreenEdgePadding = 48;

struct DesktopActionInfo
{
    QString id;
    QString name;
    QString icon;
    QString exec;
};

QString desktopEntryValue(const QString &line, const QString &key)
{
    const QString prefix = key + QStringLiteral("=");
    if (!line.startsWith(prefix))
        return {};
    return line.mid(prefix.size()).trimmed();
}

QList<DesktopActionInfo> readDesktopActionsFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QStringList actionOrder;
    QHash<QString, DesktopActionInfo> actions;
    QString currentActionId;
    bool inDesktopEntry = false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();

        if (line.isEmpty() || line.startsWith(QLatin1Char('#')))
            continue;

        if (line.startsWith(QLatin1Char('[')) && line.endsWith(QLatin1Char(']'))) {
            inDesktopEntry = (line == QStringLiteral("[Desktop Entry]"));
            currentActionId.clear();

            const QString prefix = QStringLiteral("[Desktop Action ");
            if (line.startsWith(prefix)) {
                currentActionId = line.mid(prefix.size(), line.size() - prefix.size() - 1).trimmed();
                if (!currentActionId.isEmpty()) {
                    DesktopActionInfo info;
                    info.id = currentActionId;
                    actions.insert(currentActionId, info);
                }
            }
            continue;
        }

        if (inDesktopEntry) {
            const QString value = desktopEntryValue(line, QStringLiteral("Actions"));
            if (!value.isEmpty()) {
                const QStringList ids = value.split(QLatin1Char(';'), Qt::SkipEmptyParts);
                for (const QString &rawId : ids) {
                    const QString id = rawId.trimmed();
                    if (!id.isEmpty() && !actionOrder.contains(id))
                        actionOrder << id;
                }
            }
            continue;
        }

        if (currentActionId.isEmpty())
            continue;

        DesktopActionInfo info = actions.value(currentActionId);
        if (line.startsWith(QStringLiteral("Name=")))
            info.name = line.mid(QStringLiteral("Name=").size()).trimmed();
        else if (line.startsWith(QStringLiteral("Icon=")))
            info.icon = line.mid(QStringLiteral("Icon=").size()).trimmed();
        else if (line.startsWith(QStringLiteral("Exec=")))
            info.exec = line.mid(QStringLiteral("Exec=").size()).trimmed();
        actions.insert(currentActionId, info);
    }

    QList<DesktopActionInfo> result;
    for (const QString &id : actionOrder) {
        const DesktopActionInfo info = actions.value(id);
        if (!info.id.isEmpty() && !info.name.isEmpty() && !info.exec.isEmpty())
            result.append(info);
    }

    return result;
}

}

PinnedAppsModel::PinnedAppsModel(QObject *parent)
    : QAbstractListModel(parent),
      m_settings("remisa", "RemiDock")
{
    load();
    refreshRunningState();

    auto *timer = new QTimer(this);
    timer->setInterval(1500);
    connect(timer, &QTimer::timeout, this, &PinnedAppsModel::refreshRunningState);
    timer->start();
}

int PinnedAppsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_apps.size();
}

QVariant PinnedAppsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.row() < 0 || index.row() >= m_apps.size())
        return {};

    const PinnedApp &app = m_apps.at(index.row());

    switch (role) {
    case IdRole:
        return app.id;
    case TypeRole:
        return app.type;
    case NameRole:
        return app.name;
    case IconNameRole:
        return app.iconName;
    case DesktopFileRole:
        return app.desktopFile;
    case ExecRole:
        return app.exec;
    case RunningRole:
        return app.running;
    case ActiveRole:
        return app.active;
    default:
        return {};
    }
}

QHash<int, QByteArray> PinnedAppsModel::roleNames() const
{
    return {
        {IdRole, "itemId"},
        {TypeRole, "itemType"},
        {NameRole, "name"},
        {IconNameRole, "iconName"},
        {DesktopFileRole, "desktopFile"},
        {ExecRole, "exec"},
        {RunningRole, "running"},
        {ActiveRole, "active"}
    };
}

int PinnedAppsModel::itemCount() const
{
    return m_apps.size();
}

void PinnedAppsModel::launch(int index)
{
    if (index < 0 || index >= m_apps.size())
        return;

    PinnedApp &app = m_apps[index];

    if (app.type != "app")
        return;

    bool started = false;

    if (app.desktopFile.startsWith(QStringLiteral("custom:"))) {
        started = startDetachedWithApplicationEnvironment(app.exec, {});
    } else {
        const QString command = cleanExec(app.exec);
        if (!command.isEmpty())
            started = startDetachedWithApplicationEnvironment(QStringLiteral("/bin/sh"), {QStringLiteral("-lc"), command});
    }

    /*
     * Process scanning may need a second or two before the launched program
     * appears under /proc. Mark it as running immediately after a successful
     * launch so the dock indicator is visible right away; the timer will
     * correct it later if the process exits quickly.
     */
    if (started && !app.running) {
        app.running = true;
        const QModelIndex changedIndex = this->index(index);
        emit dataChanged(changedIndex, changedIndex, {RunningRole, ActiveRole});
    }

    QTimer::singleShot(1200, this, &PinnedAppsModel::refreshRunningState);
}

QVariantList PinnedAppsModel::desktopActions(int index) const
{
    QVariantList list;

    if (index < 0 || index >= m_apps.size())
        return list;

    const PinnedApp &app = m_apps.at(index);
    if (app.type != QStringLiteral("app") || app.desktopFile.startsWith(QStringLiteral("custom:")))
        return list;

    const QString path = findDesktopFilePath(app.desktopFile);
    if (path.isEmpty())
        return list;

    const QList<DesktopActionInfo> actions = readDesktopActionsFromFile(path);
    for (const DesktopActionInfo &action : actions) {
        QVariantMap entry;
        entry.insert(QStringLiteral("id"), action.id);
        entry.insert(QStringLiteral("name"), action.name);
        entry.insert(QStringLiteral("icon"), action.icon);
        list.append(entry);
    }

    return list;
}

void PinnedAppsModel::launchDesktopAction(int index, const QString &actionId)
{
    if (index < 0 || index >= m_apps.size())
        return;

    const PinnedApp &app = m_apps.at(index);
    if (app.type != QStringLiteral("app") || app.desktopFile.startsWith(QStringLiteral("custom:")))
        return;

    const QString command = cleanExec(commandForDesktopAction(app.desktopFile, actionId));
    if (command.isEmpty())
        return;

    startDetachedWithApplicationEnvironment(QStringLiteral("/bin/sh"), {QStringLiteral("-lc"), command});
    QTimer::singleShot(1200, this, &PinnedAppsModel::refreshRunningState);
}


bool PinnedAppsModel::addDesktopFile(const QString &desktopFile)
{
    const QString cleanDesktopFile = desktopFile.trimmed();

    if (cleanDesktopFile.isEmpty())
        return false;

    if (containsDesktopFile(cleanDesktopFile))
        return false;

    const PinnedApp app = readDesktopFile(cleanDesktopFile);

    if (app.desktopFile.isEmpty() || app.exec.isEmpty())
        return false;

    beginInsertRows(QModelIndex(), m_apps.size(), m_apps.size());
    m_apps.append(app);
    endInsertRows();

    save();
    emit itemCountChanged();
    return true;
}

bool PinnedAppsModel::addCustomApplication(const QString &executablePath, const QString &iconPath)
{
    const PinnedApp app = makeCustomApplication(executablePath, iconPath);

    if (app.exec.isEmpty())
        return false;

    for (const PinnedApp &existing : m_apps) {
        if (existing.type == QStringLiteral("app") && existing.exec == app.exec)
            return false;
    }

    beginInsertRows(QModelIndex(), m_apps.size(), m_apps.size());
    m_apps.append(app);
    endInsertRows();

    save();
    emit itemCountChanged();
    return true;
}

bool PinnedAppsModel::addSeparator()
{
    beginInsertRows(QModelIndex(), m_apps.size(), m_apps.size());
    m_apps.append(makeSeparator());
    endInsertRows();

    save();
    emit itemCountChanged();
    return true;
}

void PinnedAppsModel::removeItem(int index)
{
    if (index < 0 || index >= m_apps.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_apps.removeAt(index);
    endRemoveRows();

    save();
    emit itemCountChanged();
}

void PinnedAppsModel::removeById(const QString &id)
{
    removeItem(indexOfId(id));
}

void PinnedAppsModel::moveItem(int from, int to)
{
    if (from < 0 || from >= m_apps.size())
        return;

    if (to < 0 || to >= m_apps.size())
        return;

    if (from == to)
        return;

    const int destinationChild = (to > from) ? to + 1 : to;

    if (!beginMoveRows(QModelIndex(), from, from, QModelIndex(), destinationChild))
        return;

    m_apps.move(from, to);
    endMoveRows();

    save();
    emit itemCountChanged();
}

void PinnedAppsModel::moveUp(const QString &id)
{
    const int from = indexOfId(id);
    if (from <= 0)
        return;
    moveItem(from, from - 1);
}

void PinnedAppsModel::moveDown(const QString &id)
{
    const int from = indexOfId(id);
    if (from < 0 || from >= m_apps.size() - 1)
        return;
    moveItem(from, from + 1);
}

void PinnedAppsModel::moveToTop(const QString &id)
{
    const int from = indexOfId(id);
    if (from <= 0)
        return;
    moveItem(from, 0);
}

void PinnedAppsModel::moveToBottom(const QString &id)
{
    const int from = indexOfId(id);
    if (from < 0 || from >= m_apps.size() - 1)
        return;
    moveItem(from, m_apps.size() - 1);
}


int PinnedAppsModel::indexOfItem(const QString &id) const
{
    return indexOfId(id);
}

void PinnedAppsModel::refreshRunningState()
{
    const RunningSnapshot snapshot = collectRunningSnapshot();

    for (int i = 0; i < m_apps.size(); ++i) {
        PinnedApp &app = m_apps[i];

        bool newRunning = false;

        if (app.type == QStringLiteral("app")) {
            newRunning = snapshotMatchesAnyKey(snapshot, runningDetectionKeys(app));
        }

        const bool newActive = false;

        if (app.running == newRunning && app.active == newActive)
            continue;

        app.running = newRunning;
        app.active = newActive;

        const QModelIndex changedIndex = index(i);
        emit dataChanged(changedIndex, changedIndex, {RunningRole, ActiveRole});
    }
}

QString PinnedAppsModel::desktopFileKey(const QString &desktopFile) const
{
    QString key = desktopFile;
    key = key.section('/', -1);
    if (key.endsWith(".desktop"))
        key.chop(QStringLiteral(".desktop").size());
    return key.toLower();
}

bool PinnedAppsModel::canFitAdditionalApp(int iconSize, int screenWidth, int screenHeight, const QString &edge) const
{
    return canFitAdditional("app", iconSize, screenWidth, screenHeight, edge);
}

bool PinnedAppsModel::canFitAdditionalSeparator(int iconSize, int screenWidth, int screenHeight, const QString &edge) const
{
    return canFitAdditional("separator", iconSize, screenWidth, screenHeight, edge);
}

int PinnedAppsModel::estimatedDockExtentWithAdditional(const QString &itemType, int iconSize, const QString &edge) const
{
    Q_UNUSED(edge)

    int extent = estimatedItemsExtent(iconSize);

    if (!m_apps.isEmpty())
        extent += kItemSpacing;

    extent += additionalItemExtent(itemType, iconSize);

    return extent + kHandleSize * 2 + kHandleGap * 2 + kOuterPadding * 2;
}

int PinnedAppsModel::maximumDockExtent(int screenWidth, int screenHeight, const QString &edge) const
{
    const bool vertical = edge == "left" || edge == "right";
    const int available = vertical ? screenHeight : screenWidth;
    return qMax(220, available - kScreenEdgePadding);
}

bool PinnedAppsModel::canFitAdditional(const QString &itemType, int iconSize, int screenWidth, int screenHeight, const QString &edge) const
{
    return estimatedDockExtentWithAdditional(itemType, iconSize, edge) <= maximumDockExtent(screenWidth, screenHeight, edge);
}

void PinnedAppsModel::resetDefaults()
{
    m_settings.remove("PinnedApps/items");
    m_settings.remove("PinnedApps/schemaVersion");
    load();
}

void PinnedAppsModel::load()
{
    // Start with an empty dock by default.
    // Users can add installed applications, custom applications, and separators
    // from the settings window.
    const QStringList defaults = {};

    QStringList items;
    const int schemaVersion = m_settings.value("PinnedApps/schemaVersion", 0).toInt();

    if (schemaVersion < 7) {
        items = defaults;
        m_settings.setValue("PinnedApps/schemaVersion", 7);
    } else {
        items = m_settings.value("PinnedApps/items", defaults).toStringList();
    }

    beginResetModel();
    m_apps.clear();
    m_nextSeparatorId = 1;

    for (const QString &entry : items) {
        if (entry == "separator" || entry.startsWith("separator:")) {
            m_apps.append(makeSeparator());
            continue;
        }

        if (entry.startsWith(QStringLiteral("custom\t"))) {
            const PinnedApp customApp = deserializeCustomApplication(entry);
            if (!customApp.exec.isEmpty())
                m_apps.append(customApp);
            continue;
        }

        const PinnedApp app = readDesktopFile(entry);

        if (!app.desktopFile.isEmpty())
            m_apps.append(app);
    }

    endResetModel();
    emit itemCountChanged();
}

void PinnedAppsModel::save()
{
    QStringList items;

    for (const PinnedApp &app : m_apps) {
        if (app.type == "separator")
            items << "separator";
        else if (app.desktopFile.startsWith(QStringLiteral("custom:")))
            items << serializeCustomApplication(app);
        else
            items << app.desktopFile;
    }

    m_settings.setValue("PinnedApps/items", items);
}

int PinnedAppsModel::indexOfId(const QString &id) const
{
    for (int i = 0; i < m_apps.size(); ++i) {
        if (m_apps.at(i).id == id)
            return i;
    }

    return -1;
}

int PinnedAppsModel::itemVisualExtent(const PinnedApp &app, int iconSize) const
{
    return app.type == "separator" ? 12 : iconSize + 16;
}

int PinnedAppsModel::additionalItemExtent(const QString &itemType, int iconSize) const
{
    return itemType == "separator" ? 12 : iconSize + 16;
}

int PinnedAppsModel::estimatedItemsExtent(int iconSize) const
{
    if (m_apps.isEmpty())
        return 0;

    int extent = 0;

    for (const PinnedApp &app : m_apps)
        extent += itemVisualExtent(app, iconSize);

    extent += kItemSpacing * (m_apps.size() - 1);

    return extent;
}

bool PinnedAppsModel::containsDesktopFile(const QString &desktopFile) const
{
    for (const PinnedApp &app : m_apps) {
        if (app.type == "app" && app.desktopFile == desktopFile)
            return true;
    }

    return false;
}

PinnedApp PinnedAppsModel::makeSeparator()
{
    PinnedApp app;
    app.id = QStringLiteral("separator-%1").arg(m_nextSeparatorId++);
    app.type = "separator";
    app.name = "Separator";
    return app;
}

PinnedApp PinnedAppsModel::readDesktopFile(const QString &desktopFile) const
{
    PinnedApp app;
    app.id = desktopFile;
    app.type = "app";
    app.desktopFile = desktopFile;

    const QString path = findDesktopFilePath(desktopFile);

    if (path.isEmpty())
        return fallbackDefaultApp(desktopFile);

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QTextStream in(&file);
    bool inDesktopEntry = false;
    bool noDisplay = false;
    bool hidden = false;

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();

        if (line == "[Desktop Entry]") {
            inDesktopEntry = true;
            continue;
        }

        if (line.startsWith("[") && line.endsWith("]") && line != "[Desktop Entry]") {
            inDesktopEntry = false;
            continue;
        }

        if (!inDesktopEntry)
            continue;

        if (line.startsWith("Name=") && app.name.isEmpty())
            app.name = line.mid(QString("Name=").length());

        if (line.startsWith("Icon=") && app.iconName.isEmpty())
            app.iconName = line.mid(QString("Icon=").length());

        if (line.startsWith("Exec=") && app.exec.isEmpty())
            app.exec = cleanExec(line.mid(QString("Exec=").length()));

        if (line == "NoDisplay=true")
            noDisplay = true;

        if (line == "Hidden=true")
            hidden = true;
    }

    if (noDisplay || hidden)
        return {};

    if (app.name.isEmpty())
        app.name = desktopFile;

    if (app.iconName.isEmpty())
        app.iconName = "application-x-executable";

    return app;
}

PinnedApp PinnedAppsModel::fallbackDefaultApp(const QString &desktopFile) const
{
    struct FallbackInfo {
        const char *desktopFile;
        const char *name;
        const char *iconName;
    };

    static const FallbackInfo fallbackApps[] = {
        {"org.kde.dolphin.desktop", "Dolphin", "org.kde.dolphin"},
        {"org.kde.konsole.desktop", "Konsole", "utilities-terminal"},
        {"org.kde.kate.desktop", "Kate", "org.kde.kate"},
        {"firefox.desktop", "Firefox", "firefox"},
        {"chromium.desktop", "Chromium", "chromium"},
        {"code.desktop", "Visual Studio Code", "visual-studio-code"},
        {"org.kde.systemsettings.desktop", "System Settings", "systemsettings"},
        {"org.kde.discover.desktop", "Discover", "plasmadiscover"},
        {"org.kde.gwenview.desktop", "Gwenview", "org.kde.gwenview"},
        {"org.kde.okular.desktop", "Okular", "okular"},
        {"org.kde.spectacle.desktop", "Spectacle", "spectacle"},
        {"vlc.desktop", "VLC", "vlc"}
    };

    for (const FallbackInfo &info : fallbackApps) {
        if (desktopFile == QString::fromLatin1(info.desktopFile)) {
            PinnedApp app;
            app.id = desktopFile;
            app.type = "app";
            app.name = QString::fromLatin1(info.name);
            app.iconName = QString::fromLatin1(info.iconName);
            app.desktopFile = desktopFile;
            app.exec = "";
            return app;
        }
    }

    return {};
}

QStringList PinnedAppsModel::runningDetectionKeys(const PinnedApp &app) const
{
    QSet<QString> keys;

    const QString desktopKey = desktopFileKey(app.desktopFile);
    if (!desktopKey.isEmpty()) {
        keys.insert(normalizedProcessName(desktopKey));
        const QStringList parts = desktopKey.split('.');
        if (!parts.isEmpty())
            keys.insert(normalizedProcessName(parts.last()));
    }

    const QStringList commandParts = QProcess::splitCommand(app.exec);
    if (!commandParts.isEmpty())
        keys.insert(normalizedProcessName(commandParts.first()));

    if (!app.name.isEmpty()) {
        keys.insert(normalizedProcessName(app.name));
        keys.insert(app.name.toLower().remove(QLatin1Char(' ')));
    }

    if (app.desktopFile == QStringLiteral("org.kde.systemsettings.desktop"))
        keys.insert(QStringLiteral("systemsettings"));
    if (app.desktopFile == QStringLiteral("org.kde.discover.desktop"))
        keys.insert(QStringLiteral("plasma-discover"));
    if (app.desktopFile == QStringLiteral("code.desktop"))
        keys.insert(QStringLiteral("code"));
    if (app.desktopFile == QStringLiteral("firefox.desktop"))
        keys.insert(QStringLiteral("firefox"));
    if (app.desktopFile == QStringLiteral("org.kde.dolphin.desktop"))
        keys.insert(QStringLiteral("dolphin"));
    if (app.desktopFile == QStringLiteral("org.kde.konsole.desktop"))
        keys.insert(QStringLiteral("konsole"));
    if (app.desktopFile == QStringLiteral("org.kde.kate.desktop"))
        keys.insert(QStringLiteral("kate"));
    if (app.desktopFile == QStringLiteral("org.kde.okular.desktop"))
        keys.insert(QStringLiteral("okular"));
    if (app.desktopFile == QStringLiteral("vlc.desktop"))
        keys.insert(QStringLiteral("vlc"));

    if (app.desktopFile == QStringLiteral("google-chrome.desktop"))
        keys.insert(QStringLiteral("chrome"));
    if (app.desktopFile == QStringLiteral("brave-browser.desktop"))
        keys.insert(QStringLiteral("brave"));
    if (app.desktopFile == QStringLiteral("org.kde.kwrite.desktop"))
        keys.insert(QStringLiteral("kwrite"));
    if (app.desktopFile == QStringLiteral("org.kde.gwenview.desktop"))
        keys.insert(QStringLiteral("gwenview"));
    if (app.desktopFile == QStringLiteral("org.kde.spectacle.desktop"))
        keys.insert(QStringLiteral("spectacle"));
    if (app.desktopFile == QStringLiteral("org.kde.plasma-systemmonitor.desktop"))
        keys.insert(QStringLiteral("plasma-systemmonitor"));

    keys.remove(QString());
    return QStringList(keys.begin(), keys.end());
}

PinnedApp PinnedAppsModel::makeCustomApplication(const QString &executablePath, const QString &iconPath) const
{
    QString exec = executablePath.trimmed();
    QString icon = iconPath.trimmed();

    if (exec.startsWith(QStringLiteral("file://"))) {
        const QUrl url(exec);
        if (url.isLocalFile())
            exec = url.toLocalFile();
    }

    if (icon.startsWith(QStringLiteral("file://"))) {
        const QUrl url(icon);
        if (url.isLocalFile())
            icon = url.toLocalFile();
    }

    QFileInfo execInfo(exec);
    if (!execInfo.exists() || !execInfo.isFile() || !execInfo.isExecutable())
        return {};

    PinnedApp app;
    app.id = QStringLiteral("custom:%1").arg(execInfo.absoluteFilePath());
    app.type = QStringLiteral("app");
    app.name = execInfo.completeBaseName().isEmpty() ? execInfo.fileName() : execInfo.completeBaseName();
    app.iconName = icon.isEmpty() ? QStringLiteral("application-x-executable") : icon;
    app.desktopFile = app.id;
    app.exec = execInfo.absoluteFilePath();
    return app;
}

QString PinnedAppsModel::serializeCustomApplication(const PinnedApp &app) const
{
    return QStringLiteral("custom\t%1\t%2\t%3")
        .arg(app.name, app.exec, app.iconName);
}

PinnedApp PinnedAppsModel::deserializeCustomApplication(const QString &entry) const
{
    const QStringList parts = entry.split(QLatin1Char('\t'));
    if (parts.size() < 3)
        return {};

    const QString name = parts.value(1);
    const QString exec = parts.value(2);
    const QString icon = parts.value(3);

    PinnedApp app = makeCustomApplication(exec, icon);
    if (!app.exec.isEmpty() && !name.isEmpty())
        app.name = name;
    return app;
}

QString PinnedAppsModel::findDesktopFilePath(const QString &desktopFile) const
{
    const QStringList dirs = {
        QDir::homePath() + "/.local/share/applications",
        "/usr/local/share/applications",
        "/usr/share/applications",
        "/var/lib/flatpak/exports/share/applications",
        QDir::homePath() + "/.local/share/flatpak/exports/share/applications"
    };

    for (const QString &dir : dirs) {
        const QString path = dir + "/" + desktopFile;

        if (QFile::exists(path))
            return path;
    }

    return {};
}


QString PinnedAppsModel::commandForDesktopAction(const QString &desktopFile, const QString &actionId) const
{
    const QString path = findDesktopFilePath(desktopFile);
    if (path.isEmpty() || actionId.trimmed().isEmpty())
        return {};

    const QList<DesktopActionInfo> actions = readDesktopActionsFromFile(path);
    for (const DesktopActionInfo &action : actions) {
        if (action.id == actionId)
            return action.exec;
    }

    return {};
}

QString PinnedAppsModel::cleanExec(QString exec) const
{
    exec.remove("%f");
    exec.remove("%F");
    exec.remove("%u");
    exec.remove("%U");
    exec.remove("%i");
    exec.remove("%c");
    exec.remove("%k");

    return exec.trimmed();
}
