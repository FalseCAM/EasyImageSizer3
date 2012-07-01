#include "previewimagelabel.h"
#include <QtGui>

PreviewImageLabel::PreviewImageLabel(QWidget *parent) :
    QLabel(parent)
{
         setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
         setAlignment(Qt::AlignCenter);
         setAcceptDrops(true);
         setAutoFillBackground(true);
         setMaximumSize(256,256);
         setScaledContents(true);
}


void PreviewImageLabel::dropEvent(QDropEvent* event) {
    // Drop part
    if (event && event->mimeData()) {
        const QMimeData *mData = event->mimeData();
        // Drop Images from FileManager into ImageList
        if (mData->hasUrls()) {
            QList<QUrl> urls = mData->urls();
            QString lastFile;
            for (int x = 0; x < urls.count(); ++x) {
                if (QFileInfo(urls.at(x).toLocalFile()).isDir());
                else
                    lastFile = urls.at(x).toLocalFile();
            }
            emit imageDroped(lastFile);
        }
    }
}

void PreviewImageLabel::dragEnterEvent(QDragEnterEvent *event)
 {
     setText(tr("<drop image file>"));
     setBackgroundRole(QPalette::Highlight);
     event->acceptProposedAction();
 }

 void PreviewImageLabel::dragMoveEvent(QDragMoveEvent *event)
 {
     event->acceptProposedAction();
 }

 void PreviewImageLabel::dragLeaveEvent(QDragLeaveEvent *event)
 {
     event->accept();
 }
