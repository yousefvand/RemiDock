#include "AppCatalogModel.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <algorithm>

AppCatalogModel::AppCatalogModel(QObject *parent)
    : QAbstractListModel(parent)
{
    scan();
    rebuildFiltered();
}

int AppCatalogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_filteredApps.size();
}

QVariant AppCatalogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_filteredApps.size())
        return {};

    const auto &app = m_filteredApps[index.row()];

    switch (role) {
    case NameRole:
        return app.name;
    case IconNameRole:
        return app.iconName;
    case DesktopFileRole:
        return app.desktopFile;
    case ExecRole:
        return app.exec;
    default:
        return {};
    }
}

QHash<int, QByteArray> AppCatalogModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {IconNameRole, "iconName"},
        {DesktopFileRole, "desktopFile"},
        {ExecRole, "exec"}
    };
}

QString AppCatalogModel::searchText() const
{
    return m_searchText;
}

void AppCatalogModel::setSearchText(const QString &value)
{
    if (m_searchText == value)
        return;

    m_searchText = value;
    emit searchTextChanged();

    rebuildFiltered();
}

void AppCatalogModel::reload()
{
    scan();
    rebuildFiltered();
}

void AppCatalogModel::scan()
{
    QList<CatalogApp> apps;
    QSet<QString> seen;

    QStringList dirs = {
        QDir::homePath() + "/.local/share/applications",
        "/usr/local/share/applications",
        "/usr/share/applications",
        "/var/lib/flatpak/exports/share/applications",
        QDir::homePath() + "/.local/share/flatpak/exports/share/applications"
    };

    for (const QString &dirPath : dirs) {
        QDir dir(dirPath);

        if (!dir.exists())
            continue;

        const auto files = dir.entryInfoList({"*.desktop"}, QDir::Files);

        for (const QFileInfo &fileInfo : files) {
            const QString desktopFile = fileInfo.fileName();

            if (seen.contains(desktopFile))
                continue;

            CatalogApp app = readDesktopFile(fileInfo.absoluteFilePath(), desktopFile);

            if (app.name.isEmpty() || app.exec.isEmpty())
                continue;

            seen.insert(desktopFile);
            apps.append(app);
        }
    }

    std::sort(apps.begin(), apps.end(), [](const CatalogApp &a, const CatalogApp &b) {
        return a.name.toLower() < b.name.toLower();
    });

    m_allApps = apps;
}

void AppCatalogModel::rebuildFiltered()
{
    beginResetModel();

    m_filteredApps.clear();

    const QString query = m_searchText.trimmed().toLower();

    for (const CatalogApp &app : m_allApps) {
        if (query.isEmpty()
            || app.name.toLower().contains(query)
            || app.desktopFile.toLower().contains(query)) {
            m_filteredApps.append(app);

            if (m_filteredApps.size() >= 80)
                break;
        }
    }

    endResetModel();
}

CatalogApp AppCatalogModel::readDesktopFile(const QString &path, const QString &desktopFile) const
{
    CatalogApp app;
    app.desktopFile = desktopFile;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    QTextStream in(&file);
    bool inDesktopEntry = false;
    bool noDisplay = false;
    bool hidden = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

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

    if (app.iconName.isEmpty())
        app.iconName = "application-x-executable";

    return app;
}

QString AppCatalogModel::cleanExec(QString exec) const
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
