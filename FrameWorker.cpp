#include "FrameWorker.h"
#include <QThread>
#include <QElapsedTimer>
#include <cmath>
#include <algorithm>

FrameWorker::FrameWorker(QObject *parent)
    : QObject(parent), m_running(true), m_brightness(0), m_contrast(0), m_filterActive(false) {}

FrameWorker::~FrameWorker() {}

void FrameWorker::stop() {
    m_running = false;
}

void FrameWorker::setBrightness(int val) { m_brightness = val; }
void FrameWorker::setContrast(int val) { m_contrast = val; }
void FrameWorker::setFilterActive(bool active) { m_filterActive = active; }

void FrameWorker::process() {
    const int width = 640;
    const int height = 480;

    QElapsedTimer fpsTimer;
    fpsTimer.start();
    int frameCount = 0;
    float timePhase = 0.0f;

    while (m_running) {
        // Generate raw tissue buffer (RGB888)
        QImage frame(width, height, QImage::Format_RGB888);
        timePhase += 0.1f;

        int b = m_brightness.load();
        int c = m_contrast.load();
        bool filter = m_filterActive.load();

        // Contrast factor math
        double factor = (259.0 * (c + 255.0)) / (255.0 * (259.0 - c));

        // Direct raw pixel manipulation loop
        for (int y = 0; y < height; ++y) {
            uchar *scanline = frame.scanLine(y);

            for (int x = 0; x < width; ++x) {
                // Procedural "tissue" generation (pulsating pink/red gradient)
                int dx = x - width / 2;
                int dy = y - height / 2;
                double dist = std::sqrt(dx * dx + dy * dy);
                double intensity = (std::sin(dist * 0.05 - timePhase) + 1.0) * 0.5; // 0.0 to 1.0

                int rawR = static_cast<int>(200 * intensity) + 55;
                int rawG = static_cast<int>(100 * intensity);
                int rawB = static_cast<int>(100 * intensity);

                // Apply Brightness/Contrast
                rawR = static_cast<int>(factor * (rawR - 128) + 128) + b;
                rawG = static_cast<int>(factor * (rawG - 128) + 128) + b;
                rawB = static_cast<int>(factor * (rawB - 128) + 128) + b;

                // Simple Edge/Threshold Filter (Simulating Boundary detection)
                if (filter && intensity > 0.8) {
                    rawR = 0; rawG = 255; rawB = 0; // highlight boundaries in green
                }

                scanline[x * 3]     = static_cast<uchar>(std::clamp(rawR, 0, 255));
                scanline[x * 3 + 1] = static_cast<uchar>(std::clamp(rawG, 0, 255));
                scanline[x * 3 + 2] = static_cast<uchar>(std::clamp(rawB, 0, 255));
            }
        }

        // Deep copy the frame before sending to UI to prevent cross-thread memory corruption
        emit frameReady(frame.copy());

        frameCount++;
        if (fpsTimer.elapsed() >= 1000) {
            emit fpsUpdate(frameCount);
            frameCount = 0;
            fpsTimer.restart();
        }

        // Throttle to ~30 FPS (33ms per frame)
        QThread::msleep(33);
    }
}