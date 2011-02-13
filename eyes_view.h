#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QVector>
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
    void                update_bulwers      ( core_stats * input );
    int                 heightForWidth      ( int w )                               const;
    QVariant            inputMethodQuery    ( Qt::InputMethodQuery query )          const;
    QSize               sizeHint            () 															const;
signals:
public slots:
        void            mousePressEvent     ( QMouseEvent * ev );
        void            mouseMoveEvent      ( QMouseEvent * ev );
private:
    QPixmap           * out,
                      * shadow,
                      * mirror,
                      * eyein,
                      * eye,
                      * spec;
    int                 px,
                        py,
                        epx1,
                        epx2,
                        epy;
};

extern eyes_view * eyes;

#endif // EYES_VIEW_H
