#ifndef EYES_VIEW_H
#define EYES_VIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QMap>
#include <QList>
#include <QMouseEvent>
#include <QtConcurrentRun>
#include <QTimer>
#include <QThread>
#include <QTime>
#include <libconfig.h++>
#include <ostream>

#include "bulwers.h"
#include "defines.hxx"
#include "animation.hxx"

using namespace libconfig;
using namespace std;

class eyes_clapper;
class eyes_looker;

class eyes_view : public QWidget
{
    Q_OBJECT
public:
    explicit    	eyes_view           ( QWidget * parent, QString color );
                        ~eyes_view          ();
    void        	open_images         ( QString folder );
    void        	paintEvent          ( QPaintEvent * );
    void                closeEvent          ( QCloseEvent * ev );
    void                update_bulwers      ( core_stats * color );
    void                set_face            ( QString nface );
    void                set_face            ( QString nface, void * nlocker );
    void                set_eyes            ( QString neyes );
    void                set_eyes_position   ( int nx1, int nx2, int ny );
    void                lock_face           ( void * nlocker );
    void                unlock_face         ( void * lovker );
    void                unlock_face         ();
    void                set_animation       ( QString start, QString end, int from, int to );
    void                graphics_prepare    ();
    int                 heightForWidth      ( int w )                               const;
    int                 get_next_clap_delay ();
    int                 get_eyes_x1         () { return epx1; }
    int                 get_eyes_x2         () { return epx2; }
    int                 get_eyes_y          () { return epy; }
    QVariant            inputMethodQuery    ( Qt::InputMethodQuery query )          const;
    QSize               sizeHint            ()                                      const;
    QString             get_face            ();
    QString	       get_face_send       ();
signals:
public slots:
    void            mousePressEvent     ( QMouseEvent * ev );
    void            mouseMoveEvent      ( QMouseEvent * ev );
private:
    QTimer                * timer;
    QTime                   time;
    QWidget               * win;
    QBitmap                 mask;
    QPixmap               * area;
    QString                 eye,
                            face,
                            face_next,
			    face_send,
                            spec,
                            color;
    QMap<QString,QPixmap>   pics;
    QMap<QString,QPixmap>   eyes;
    QFuture<void>           c_main;
    int                     px,
                            py,
                            epx1,
                            epx2,
                            epy,
                            emx1,
                            emx2,
                            emy;
    bool                    is_face_locked;
    void                  * locker;
    eyes_clapper          * clapper;
    eyes_looker           * looker;
    Config                * set;
};

class eyes_clapper : public QThread
{
    Q_OBJECT
public:
    explicit    eyes_clapper        ( eyes_view * neyes );
    int         get_next_clap_delay ();
    void        run                 ();
    void        set_animation       ( QString nstart, QString nend, int nfrom, int nto );
    void        load_config         ( Config * set );
public slots:
    void        clap                ();
private:
    int                         stage,
                                from,
                                to,
                                size1,
                                size2,
                                min_dl,
                                max_dl;
    eyes_view                 * eyes;
    QTime                       time;
    QTimer                    * timer;
    QString                     old_face,
                                start,
                                end;
    QMap<QString,animation*>    animations;
};

class eyes_looker : public QObject
{
    Q_OBJECT
public:
            eyes_looker ( eyes_view * eyes );
    void    load_config ( Config * set );
    void    run ();
public slots:
    void    look ();
private:
    eyes_view * eyes;
    QTimer    * timer;
    int         min_dx,
                max_dx,
                min_dy,
                max_dy,
                min_dl,
                max_dl;
};

extern eyes_view * eyes;

#endif // EYES_VIEW_H
