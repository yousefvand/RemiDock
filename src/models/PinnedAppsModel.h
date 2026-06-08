#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QList>
#include <QSettings>
#include <QString>
#include <QStringList>

struct PinnedApp
{
    QString id;
    QString type = "app"; // app or separator
    QString name;
    QString iconName;
    QString desktopFile;
    QString exec;
    bool running = false;
    bool active = false;
};

class PinnedAppsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        NameRole,
        IconNameRole,
        DesktopFileRole,
        ExecRole,
        RunningRole,
        ActiveRole
    };

    explicit PinnedAppsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int itemCount() const;

    Q_INVOKABLE void launch(int index);

    Q_INVOKABLE bool addDesktopFile(const QString &desktopFile);
    Q_INVOKABLE bool addCustomApplication(const QString &executablePath, const QString &iconPath);
    Q_INVOKABLE bool addSeparator();

    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void removeById(const QString &id);

    Q_INVOKABLE void moveItem(int from, int to);
    Q_INVOKABLE void moveUp(const QString &id);
    Q_INVOKABLE void moveDown(const QString &id);
    Q_INVOKABLE void moveToTop(const QString &id);
    Q_INVOKABLE void moveToBottom(const QString &id);

    Q_INVOKABLE bool canFitAdditionalApp(int iconSize, int screenWidth, int screenHeight, const QString &edge) const;
    Q_INVOKABLE bool canFitAdditionalSeparator(int iconSize, int screenWidth, int screenHeight, const QString &edge) const;
    Q_INVOKABLE int estimatedDockExtentWithAdditional(const QString &itemType, int iconSize, const QString &edge) const;
    Q_INVOKABLE int maximumDockExtent(int screenWidth, int screenHeight, const QString &edge) const;

    Q_INVOKABLE void resetDefaults();
    Q_INVOKABLE void refreshRunningState();

signals:
    void itemCountChanged();

private:
    void load();
    void save();

    QString desktopFileKey(const QString &desktopFile) const;
    int indexOfId(const QString &id) const;

    int itemVisualExtent(const PinnedApp &app, int iconSize) const;
    int additionalItemExtent(const QString &itemType, int iconSize) const;
    int estimatedItemsExtent(int iconSize) const;
    bool canFitAdditional(const QString &itemType, int iconSize, int screenWidth, int screenHeight, const QString &edge) const;

    bool containsDesktopFile(const QString &desktopFile) const;
    PinnedApp makeSeparator();
    PinnedApp readDesktopFile(const QString &desktopFile) const;
    PinnedApp fallbackDefaultApp(const QString &desktopFile) const;
    PinnedApp makeCustomApplication(const QString &executablePath, const QString &iconPath) const;
    QString serializeCustomApplication(const PinnedApp &app) const;
    PinnedApp deserializeCustomApplication(const QString &entry) const;
    QString findDesktopFilePath(const QString &desktopFile) const;
    QString cleanExec(QString exec) const;
    QStringList runningDetectionKeys(const PinnedApp &app) const;

    QList<PinnedApp> m_apps;
    QSettings m_settings;
    int m_nextSeparatorId = 1;
};
