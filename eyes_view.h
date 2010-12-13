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
signals:
public slots:
        void    mousePressEvent ( QMouseEvent * ev );
        void    mouseMoveEvent  ( QMouseEvent * ev );
private:
    QGraphicsScene    * g_scene;
    QVector <QPixmap>   images;
    bulwers_enum        bulwers;
    int                 px,
                        py;
};

#endif // EYES_VIEW_H
