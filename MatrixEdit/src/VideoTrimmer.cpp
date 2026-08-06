#include "VideoTrimmer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <algorithm>

VideoTrimmer::VideoTrimmer(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    setMinimumHeight(120);
}

void VideoTrimmer::setDuration(qint64 durationMs) {
    if (durationMs < 0) durationMs = 0;
    if (m_durationMs != durationMs) {
        m_durationMs = durationMs;
        if (m_outPointMs > m_durationMs) {
            m_outPointMs = m_durationMs;
            emit outPointChanged(m_outPointMs);
        }
        update();
    }
}

void VideoTrimmer::setTrimRange(qint64 inPointMs, qint64 outPointMs) {
    inPointMs = clampMs(inPointMs);
    outPointMs = clampMs(outPointMs);
    if (inPointMs > outPointMs) std::swap(inPointMs, outPointMs);

    if (m_inPointMs != inPointMs || m_outPointMs != outPointMs) {
        m_inPointMs = inPointMs;
        m_outPointMs = outPointMs;
        update();
        emit inPointChanged(m_inPointMs);
        emit outPointChanged(m_outPointMs);
    }
}

void VideoTrimmer::setPlayheadPosition(qint64 positionMs) {
    positionMs = clampMs(positionMs);
    if (m_playheadMs != positionMs) {
        m_playheadMs = positionMs;
        update();
        emit playheadMoved(m_playheadMs);
    }
}

void VideoTrimmer::setThumbnails(const QList<QPixmap> &thumbnails) {
    m_thumbnails = thumbnails;
    update();
}

void VideoTrimmer::setZoom(qreal zoom) {
    m_zoom = std::clamp(zoom, 0.1, 20.0);
    update();
}

int VideoTrimmer::msToPixel(qint64 ms) const {
    if (m_durationMs <= 0 || width() <= 0) return 0;
    qint64 visibleDuration = static_cast<qint64>(m_durationMs / m_zoom);
    qint64 offsetMs = (m_durationMs - visibleDuration) / 2;
    qint64 relativeMs = ms - offsetMs;
    if (relativeMs < 0) relativeMs = 0;
    if (relativeMs > visibleDuration) relativeMs = visibleDuration;
    return static_cast<int>((static_cast<qreal>(relativeMs) / visibleDuration) * width());
}

qint64 VideoTrimmer::pixelToMs(int x) const {
    if (m_durationMs <= 0 || width() <= 0) return 0;
    qint64 visibleDuration = static_cast<qint64>(m_durationMs / m_zoom);
    qint64 offsetMs = (m_durationMs - visibleDuration) / 2;
    qreal ratio = static_cast<qreal>(x) / width();
    return clampMs(static_cast<qint64>(ratio * visibleDuration) + offsetMs);
}

qint64 VideoTrimmer::clampMs(qint64 ms) const {
    return std::clamp(ms, static_cast<qint64>(0), m_durationMs);
}

void VideoTrimmer::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background
    painter.fillRect(rect(), QColor(35, 35, 38));

    // Timeline track
    const int trackHeight = 30;
    const int trackY = height() / 2 - trackHeight / 2;
    painter.fillRect(0, trackY, width(), trackHeight, QColor(50, 50, 55));

    // Thumbnails
    if (!m_thumbnails.isEmpty()) {
        const qreal thumbWidth = static_cast<qreal>(width()) / m_thumbnails.size();
        for (int i = 0; i < m_thumbnails.size(); ++i) {
            const QPixmap &thumb = m_thumbnails[i];
            if (!thumb.isNull()) {
                int x = static_cast<int>(i * thumbWidth);
                painter.drawPixmap(x, trackY, static_cast<int>(thumbWidth), trackHeight,
                    thumb.scaled(static_cast<int>(thumbWidth), trackHeight,
                                Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            }
        }
    }

    // Time markers
    painter.setPen(QColor(80, 80, 90));
    int markerInterval = std::max(50, static_cast<int>(100 / m_zoom));
    for (int x = 0; x <= width(); x += markerInterval) {
        qint64 ms = pixelToMs(x);
        int seconds = static_cast<int>(ms / 1000);
        int minutes = seconds / 60;
        seconds %= 60;

        painter.drawLine(x, trackY, x, trackY + trackHeight);

        if (x % (markerInterval * 2) == 0 || x == 0 || x == width()) {
            QString timeStr = QString("%1:%2").arg(minutes, 2, 'f', 0, '0')
                                                  .arg(seconds, 2, 'f', 0, '0');
            int textY = (x < width() / 2) ? trackY - 20 : trackY + trackHeight + 15;
            painter.setPen(QColor(200, 200, 210));
            painter.drawText(x + 5, textY, timeStr);
            painter.setPen(QColor(80, 80, 90));
        }
    }

    // Cut zone
    int inX = msToPixel(m_inPointMs);
    int outX = msToPixel(m_outPointMs);
    int playheadX = msToPixel(m_playheadMs);

    // Dimmed areas
    painter.fillRect(0, 0, inX, height(), QColor(20, 20, 25, 200));
    painter.fillRect(outX, 0, width() - outX, height(), QColor(20, 20, 25, 200));
    painter.fillRect(inX, 0, outX - inX, height(), QColor(70, 130, 180, 100));

    // Playhead
    painter.setPen(QPen(QColor(255, 255, 255), 2));
    painter.drawLine(playheadX, 0, playheadX, height());

    // Handles
    const int handleWidth = 12;
    const int handleHeight = 40;
    const int handleY = trackY - (handleHeight - trackHeight) / 2;

    painter.fillRect(inX - handleWidth/2, handleY, handleWidth, handleHeight, QColor(255, 85, 85));
    painter.setPen(QColor(255, 255, 255, 180));
    painter.drawRect(inX - handleWidth/2, handleY, handleWidth, handleHeight);

    painter.fillRect(outX - handleWidth/2, handleY, handleWidth, handleHeight, QColor(85, 200, 85));
    painter.drawRect(outX - handleWidth/2, handleY, handleWidth, handleHeight);

    // Playhead triangle
    painter.setBrush(QColor(255, 255, 255));
    painter.setPen(Qt::NoPen);
    QPolygon triangle;
    triangle << QPoint(playheadX, trackY - 8)
             << QPoint(playheadX - 6, trackY + 4)
             << QPoint(playheadX + 6, trackY + 4);
    painter.drawPolygon(triangle);

    // Current time display
    int seconds = static_cast<int>(m_playheadMs / 1000);
    int minutes = seconds / 60;
    seconds %= 60;
    QString timeStr = QString("%1:%2").arg(minutes, 2, 'f', 0, '0')
                                          .arg(seconds, 2, 'f', 0, '0');
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(playheadX + 10, 20, timeStr);
}

void VideoTrimmer::mousePressEvent(QMouseEvent *event) {
    int x = event->position().toPoint().x();
    int inX = msToPixel(m_inPointMs);
    int outX = msToPixel(m_outPointMs);
    int playheadX = msToPixel(m_playheadMs);

    if (std::abs(x - inX) <= 8) {
        m_dragMode = DragIn;
        m_dragStartX = x;
        m_dragStartMs = m_inPointMs;
    } else if (std::abs(x - outX) <= 8) {
        m_dragMode = DragOut;
        m_dragStartX = x;
        m_dragStartMs = m_outPointMs;
    } else if (std::abs(x - playheadX) <= 8) {
        m_dragMode = DragPlayhead;
        m_dragStartX = x;
        m_dragStartMs = m_playheadMs;
    } else {
        setPlayheadPosition(pixelToMs(x));
        m_dragMode = DragPlayhead;
        m_dragStartX = x;
        m_dragStartMs = m_playheadMs;
    }
    event->accept();
}

void VideoTrimmer::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragMode == None) return;

    int x = event->position().toPoint().x();
    qint64 newMs = pixelToMs(x);

    switch (m_dragMode) {
    case DragIn:
        newMs = std::min(newMs, m_outPointMs);
        setTrimRange(newMs, m_outPointMs);
        break;
    case DragOut:
        newMs = std::max(newMs, m_inPointMs);
        setTrimRange(m_inPointMs, newMs);
        break;
    case DragPlayhead:
        setPlayheadPosition(newMs);
        break;
    case None:
        break;
    }
    event->accept();
}

void VideoTrimmer::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    m_dragMode = None;
}

void VideoTrimmer::wheelEvent(QWheelEvent *event) {
    qreal oldZoom = m_zoom;
    qreal zoomFactor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    m_zoom = std::clamp(m_zoom * zoomFactor, 0.1, 20.0);

    int mouseX = event->position().toPoint().x();
    qint64 mouseMsBefore = pixelToMs(mouseX);
    qint64 visibleDurationBefore = static_cast<qint64>(m_durationMs / oldZoom);
    qint64 visibleDurationAfter = static_cast<qint64>(m_durationMs / m_zoom);
    qint64 offsetBefore = (m_durationMs - visibleDurationBefore) / 2;
    qint64 mouseMsRelative = mouseMsBefore - offsetBefore;
    qint64 offsetAfter = mouseMsBefore - static_cast<qint64>(mouseMsRelative * (visibleDurationAfter / static_cast<qreal>(visibleDurationBefore)));
    m_zoomCenterMs = offsetAfter + visibleDurationAfter / 2;

    update();
    event->accept();
}
