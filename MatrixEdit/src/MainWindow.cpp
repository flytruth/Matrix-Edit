#include "MainWindow.h"
#include "ThemeManager.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QMessageBox>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isPlaying(false), m_currentTheme(ThemeManager::Dark) {
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
    loadStylesheet();

    connect(m_trimmer, &VideoTrimmer::inPointChanged, this, &MainWindow::onTrimInChanged);
    connect(m_trimmer, &VideoTrimmer::outPointChanged, this, &MainWindow::onTrimOutChanged);
    connect(m_trimmer, &VideoTrimmer::playheadMoved, this, &MainWindow::onPlayheadMoved);

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](ThemeManager::Theme theme) {
        m_currentTheme = theme;
        loadStylesheet();
    });

    setWindowTitle("MatrixEdit - Video Editor");
    resize(1400, 900);
}

MainWindow::~MainWindow() {
    delete m_mediaPlayer;
    delete m_audioOutput;
}

void MainWindow::setupUI() {
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_videoWidget = new QVideoWidget(this);
    m_mediaPlayer->setVideoOutput(m_videoWidget);

    // Optimize for performance
    m_videoWidget->setAttribute(Qt::WA_OpaquePaintEvent);
    m_videoWidget->setAttribute(Qt::WA_PaintOnScreen);
    m_audioOutput->setVolume(0.5);

    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        m_trimmer->setPlayheadPosition(position);
        int seconds = static_cast<int>(position / 1000);
        int minutes = seconds / 60;
        seconds %= 60;
        m_timeLabel->setText(QString("%1:%2").arg(minutes, 2, 'f', 0, '0').arg(seconds, 2, 'f', 0, '0'));
    });

    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        m_trimmer->setDuration(duration);
        m_trimmer->setTrimRange(0, duration);
        m_seekSlider->setMaximum(duration);
        generateThumbnails();
    });
}

void MainWindow::setupMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *openAction = new QAction("&Open", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    fileMenu->addAction(openAction);

    QAction *exportAction = new QAction("&Export", this);
    exportAction->setShortcut(QKeySequence::SaveAs);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExport);
    fileMenu->addAction(exportAction);

    fileMenu->addSeparator();
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);

    QMenu *editMenu = menuBar->addMenu("&Edit");
    QAction *cutAction = new QAction("Cu&t", this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, this, &MainWindow::onCut);
    editMenu->addAction(cutAction);

    QAction *undoAction = new QAction("&Undo", this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    editMenu->addAction(undoAction);

    QAction *redoAction = new QAction("&Redo", this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);
    editMenu->addAction(redoAction);

    QMenu *addMenu = menuBar->addMenu("&Add");
    QAction *textAction = new QAction("&Text", this);
    connect(textAction, &QAction::triggered, this, &MainWindow::onAddText);
    addMenu->addAction(textAction);

    QAction *audioAction = new QAction("&Audio", this);
    connect(audioAction, &QAction::triggered, this, &MainWindow::onAddAudio);
    addMenu->addAction(audioAction);

    QMenu *viewMenu = menuBar->addMenu("&View");
    QAction *themeAction = new QAction("Toggle &Theme", this);
    themeAction->setShortcut(QKeySequence("Ctrl+Shift+T"));
    connect(themeAction, &QAction::triggered, this, &MainWindow::onToggleTheme);
    viewMenu->addAction(themeAction);

    setMenuBar(menuBar);
}

void MainWindow::setupToolBar() {
    m_toolbar = new QToolBar("Main Toolbar", this);
    m_toolbar->setMovable(false);

    m_openBtn = new QPushButton("Open", this);
    m_openBtn->setToolTip("Open video file (Ctrl+O)");
    connect(m_openBtn, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    m_toolbar->addWidget(m_openBtn);

    m_toolbar->addSeparator();

    m_playPauseBtn = new QPushButton("Play", this);
    m_playPauseBtn->setMinimumWidth(80);
    m_playPauseBtn->setToolTip("Play/Pause (Space)");
    connect(m_playPauseBtn, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    m_toolbar->addWidget(m_playPauseBtn);

    m_cutBtn = new QPushButton("Cut", this);
    m_cutBtn->setToolTip("Cut at playhead (Ctrl+X)");
    connect(m_cutBtn, &QPushButton::clicked, this, &MainWindow::onCut);
    m_toolbar->addWidget(m_cutBtn);

    m_undoBtn = new QPushButton("Undo", this);
    m_undoBtn->setToolTip("Undo (Ctrl+Z)");
    connect(m_undoBtn, &QPushButton::clicked, this, &MainWindow::onUndo);
    m_toolbar->addWidget(m_undoBtn);

    m_redoBtn = new QPushButton("Redo", this);
    m_redoBtn->setToolTip("Redo (Ctrl+Y)");
    connect(m_redoBtn, &QPushButton::clicked, this, &MainWindow::onRedo);
    m_toolbar->addWidget(m_redoBtn);

    m_textBtn = new QPushButton("Text", this);
    m_textBtn->setToolTip("Add text overlay");
    connect(m_textBtn, &QPushButton::clicked, this, &MainWindow::onAddText);
    m_toolbar->addWidget(m_textBtn);

    m_audioBtn = new QPushButton("Audio", this);
    m_audioBtn->setToolTip("Add audio track");
    connect(m_audioBtn, &QPushButton::clicked, this, &MainWindow::onAddAudio);
    m_toolbar->addWidget(m_audioBtn);

    m_themeBtn = new QPushButton("Theme", this);
    m_themeBtn->setToolTip("Toggle light/dark theme (Ctrl+Shift+T)");
    connect(m_themeBtn, &QPushButton::clicked, this, &MainWindow::onToggleTheme);
    m_toolbar->addWidget(m_themeBtn);

    m_toolbar->addSeparator();

    m_timeLabel = new QLabel("00:00", this);
    m_timeLabel->setMinimumWidth(100);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_toolbar->addWidget(m_timeLabel);

    addToolBar(m_toolbar);
}

void MainWindow::setupCentralWidget() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Video Preview
    QGroupBox *previewGroup = new QGroupBox("Preview", this);
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);
    previewLayout->setContentsMargins(0, 0, 0, 0);

    m_videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoWidget->setMinimumHeight(400);
    previewLayout->addWidget(m_videoWidget);

    m_seekSlider = new QSlider(Qt::Horizontal, this);
    connect(m_seekSlider, &QSlider::valueChanged, m_mediaPlayer, &QMediaPlayer::setPosition);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, m_seekSlider, &QSlider::setValue);
    previewLayout->addWidget(m_seekSlider);

    mainLayout->addWidget(previewGroup, 1);

    // Timeline
    QGroupBox *timelineGroup = new QGroupBox("Timeline", this);
    QVBoxLayout *timelineLayout = new QVBoxLayout(timelineGroup);
    timelineLayout->setContentsMargins(0, 0, 0, 0);

    m_trimmer = new VideoTrimmer(this);
    m_trimmer->setMinimumHeight(120);
    timelineLayout->addWidget(m_trimmer);

    mainLayout->addWidget(timelineGroup);

    setCentralWidget(centralWidget);
}

void MainWindow::loadStylesheet() {
    if (m_currentTheme == ThemeManager::Light) {
        setStyleSheet(
            "QMainWindow { background: #f0f0f0; }"
            "QMenuBar { background: #f8f8f8; color: #000; }"
            "QMenuBar::item { padding: 4px 8px; }"
            "QMenuBar::item:selected { background: #e0e0e0; }"
            "QMenu { background: #fff; color: #000; border: 1px solid #ccc; }"
            "QMenu::item:selected { background: #e0e0e0; }"
            "QToolBar { background: #f8f8f8; border-bottom: 1px solid #ccc; }"
            "QPushButton { background: #e0e0e0; color: #000; border: 1px solid #ccc; padding: 6px 12px; border-radius: 4px; }"
            "QPushButton:hover { background: #d0d0d0; }"
            "QLabel { color: #000; }"
            "QGroupBox { border: 1px solid #ccc; border-radius: 4px; margin-top: 8px; padding-top: 12px; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 8px; color: #000; }"
            "QSlider::groove:horizontal { height: 8px; background: #ccc; border-radius: 4px; }"
            "QSlider::handle:horizontal { background: #888; width: 16px; height: 16px; margin: -4px 0; border-radius: 8px; }"
        );
    } else {
        setStyleSheet(
            "QMainWindow { background: #1e1e1e; }"
            "QMenuBar { background: #252526; color: #ccc; }"
            "QMenuBar::item { padding: 4px 8px; }"
            "QMenuBar::item:selected { background: #2d2d30; }"
            "QMenu { background: #2d2d30; color: #ccc; border: 1px solid #000; }"
            "QMenu::item:selected { background: #3e3e42; }"
            "QToolBar { background: #252526; border-bottom: 1px solid #000; }"
            "QPushButton { background: #2d2d30; color: #ccc; border: 1px solid #3d3d3d; padding: 6px 12px; border-radius: 4px; }"
            "QPushButton:hover { background: #363636; }"
            "QLabel { color: #ccc; }"
            "QGroupBox { border: 1px solid #3d3d3d; border-radius: 4px; margin-top: 8px; padding-top: 12px; }"
            "QGroupBox::title { subcontrol-origin: margin; left: 8px; color: #fff; }"
            "QSlider::groove:horizontal { height: 8px; background: #333; border-radius: 4px; }"
            "QSlider::handle:horizontal { background: #555; width: 16px; height: 16px; margin: -4px 0; border-radius: 8px; }"
        );
    }
}

void MainWindow::generateThumbnails() {
    QList<QPixmap> thumbnails;
    for (int i = 0; i < 20; ++i) {
        QPixmap thumb(100, 60);
        thumb.fill(QColor(40 + i*5, 40 + i*5, 50 + i*5));
        QPainter p(&thumb);
        p.setPen(Qt::white);
        p.drawText(10, 20, QString("%1s").arg(i));
        p.end();
        thumbnails.append(thumb);
    }
    m_trimmer->setThumbnails(thumbnails);
}

void MainWindow::onPlayPause() {
    m_isPlaying = !m_isPlaying;
    m_playPauseBtn->setText(m_isPlaying ? "Pause" : "Play");

    if (m_isPlaying) {
        m_mediaPlayer->play();
        statusBar()->showMessage("Playing...");
    } else {
        m_mediaPlayer->pause();
        statusBar()->showMessage("Paused");
    }
}

void MainWindow::onExport() {
    QString fileName = QFileDialog::getSaveFileName(
        this, "Export Video",
        QDir::homePath(),
        "MP4 Files (*.mp4);;All Files (*)"
    );
    if (!fileName.isEmpty()) {
        statusBar()->showMessage("Exporting to: " + fileName);
        // TODO: Implement actual export
    }
}

void MainWindow::onOpenFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Video",
        QDir::homePath(),
        "Video Files (*.mp4 *.avi *.mov *.mkv);;All Files (*)"
    );

    if (!fileName.isEmpty()) {
        m_currentFile = fileName;
        statusBar()->showMessage("Loaded: " + fileName);

        m_mediaPlayer->setSource(QUrl::fromLocalFile(fileName));
        m_mediaPlayer->play();
        m_isPlaying = true;
        m_playPauseBtn->setText("Pause");
    }
}

void MainWindow::onCut() {
    qint64 position = m_trimmer->playheadPosition();
    statusBar()->showMessage(QString("Cut at %1ms").arg(position));
    // TODO: Implement actual cutting
}

void MainWindow::onUndo() {
    statusBar()->showMessage("Undo");
    // TODO: Implement undo
}

void MainWindow::onRedo() {
    statusBar()->showMessage("Redo");
    // TODO: Implement redo
}

void MainWindow::onAddText() {
    statusBar()->showMessage("Add Text Overlay");
    // TODO: Implement text overlay
}

void MainWindow::onAddAudio() {
    statusBar()->showMessage("Add Audio Track");
    // TODO: Implement audio track
}

void MainWindow::onToggleTheme() {
    ThemeManager::Theme newTheme = (m_currentTheme == ThemeManager::Light) ? ThemeManager::Dark : ThemeManager::Light;
    ThemeManager::instance().setTheme(newTheme);
    m_currentTheme = newTheme;
}

void MainWindow::onTrimInChanged(qint64 ms) {
    statusBar()->showMessage(QString("In: %1ms").arg(ms));
}

void MainWindow::onTrimOutChanged(qint64 ms) {
    statusBar()->showMessage(QString("Out: %1ms").arg(ms));
}

void MainWindow::onPlayheadMoved(qint64 ms) {
    m_mediaPlayer->setPosition(ms);
}#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isPlaying(false) {
    setupUI();
    setWindowTitle("MatrixEdit");
    resize(1200, 800);
}

MainWindow::~MainWindow() {
    delete mediaPlayer;
    delete audioOutput;
}

void MainWindow::setupUI() {
    toolbar = new QToolBar(this);
    toolbar->setMovable(false);

    openBtn = new QPushButton("Open", this);
    connect(openBtn, &QPushButton::clicked, this, &MainWindow::onOpenFile);
    toolbar->addWidget(openBtn);

    playPauseBtn = new QPushButton("Play", this);
    playPauseBtn->setMinimumWidth(80);
    connect(playPauseBtn, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    toolbar->addWidget(playPauseBtn);

    exportBtn = new QPushButton("Export", this);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::onExport);
    toolbar->addWidget(exportBtn);

    timeLabel = new QLabel("00:00", this);
    toolbar->addWidget(timeLabel);

    addToolBar(toolbar);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    videoWidget = new QVideoWidget(this);
    videoWidget->setMinimumHeight(400);
    layout->addWidget(videoWidget);

    seekSlider = new QSlider(Qt::Horizontal, this);
    layout->addWidget(seekSlider);

    QGroupBox *timeline = new QGroupBox("Timeline", this);
    QWidget *timelineContent = new QWidget(this);
    timelineContent->setMinimumHeight(100);
    timelineContent->setStyleSheet("background:#333;");
    QVBoxLayout *tl = new QVBoxLayout(timeline);
    tl->addWidget(timelineContent);
    layout->addWidget(timeline);

    setCentralWidget(central);

    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setVideoOutput(videoWidget);

    connect(seekSlider, &QSlider::valueChanged, mediaPlayer, &QMediaPlayer::setPosition);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, seekSlider, &QSlider::setValue);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        seekSlider->setMaximum(duration);
    });
}

void MainWindow::onPlayPause() {
    isPlaying = !isPlaying;
    playPauseBtn->setText(isPlaying ? "Pause" : "Play");
    if (isPlaying) mediaPlayer->play();
    else mediaPlayer->pause();
}

void MainWindow::onExport() {
    QFileDialog::getSaveFileName(this, "Export Video");
}

void MainWindow::onOpenFile() {
    QString file = QFileDialog::getOpenFileName(this, "Open Video", QDir::homePath(), "Video Files (*.mp4 *.avi *.mov)");
    if (!file.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(file));
        mediaPlayer->play();
        isPlaying = true;
        playPauseBtn->setText("Pause");
    }
}

