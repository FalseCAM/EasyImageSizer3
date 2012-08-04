#include "map.h"
#include <math.h>

#include <QtGui>
#include <QtNetwork/QtNetwork>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

uint qHash(const QPoint& p)
{
    return p.x() * 17 ^ p.y();
}

// tile size in pixels
const int tdim = 256;

QPointF tileForCoordinate(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

qreal longitudeFromTile(qreal tx, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal lat = tx / zn * 360.0 - 180.0;
    return lat;
}

qreal latitudeFromTile(qreal ty, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal n = M_PI - 2 * M_PI * ty / zn;
    qreal lng = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    return lng;
}


Map::Map(QWidget *parent)
    : QWidget(parent), m_width(400), m_height(300), zoom(15),
      latitude(59.9138204), longitude(10.7387413)
{
    m_emptyTile = QPixmap(tdim, tdim);
    m_emptyTile.fill(Qt::lightGray);

    QNetworkDiskCache *cache = new QNetworkDiskCache;
    cache->setCacheDirectory(QDesktopServices::storageLocation
                                (QDesktopServices::CacheLocation));
    m_manager.setCache(cache);
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
    connect(this, SIGNAL(updated(QRect)), this, SLOT(updateMap(QRect)));
}

Map::~Map(){

}

void Map::setCenter(qreal lat, qreal lng)
{
    latitude = lat;
    longitude = lng;
    invalidate();
}

void Map::invalidate()
{
    if (m_width <= 0 || m_height <= 0)
        return;

    QPointF ct = tileForCoordinate(latitude, longitude, zoom);
    qreal tx = ct.x();
    qreal ty = ct.y();

    // top-left corner of the center tile
    int xp = m_width / 2 - (tx - floor(tx)) * tdim;
    int yp = m_height / 2 - (ty - floor(ty)) * tdim;

    // first tile vertical and horizontal
    int xa = (xp + tdim - 1) / tdim;
    int ya = (yp + tdim - 1) / tdim;
    int xs = static_cast<int>(tx) - xa;
    int ys = static_cast<int>(ty) - ya;

    // offset for top-left tile
    m_offset = QPoint(xp - xa * tdim, yp - ya * tdim);

    // last tile vertical and horizontal
    int xe = static_cast<int>(tx) + (m_width - xp - 1) / tdim;
    int ye = static_cast<int>(ty) + (m_height - yp - 1) / tdim;

    // build a rect
    m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);

    if (m_url.isEmpty())
        download();

    emit updated(QRect(0, 0, m_width, m_height));
}

void Map::render(QPainter *p, const QRect &rect)
{
    for (int x = 0; x <= m_tilesRect.width(); ++x)
        for (int y = 0; y <= m_tilesRect.height(); ++y) {
            QPoint tp(x + m_tilesRect.left(), y + m_tilesRect.top());
            QRect box = tileRect(tp);
            if (rect.intersects(box)) {
                if (m_tilePixmaps.contains(tp))
                    p->drawPixmap(box, m_tilePixmaps.value(tp));
                else
                    p->drawPixmap(box, m_emptyTile);
            }
        }
    p->setBrush(QBrush(Qt::red));
    // TODO: circle on wrong position
    p->drawEllipse(rect.width()/2-4, rect.height()/2-4, 8, 8);
}

void Map::pan(const QPoint &delta)
{
    QPointF dx = QPointF(delta) / qreal(tdim);
    QPointF center = tileForCoordinate(latitude, longitude, zoom) - dx;
    latitude = latitudeFromTile(center.y(), zoom);
    longitude = longitudeFromTile(center.x(), zoom);
    emit latChanged(latitude);
    emit lngChanged(longitude);
    invalidate();
}

void Map::handleNetworkData(QNetworkReply *reply)
{
    QImage img;
    QPoint tp = reply->request().attribute(QNetworkRequest::User).toPoint();
    QUrl url = reply->url();
    if (!reply->error())
        if (!img.load(reply, 0))
            img = QImage();
    reply->deleteLater();
    m_tilePixmaps[tp] = QPixmap::fromImage(img);
    if (img.isNull())
        m_tilePixmaps[tp] = m_emptyTile;
    emit updated(tileRect(tp));

    // purge unused spaces
    QRect bound = m_tilesRect.adjusted(-2, -2, 2, 2);
    foreach(QPoint tp, m_tilePixmaps.keys())
    if (!bound.contains(tp))
        m_tilePixmaps.remove(tp);

    download();
}

void Map::download()
{
    QPoint grab(0, 0);
    for (int x = 0; x <= m_tilesRect.width(); ++x)
        for (int y = 0; y <= m_tilesRect.height(); ++y) {
            QPoint tp = m_tilesRect.topLeft() + QPoint(x, y);
            if (!m_tilePixmaps.contains(tp)) {
                grab = tp;
                break;
            }
        }
    if (grab == QPoint(0, 0)) {
        m_url = QUrl();
        return;
    }

    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    m_url = QUrl(path.arg(zoom).arg(grab.x()).arg(grab.y()));
    qDebug("[Geotag-Plugin] downloading: %s", qPrintable(m_url.toString()));
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("User-Agent", "EasyImageSizer Geotag plugin");
    request.setAttribute(QNetworkRequest::User, QVariant(grab));
    m_manager.get(request);
}

QRect Map::tileRect(const QPoint &tp)
{
    QPoint t = tp - m_tilesRect.topLeft();
    int x = t.x() * tdim + m_offset.x();
    int y = t.y() * tdim + m_offset.y();
    return QRect(x, y, tdim, tdim);
}

void Map::updateMap(const QRect &r)
{
    update(r);
}

void Map::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    render(&p, event->rect());
    p.setPen(Qt::black);
    p.end();
}

void Map::timerEvent(QTimerEvent *)
{
    update();
}

void Map::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;
    pressed = snapped = true;
    pressPos = dragPos = event->pos();
}

void Map::mouseMoveEvent(QMouseEvent *event)
{
    if (!event->buttons())
        return;

    if (!pressed || !snapped) {
        QPoint delta = event->pos() - pressPos;
        pressPos = event->pos();
        pan(delta);
        return;
    } else {
        const int threshold = 10;
        QPoint delta = event->pos() - pressPos;
        if (snapped) {
            snapped &= delta.x() < threshold;
            snapped &= delta.y() < threshold;
            snapped &= delta.x() > -threshold;
            snapped &= delta.y() > -threshold;
        }
    }
}

void Map::mouseReleaseEvent(QMouseEvent *)
{
    update();
}

void Map::zoomIn(){
    this->zoom++;
    if(zoom > 16) zoom = 16;
    invalidate();
}

void Map::zoomOut(){
    this->zoom--;
    if(zoom < 3) zoom = 3;
    invalidate();
}
