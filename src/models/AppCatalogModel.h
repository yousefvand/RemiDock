#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

struct CatalogApp
{
    QString name;
    QString iconName;
    QString desktopFile;
    QString exec;
};

class AppCatalogModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        IconNameRole,
        DesktopFileRole,
        ExecRole
    };

    explicit AppCatalogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString searchText() const;
    void setSearchText(const QString &value);

    Q_INVOKABLE void reload();

signals:
    void searchTextChanged();

private:
    void scan();
    void rebuildFiltered();
    CatalogApp readDesktopFile(const QString &path, const QString &desktopFile) const;
    QString cleanExec(QString exec) const;

    QList<CatalogApp> m_allApps;
    QList<CatalogApp> m_filteredApps;
    QString m_searchText;
};
