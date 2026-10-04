#ifndef FRAMEWORKER_H
#define FRAMEWORKER_H

#pragma once
#include <QObject>
#include <QImage>
#include <atomic>

class FrameWorker : public QObject
{
    Q_OBJECT

public:
    explicit FrameWorker(QObject *parent = nullptr);
    ~FrameWorker();

    void stop();
    void setBrightness(int val);
    void setContrast(int val);
    void setFilterActive(bool active);

public slots:
    void process(); // main generation loop

signals:
    void frameReady(QImage frame);
    void fpsUpdate(int fps);

private:
    std::atomic<bool> m_running;
    std::atomic<int> m_brightness;
    std::atomic<int> m_contrast;
    std::atomic<bool> m_filterActive;
};

#endif // FRAMEWORKER_H
