#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

class ThemeManager : public QObject {
    Q_OBJECT
public:
    enum Theme { Light, Dark };
    Q_ENUM(Theme)

    static ThemeManager& instance();
    void setTheme(Theme theme);
    Theme currentTheme() const;
    QString getStylesheet() const;

signals:
    void themeChanged(Theme theme);

private:
    ThemeManager();
    Theme m_currentTheme = Dark;
};
#endif
