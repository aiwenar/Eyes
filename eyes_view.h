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

#include "bulwers.h"
#include "defines.hxx"
#include "animation.hxx"

class eyes_clapper;

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
    void                lock_face           ( void * nlocker );
    void                unlock_face         ( void * lovker );
    void                unlock_face         ();
    void                set_animation       ( QString start, QString end, int from, int to );
    int                 heightForWidth      ( int w )                               const;
    int                 get_next_clap_delay ();
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
    QWidget               * win;
    QBitmap                 mask;
    QPixmap               * area;
    QString                 eye,
                            face,
                            face_next,
			    face_send,
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
    bool                    is_face_locked;
    void                  * locker;
    eyes_clapper          * clapper;
};

class eyes_clapper : public QThread
{
    Q_OBJECT
public:
    explicit    eyes_clapper        ( eyes_view * neyes );
    int         get_next_clap_delay ();
    void        run                 ();
    void        set_animation       ( QString nstart, QString nend, int nfrom, int nto );
public slots:
    void        clap                ();
private:
    int                         stage,
                                from,
                                to,
                                size1,
                                size2;
    eyes_view                 * eyes;
    QTime                       time;
    QTimer                    * timer;
    QString                     old_face,
                                start,
                                end;
    QMap<QString,animation*>    animations;
};

extern eyes_view * eyes;

#endif // EYES_VIEW_H
