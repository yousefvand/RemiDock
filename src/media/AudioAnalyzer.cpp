#include "AudioAnalyzer.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QStandardPaths>
#include <QtMath>
#include <algorithm>

namespace {
constexpr int SampleRate = 44100;
constexpr int Channels = 1;
constexpr int BytesPerSample = 2;
constexpr int FrameSamples = 1024;
constexpr int FrameBytes = FrameSamples * Channels * BytesPerSample;

qreal clamp01(qreal v)
{
    return std::clamp(v, 0.0, 1.0);
}
}

AudioAnalyzer::AudioAnalyzer(QObject *parent)
    : QObject(parent)
{
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &AudioAnalyzer::readAudio);
    connect(&m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &AudioAnalyzer::handleProcessFinished);

    m_frameTimer.setInterval(16);
    connect(&m_frameTimer, &QTimer::timeout, this, &AudioAnalyzer::processFrame);

    m_restartTimer.setSingleShot(true);
    m_restartTimer.setInterval(1200);
    connect(&m_restartTimer, &QTimer::timeout, this, [this]() {
        if (m_enabled)
            start();
    });
}

AudioAnalyzer::~AudioAnalyzer()
{
    stop();
}

bool AudioAnalyzer::available() const { return m_available; }
bool AudioAnalyzer::running() const { return m_running; }
qreal AudioAnalyzer::level() const { return m_level; }
qreal AudioAnalyzer::beat() const { return m_beat; }
qreal AudioAnalyzer::fast() const { return m_fast; }
qreal AudioAnalyzer::slow() const { return m_slow; }
qreal AudioAnalyzer::pulse() const { return m_pulse; }

void AudioAnalyzer::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;

    if (m_enabled)
        start();
    else
        stop();
}

void AudioAnalyzer::start()
{
    if (m_process.state() != QProcess::NotRunning)
        return;

    const QString parec = QStandardPaths::findExecutable(QStringLiteral("parec"));

    if (parec.isEmpty()) {
        setAvailable(false);
        setRunning(false);
        return;
    }

    m_buffer.clear();

    QStringList args;
    args << QStringLiteral("--device=@DEFAULT_MONITOR@");
    args << QStringLiteral("--format=s16le");
    args << QStringLiteral("--rate=%1").arg(SampleRate);
    args << QStringLiteral("--channels=1");
    args << QStringLiteral("--latency-msec=40");

    m_process.setProgram(parec);
    m_process.setArguments(args);
    m_process.setProcessChannelMode(QProcess::SeparateChannels);
    m_process.start();

    if (!m_process.waitForStarted(500)) {
        setAvailable(false);
        setRunning(false);
        restartLater();
        return;
    }

    setAvailable(true);
    setRunning(true);

    if (!m_frameTimer.isActive())
        m_frameTimer.start();
}

void AudioAnalyzer::stop()
{
    m_restartTimer.stop();
    m_frameTimer.stop();

    if (m_process.state() != QProcess::NotRunning) {
        m_process.kill();
        m_process.waitForFinished(500);
    }

    m_buffer.clear();
    setRunning(false);
    setLevel(0.0);
    setBeat(0.0);
    setFast(0.0);
    setSlow(0.0);
    setPulse(0.0);
}

void AudioAnalyzer::readAudio()
{
    m_buffer.append(m_process.readAllStandardOutput());

    const int maxBytes = FrameBytes * 12;
    if (m_buffer.size() > maxBytes)
        m_buffer.remove(0, m_buffer.size() - maxBytes);
}

void AudioAnalyzer::processFrame()
{
    if (m_buffer.size() < FrameBytes) {
        setLevel(m_level * 0.92);
        setBeat(m_beat * 0.86);
        setPulse(m_pulse * 0.90);
        return;
    }

    const QByteArray frame = m_buffer.left(FrameBytes);
    m_buffer.remove(0, FrameBytes);

    const auto *samples = reinterpret_cast<const qint16 *>(frame.constData());
    const int count = frame.size() / BytesPerSample;

    double sumSquares = 0.0;
    double sumAbsDiff = 0.0;
    qint16 last = samples[0];

    for (int i = 0; i < count; ++i) {
        const double s = samples[i] / 32768.0;
        sumSquares += s * s;
        sumAbsDiff += qAbs(samples[i] - last) / 65536.0;
        last = samples[i];
    }

    const qreal rms = qSqrt(sumSquares / qMax(1, count));
    const qreal transient = clamp01((sumAbsDiff / qMax(1, count)) * 16.0);

    // Slowly learn a floor, then amplify movement above it.
    m_noiseFloor = m_noiseFloor * 0.995 + rms * 0.005;
    const qreal normalized = clamp01((rms - m_noiseFloor * 0.65) * 8.0);

    const qreal previousFast = m_fast;
    const qreal fastValue = m_fast * 0.55 + normalized * 0.45;
    const qreal slowValue = m_slow * 0.92 + normalized * 0.08;
    const qreal beatValue = clamp01((fastValue - slowValue) * 3.2 + transient * 0.65);
    const qreal pulseValue = clamp01(m_pulse * 0.65 + beatValue * 0.35);

    setFast(fastValue);
    setSlow(slowValue);
    setLevel(normalized);
    setBeat(beatValue > previousFast * 0.55 ? beatValue : beatValue * 0.75);
    setPulse(pulseValue);
}

void AudioAnalyzer::handleProcessFinished(int, QProcess::ExitStatus)
{
    setRunning(false);
    setAvailable(false);
    m_frameTimer.stop();
    m_buffer.clear();

    if (m_enabled)
        restartLater();
}

void AudioAnalyzer::restartLater()
{
    if (!m_restartTimer.isActive())
        m_restartTimer.start();
}

void AudioAnalyzer::setAvailable(bool value)
{
    if (m_available == value)
        return;

    m_available = value;
    emit availableChanged();
}

void AudioAnalyzer::setRunning(bool value)
{
    if (m_running == value)
        return;

    m_running = value;
    emit runningChanged();
}

void AudioAnalyzer::setLevel(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(m_level, value))
        return;

    m_level = value;
    emit levelChanged();
}

void AudioAnalyzer::setBeat(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(m_beat, value))
        return;

    m_beat = value;
    emit beatChanged();
}

void AudioAnalyzer::setFast(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(m_fast, value))
        return;

    m_fast = value;
    emit fastChanged();
}

void AudioAnalyzer::setSlow(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(m_slow, value))
        return;

    m_slow = value;
    emit slowChanged();
}

void AudioAnalyzer::setPulse(qreal value)
{
    value = clamp01(value);
    if (qFuzzyCompare(m_pulse, value))
        return;

    m_pulse = value;
    emit pulseChanged();
}
