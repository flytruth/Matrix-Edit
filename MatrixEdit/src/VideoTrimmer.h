#ifndef VIDEOTRIMMER_H
#define VIDEOTRIMMER_H

#include <QWidget>
#include <QList>
#include <QPixmap>

class VideoTrimmer : public QWidget {
    Q_OBJECT
public:
    explicit VideoTrimmer(QWidget *parent = nullptr);

    void setDuration(qint64 durationMs);
    void setTrimRange(qint64 inPointMs, qint64 outPointMs);
    void setPlayheadPosition(qint64 positionMs);
    void setThumbnails(const QList<QPixmap> &thumbnails);
    void setZoom(qreal zoom);

    qint64 inPoint() const { return m_inPointMs; }
    qint64 outPoint() const { return m_outPointMs; }
    qint64 playheadPosition() const { return m_playheadMs; }

signals:
    void inPointChanged(qint64 ms);
    void outPointChanged(qint64 ms);
    void playheadMoved(qint64 ms);
    void cutRequested(qint64 positionMs);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    int msToPixel(qint64 ms) const;
    qint64 pixelToMs(int x) const;
    qint64 clampMs(qint64 ms) const;

    qint64 m_durationMs = 0;
    qint64 m_inPointMs = 0;
    qint64 m_outPointMs = 0;
    qint64 m_playheadMs = 0;
    qreal m_zoom = 1.0;

    QList<QPixmap> m_thumbnails;
    enum DragMode { None, DragIn, DragOut, DragPlayhead };
    DragMode m_dragMode = None;
    int m_dragStartX = 0;
    qint64 m_dragStartMs = 0;
};
#endif
