#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

class QPushButton;

class WelcomeDialog : public QDialog {
    Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *parent = nullptr);

private slots:
    void onCreateVideo();
    void onCreatePhoto();
    void onOpenProject();

private:
    QPushButton *m_videoBtn;
    QPushButton *m_photoBtn;
    QPushButton *m_openBtn;
};
#endif
