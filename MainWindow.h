#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QThread>
#include "FrameWorker.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame(const QImage &frame);
    void updateFps(int fps);

private:
    QLabel *m_videoDisplay;
    QLabel *m_fpsLabel;
    QSlider *m_brightSlider;
    QSlider *m_contrastSlider;
    QPushButton *m_filterBtn;

    QThread *m_workerThread;
    FrameWorker *m_worker;
    bool m_filterState = false;
};

#endif // MAINWINDOW_H