#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QMouseEvent>

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
    int                 heightForWidth      ( int w )                               const;
    QVariant            inputMethodQuery    ( Qt::InputMethodQuery query )          const;
    QSize               sizeHint            () 															const;
signals:
public slots:
    void            mousePressEvent     ( QMouseEvent * ev );
    void            mouseMoveEvent      ( QMouseEvent * ev );
private:
    QString                 eye,
                            spec,
                            out,
                            shadow,
                            mirror;
    QMap<QString,QPixmap>   pics;
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
