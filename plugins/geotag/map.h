#ifndef MAP_H
#define MAP_H


// Some code from http://qt-project.org/doc/qt-4.8/demos-embedded-lightmaps.html
// slippymap.h
#include <QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include <QPixmap>
#include <QUrl>

class QNetworkReply;
class QPainter;

class Map : public QWidget
{
    Q_OBJECT
public:
    Map(QWidget *parent = 0);
    ~Map();
    void setCenter(qreal lat, qreal lng);

    void invalidate();
    void render(QPainter *p, const QRect &rect);
    void pan(const QPoint &delta);

    int m_width;
    int m_height;
    int zoom;
    qreal latitude;
    qreal longitude;

signals:
    void updated(const QRect &rect);
    void latChanged(const qreal lat);
    void lngChanged(const qreal lng);

private slots:
    void updateMap(const QRect &r);
    void handleNetworkData(QNetworkReply *reply);
    void download();
public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    QRect tileRect(const QPoint &tp);

private:
    QPoint m_offset;
    QPoint m_center;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QPoint, QPixmap> m_tilePixmaps;
    QNetworkAccessManager m_manager;
    QUrl m_url;

    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;
};

#endif // MAP_H
