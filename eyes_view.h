#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QMap>
#include <QMouseEvent>
#include <QtConcurrentRun>

#include "bulwers.h"
#include "defines.hxx"

class eyes_view : public QWidget
{
    Q_OBJECT
public:
    explicit    	eyes_view           ( QWidget * parent );
                        ~eyes_view          ();
    void        	open_images         ( QString folder );
    void        	paintEvent          ( QPaintEvent * );
    void                closeEvent          ( QCloseEvent * ev );
    void                update_bulwers      ( core_stats * color );
    void                update_mask         ();
    int                 heightForWidth      ( int w )                               const;
    QVariant            inputMethodQuery    ( Qt::InputMethodQuery query )          const;
    QSize               sizeHint            () 															const;
signals:
public slots:
    void            mousePressEvent     ( QMouseEvent * ev );
    void            mouseMoveEvent      ( QMouseEvent * ev );
private:
    QWidget               * win;
    QBitmap                 mask;
    QPixmap               * area;
    QString                 eye,
                            face,
                            spec;
    QMap<QString,QPixmap>   pics;
    QFuture<void>           c_main;
    int                     px,
                            py,
                            epx1,
                            epx2,
                            epy,
                            emx1,
                            emx2,
                            emy;
};

extern eyes_view * eyes;

#endif // EYES_VIEW_H
