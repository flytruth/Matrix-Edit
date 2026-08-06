#include "ThemeManager.h"
#include <QApplication>

ThemeManager::ThemeManager() {}

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setTheme(Theme theme) {
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        emit themeChanged(theme);
        qApp->setStyleSheet(getStylesheet());
    }
}

ThemeManager::Theme ThemeManager::currentTheme() const {
    return m_currentTheme;
}

QString ThemeManager::getStylesheet() const {
    if (m_currentTheme == Light) {
        return
            "QMainWindow { background: #f0f0f0; }"
            "QToolBar { background: #f8f8f8; }"
            "QPushButton { background: #e0e0e0; color: #000; border: 1px solid #ccc; }"
            "QLabel { color: #000; }"
            "QGroupBox { border: 1px solid #ccc; }";
    } else {
        return
            "QMainWindow { background: #1e1e1e; }"
            "QToolBar { background: #252526; }"
            "QPushButton { background: #2d2d30; color: #ccc; border: 1px solid #3d3d3d; }"
            "QLabel { color: #ccc; }"
            "QGroupBox { border: 1px solid #3d3d3d; }"
            "QSlider::groove:horizontal { height: 8px; background: #333; }"
            "QSlider::handle:horizontal { background: #555; width: 16px; height: 16px; margin: -4px 0; }";
    }
}
