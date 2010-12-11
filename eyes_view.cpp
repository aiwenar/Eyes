#include "eyes_view.h"
#include "eyes_info.h"

eyes_view::eyes_view ( QWidget * parent ) : QWidget ( parent )
{
    g_scene = new QGraphicsScene ( this );
    setMinimumSize ( EYES_W, EYES_H );
    setMaximumSize ( EYES_W, EYES_H );
}

eyes_view::~eyes_view ()
{
    delete g_scene;
}

void eyes_view::open_images ( QString folder )
{
}
