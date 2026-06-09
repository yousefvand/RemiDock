#pragma once

#include <QObject>
#include <QSettings>

class RemiDockController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int iconSize READ iconSize WRITE setIconSize NOTIFY iconSizeChanged)
    Q_PROPERTY(int dockHeight READ dockHeight WRITE setDockHeight NOTIFY dockHeightChanged)
    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide NOTIFY autoHideChanged)
    Q_PROPERTY(QString edge READ edge WRITE setEdge NOTIFY edgeChanged)
    Q_PROPERTY(bool editMode READ editMode WRITE setEditMode NOTIFY editModeChanged)
    Q_PROPERTY(bool settingsVisible READ settingsVisible WRITE setSettingsVisible NOTIFY settingsVisibleChanged)
    Q_PROPERTY(bool musicDanceEnabled READ musicDanceEnabled WRITE setMusicDanceEnabled NOTIFY musicDanceEnabledChanged)
    Q_PROPERTY(QString dockMode READ dockMode WRITE setDockMode NOTIFY dockModeChanged)
    Q_PROPERTY(QString hoverAnimation READ hoverAnimation WRITE setHoverAnimation NOTIFY hoverAnimationChanged)
    Q_PROPERTY(bool dockFrameVisible READ dockFrameVisible WRITE setDockFrameVisible NOTIFY dockFrameVisibleChanged)

public:
    explicit RemiDockController(QObject *parent = nullptr);

    int iconSize() const;
    int dockHeight() const;
    bool autoHide() const;
    QString edge() const;
    bool editMode() const;
    bool settingsVisible() const;
    bool musicDanceEnabled() const;
    QString dockMode() const;
    QString hoverAnimation() const;
    bool dockFrameVisible() const;

    Q_INVOKABLE void cycleEdge();
    Q_INVOKABLE void setEdgeFromDrag(qreal dx, qreal dy);
    Q_INVOKABLE void openSettings();
    Q_INVOKABLE void closeSettings();
    Q_INVOKABLE void toggleSettings();
    Q_INVOKABLE void toggleEditMode();
    Q_INVOKABLE void toggleMusicDance();

public slots:
    void setIconSize(int value);
    void setDockHeight(int value);
    void setAutoHide(bool value);
    void setEdge(const QString &value);
    void setEditMode(bool value);
    void setSettingsVisible(bool value);
    void setMusicDanceEnabled(bool value);
    void setDockMode(const QString &value);
    void setHoverAnimation(const QString &value);
    void setDockFrameVisible(bool value);
    void save();

signals:
    void iconSizeChanged();
    void dockHeightChanged();
    void autoHideChanged();
    void edgeChanged();
    void editModeChanged();
    void settingsVisibleChanged();
    void musicDanceEnabledChanged();
    void dockModeChanged();
    void hoverAnimationChanged();
    void dockFrameVisibleChanged();
    void layerShellNeedsReconfigure();

private:
    bool isValidEdge(const QString &value) const;
    bool isValidDockMode(const QString &value) const;
    bool isValidHoverAnimation(const QString &value) const;

    QSettings m_settings;
    int m_iconSize = 48;
    int m_dockHeight = 82;
    bool m_autoHide = false;
    QString m_edge = "bottom";
    bool m_editMode = false;
    bool m_settingsVisible = false;
    bool m_musicDanceEnabled = false;
    QString m_dockMode = "always";
    QString m_hoverAnimation = "simple";
    bool m_dockFrameVisible = false;
};
