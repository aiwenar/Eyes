#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QMouseEvent>

#include "bulwers.h"

class eyes_view : public QWidget
{
    Q_OBJECT
public:
    explicit    eyes_view       ( QWidget * parent );
                ~eyes_view      ();
    void        open_images     ( QString folder );
    void        paintEvent      ( QPaintEvent * );
    void        update_bulwers  ( bulwers_s nbulwers );
signals:
public slots:
        void    mousePressEvent ( QMouseEvent * ev );
        void    mouseMoveEvent  ( QMouseEvent * ev );
private:
    void        null_protect    ( QPixmap * pix, QString pix_name );
    QPixmap           * out,
                      * shadow,
                      * mirror,
                      * eyein,
                      * eye,
                      * spec;
    bulwers_s           bulwers;
    int                 px,
                        py,
                        epx,
                        epy;
};

#endif // EYES_VIEW_H
