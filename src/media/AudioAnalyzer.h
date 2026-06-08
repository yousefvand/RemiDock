#pragma once

#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QByteArray>

class AudioAnalyzer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(qreal level READ level NOTIFY levelChanged)
    Q_PROPERTY(qreal beat READ beat NOTIFY beatChanged)
    Q_PROPERTY(qreal fast READ fast NOTIFY fastChanged)
    Q_PROPERTY(qreal slow READ slow NOTIFY slowChanged)
    Q_PROPERTY(qreal pulse READ pulse NOTIFY pulseChanged)

public:
    explicit AudioAnalyzer(QObject *parent = nullptr);
    ~AudioAnalyzer() override;

    bool available() const;
    bool running() const;
    qreal level() const;
    qreal beat() const;
    qreal fast() const;
    qreal slow() const;
    qreal pulse() const;

public slots:
    void setEnabled(bool enabled);
    void start();
    void stop();

signals:
    void availableChanged();
    void runningChanged();
    void levelChanged();
    void beatChanged();
    void fastChanged();
    void slowChanged();
    void pulseChanged();

private slots:
    void readAudio();
    void processFrame();
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void setAvailable(bool value);
    void setRunning(bool value);
    void setLevel(qreal value);
    void setBeat(qreal value);
    void setFast(qreal value);
    void setSlow(qreal value);
    void setPulse(qreal value);
    void restartLater();

    QProcess m_process;
    QByteArray m_buffer;
    QTimer m_frameTimer;
    QTimer m_restartTimer;

    bool m_enabled = false;
    bool m_available = false;
    bool m_running = false;

    qreal m_level = 0.0;
    qreal m_beat = 0.0;
    qreal m_fast = 0.0;
    qreal m_slow = 0.0;
    qreal m_pulse = 0.0;
    qreal m_noiseFloor = 0.02;
};
