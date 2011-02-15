#include "eyes_view.h"
#include "eyes_info.h"

#include <QPainter>
#include <iostream>

using namespace std;

#define MM_NO_MOTION 10000

static char   folder[]    = "./pics/";
static char * files[]     = {
    "clap_01_a", "clap_01_m", "clap_01_o", "clap_01_s",         // 4
    "clap_02_a", "clap_02_m", "clap_02_o", "clap_02_s",         // 8
    "clap_03_a", "clap_03_m", "clap_03_o", "clap_03_s",         // 12
    "clap_04_a", "clap_04_m", "clap_04_o", "clap_04_s",         // 16
    "clap_05_a", "clap_05_m", "clap_05_o", "clap_05_s",         // 20
    "bul_01_a", "bul_01_m", "bul_01_o", "bul_01_s",             // 24
    "bul_02_a", "bul_02_m",
    "cusual_01_a", "cusual_01_m", "cusual_01_o", "cusual_01_s", //
    "eye_01_n", "eye_02_n", "eye_03_n", "eye_04_n", "eye_05_n", //
    "eye_06_n", "eye_07_n", "eye_08_n", "eye_09_n", "eye_10_n", //
    "tired_01", "tired_02", "tired_03",                         //
    "spec"                                                      //
};

void core_main ();
bool is_finished;

eyes_view::eyes_view ( QWidget * parent ) : QWidget ( parent )
{
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    epx1 = 46;
    epx2 = 213;
    emx1 = 83;
    emx2 = 252;
    epy = 10;
    emy = 24;
    setMinimumSize ( EYES_W, EYES_H );
    setMaximumSize ( EYES_W, EYES_H );
    spec = "spec";
    eye = "eye_04_n";
    face = "cusual_01";
    is_finished = false;
    c_main = QtConcurrent::run ( core_main );
    area = new QPixmap ( EYES_W, EYES_H );
    update_mask ();
    win = parent;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( eyes_time_event () ) );
}

eyes_view::~eyes_view ()
{
                delete &px, &py, &epx1, &epx2, &epy, &emx1, &emx2, &emy;
}

void eyes_view::open_images ( QString color )
{
    QPixmap file;
    bool no_file ( false );
    for ( int i=0 ; i<38 ; i++ )
    {
        file.load ( QString ( folder ) + files[i] + ".png" );
        if ( file.isNull () )
        {
            cerr << "[error :] file " << ( QString ( folder ) + files[i] + ".png" ).toStdString () << " is nil.\n";
            no_file = true;
        }
        else
        {
            pics.insert ( QString ( files[i] ), file.scaled ( EYES_W, EYES_H, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ) );
            cerr << "[info :] loading image " << files[i] << ".\n";
        }
    }
    if ( no_file )
    {
        c_main.cancel ();
        cerr << "Some files may not exist, exiting...\n";
        exit ( 2 );
    }
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    QPainter paint ( this );
    QPainter parea ( area );
    area->fill ( QColor ( 0, 0, 0 ) );
    parea.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[face+"_a"] );
    parea.drawPixmap ( epx1, epy, EYE_S, EYE_S, pics[eye] );
    parea.drawPixmap ( epx2, epy, EYE_S, EYE_S, pics[eye] );
    parea.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[face+"_s"] );
    parea.drawPixmap ( emx1, emy, EYE_M, EYE_M, pics[spec] );
    parea.drawPixmap ( emx2, emy, EYE_M, EYE_M, pics[spec] );
    parea.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[face+"_m"] );
    parea.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[face+"_o"] );
    parea.end ();
    area->setMask ( pics[face+"_a"].mask () );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, *area );
}

void eyes_view::mousePressEvent ( QMouseEvent * ev )
{
    ev->ignore ();
}

void eyes_view::mouseMoveEvent ( QMouseEvent * ev )
{
    ev->accept ();
    if ( px == MM_NO_MOTION or py == MM_NO_MOTION )
    {
        px = ev->x ();
        py = ev->y ();
        return;
    }
    int dx = ( ( px > ev->x () ? -1 : 1 ) * px ) + ev->x ();
    int dy = ( ( py > ev->y () ? -1 : 1 ) * py ) + ev->y ();
    move ( dx, dy );
    repaint ();
}

void eyes_view::eyes_time_event ()
{

}

void eyes_view::closeEvent ( QCloseEvent * ev )
{
    ev->accept ();
    is_finished = true;
    c_main.waitForFinished ();
    cout << "\033[0m";
}

void eyes_view::update_mask ()
{
    return;
}

int eyes_view::heightForWidth ( int w ) const
{
    return ( EYES_H );
}

QVariant eyes_view::inputMethodQuery ( Qt::InputMethodQuery query ) const
{
    return ( nil );
}

QSize eyes_view::sizeHint () const
{
    return ( QSize ( EYES_W, EYES_H ) );
}
