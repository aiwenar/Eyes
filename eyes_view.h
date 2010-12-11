#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QVector>

#include "bulwers.h"

class eyes_view : public QWidget
{
    Q_OBJECT
public:
    explicit    eyes_view   ( QWidget * parent );
                ~eyes_view  ();
    void        open_images ( QString folder );
signals:
public slots:
private:
    QGraphicsScene  * g_scene;
    QVector <QImage>  images;
    bulwers_enum      bulwers;
};

#endif // EYES_VIEW_H
