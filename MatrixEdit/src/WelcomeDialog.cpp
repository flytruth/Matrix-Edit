#include "WelcomeDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>

WelcomeDialog::WelcomeDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("MatrixEdit - Welcome");
    setFixedSize(600, 400);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("MatrixEdit", this);
    title->setStyleSheet("font-size: 32px; font-weight: bold; color: #4a90e2;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QLabel *subtitle = new QLabel("Create amazing videos", this);
    subtitle->setStyleSheet("font-size: 16px; color: #666;");
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    layout->addStretch();

    m_videoBtn = new QPushButton("Create Video Project", this);
    m_videoBtn->setStyleSheet("QPushButton { padding: 12px; font-size: 16px; }");
    m_videoBtn->setMinimumWidth(200);
    connect(m_videoBtn, &QPushButton::clicked, this, &WelcomeDialog::onCreateVideo);
    layout->addWidget(m_videoBtn, 0, Qt::AlignCenter);

    m_photoBtn = new QPushButton("Create Photo Project", this);
    m_photoBtn->setStyleSheet("QPushButton { padding: 12px; font-size: 16px; }");
    m_photoBtn->setMinimumWidth(200);
    connect(m_photoBtn, &QPushButton::clicked, this, &WelcomeDialog::onCreatePhoto);
    layout->addWidget(m_photoBtn, 0, Qt::AlignCenter);

    m_openBtn = new QPushButton("Open Existing Project", this);
    m_openBtn->setStyleSheet("QPushButton { padding: 12px; font-size: 16px; }");
    m_openBtn->setMinimumWidth(200);
    connect(m_openBtn, &QPushButton::clicked, this, &WelcomeDialog::onOpenProject);
    layout->addWidget(m_openBtn, 0, Qt::AlignCenter);

    layout->addStretch();

    QLabel *footer = new QLabel("© 2026 MatrixCut. All rights reserved.", this);
    footer->setStyleSheet("color: #888; font-size: 12px;");
    footer->setAlignment(Qt::AlignCenter);
    layout->addWidget(footer);
}

void WelcomeDialog::onCreateVideo() {
    accept();
}

void WelcomeDialog::onCreatePhoto() {
    accept();
}

void WelcomeDialog::onOpenProject() {
    QString file = QFileDialog::getOpenFileName(this, "Open Project");
    if (!file.isEmpty()) {
        accept();
    }
}
