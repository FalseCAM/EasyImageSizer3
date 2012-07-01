#ifndef PREVIEWIMAGELABEL_H
#define PREVIEWIMAGELABEL_H
#include <QWidget>
#include <QLabel>

class QDragEnterEvent;
class QDropEvent;

class PreviewImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PreviewImageLabel(QWidget *parent=0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    
signals:
    void imageDroped(QString);
    
public slots:
    
};

#endif // PREVIEWIMAGELABEL_H
