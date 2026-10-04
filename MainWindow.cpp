#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    m_videoDisplay = new QLabel("Initializing Camera...", this);
    m_videoDisplay->setMinimumSize(640, 480);
    m_videoDisplay->setAlignment(Qt::AlignCenter);
    m_videoDisplay->setStyleSheet("background-color: black; color: white;");

    m_fpsLabel = new QLabel("FPS: 0", this);

    auto *controlLayout = new QHBoxLayout();
    m_brightSlider = new QSlider(Qt::Horizontal, this);
    m_brightSlider->setRange(-100, 100);

    m_contrastSlider = new QSlider(Qt::Horizontal, this);
    m_contrastSlider->setRange(-100, 100);

    m_filterBtn = new QPushButton("Toggle Boundary Filter", this);

    controlLayout->addWidget(new QLabel("Brightness:", this));
    controlLayout->addWidget(m_brightSlider);
    controlLayout->addWidget(new QLabel("Contrast:", this));
    controlLayout->addWidget(m_contrastSlider);
    controlLayout->addWidget(m_filterBtn);
    controlLayout->addWidget(m_fpsLabel);

    mainLayout->addWidget(m_videoDisplay);
    mainLayout->addLayout(controlLayout);
    setCentralWidget(central);


    // --- ARCHITECTURE: Thread Setup ---
    m_workerThread = new QThread(this);
    m_worker = new FrameWorker();

    // Move the worker object to the background thread
    m_worker->moveToThread(m_workerThread);

    // Wire up the signals and slots safely across threads
    connect(m_workerThread, &QThread::started, m_worker, &FrameWorker::process);
    connect(m_worker, &FrameWorker::frameReady, this, &MainWindow::updateFrame);
    connect(m_worker, &FrameWorker::fpsUpdate, this, &MainWindow::updateFps);

    // Connect UI controls to the worker
    connect(m_brightSlider, &QSlider::valueChanged, this, [this](int v){ m_worker->setBrightness(v); });
    connect(m_contrastSlider, &QSlider::valueChanged, this, [this](int v){ m_worker->setContrast(v); });
    connect(m_filterBtn, &QPushButton::clicked, this, [this](){
        m_filterState = !m_filterState;
        m_worker->setFilterActive(m_filterState);
    });

    // Start thread
    m_workerThread->start();
}

MainWindow::~MainWindow() {
    // graceful teardown
    if (m_workerThread->isRunning()) {
        m_worker->stop();
        m_workerThread->quit();
        m_workerThread->wait(); // wait for thread to cleanly exit
    }
    delete m_worker; // delte worker manually (no parent)
}

void MainWindow::updateFrame(const QImage &frame) {
    m_videoDisplay->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::updateFps(int fps) {
    m_fpsLabel->setText(QString("FPS: %1").arg(fps));
}