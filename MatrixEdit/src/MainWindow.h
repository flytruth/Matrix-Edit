#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include "ThemeManager.h"
#include "VideoTrimmer.h"

QT_BEGIN_NAMESPACE
class QToolBar;
class QPushButton;
class QLabel;
class QSlider;
class QMediaPlayer;
class QVideoWidget;
class QAudioOutput;
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayPause();
    void onExport();
    void onOpenFile();
    void onCut();
    void onUndo();
    void onRedo();
    void onAddText();
    void onAddAudio();
    void onToggleTheme();
    void onTrimInChanged(qint64 ms);
    void onTrimOutChanged(qint64 ms);
    void onPlayheadMoved(qint64 ms);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupCentralWidget();
    void loadStylesheet();
    void generateThumbnails();

    QToolBar *m_toolbar;
    QPushButton *m_playPauseBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_openBtn;
    QPushButton *m_cutBtn;
    QPushButton *m_undoBtn;
    QPushButton *m_redoBtn;
    QPushButton *m_textBtn;
    QPushButton *m_audioBtn;
    QPushButton *m_themeBtn;
    QLabel *m_timeLabel;
    QSlider *m_seekSlider;

    VideoTrimmer *m_trimmer;
    QMediaPlayer *m_mediaPlayer;
    QVideoWidget *m_videoWidget;
    QAudioOutput *m_audioOutput;

    bool m_isPlaying;
    QString m_currentFile;
    ThemeManager::Theme m_currentTheme;
};
#endif

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onPlayPause();
    void onExport();
    void onOpenFile();
private:
    void setupUI();
    QToolBar *toolbar;
    QPushButton *playPauseBtn;
    QPushButton *exportBtn;
    QPushButton *openBtn;
    QLabel *timeLabel;
    QSlider *seekSlider;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QAudioOutput *audioOutput;
    bool isPlaying;
};
#endif

