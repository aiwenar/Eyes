#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include "configurator.hxx"

Configurator::Configurator ( QWidget * parent ) : QDialog ( parent )
{
    resize          ( 647, 400 );
    setWindowTitle  ( "!eyes config" );

    //tabs = new QTabWidget ( this );
    //tabs->setTabPosition  ( QTabWidget::East );
    //tabs->addTab ( new ConfTabUi (), QString ( "eyes" ) );
    ConfTabUi * ui = new ConfTabUi ( this );
}

void Configurator::set_icon ( QIcon * icn )
{
    icon = icn;
    setWindowIcon ( *icn );
}

ConfTabUi::ConfTabUi ( QWidget * parent ) : QWidget ( parent )
{
    geyes    = new QGroupBox ( "eyes" );
    glooker  = new QGroupBox ( "looker" );
    gclapper = new QGroupBox ( "clapper" );


    QVBoxLayout * ml = new QVBoxLayout ( this );
    ml->addWidget   ( geyes, );
    ml->addWidget   ( glooker );
    ml->addWidget   ( gclapper );

    ccolor = new QComboBox ();
    ccolor->addItem ( "green" );

    QGridLayout * geyesl = new QGridLayout ();
    geyesl->addWidget ( new QLabel ( "color" ), 0, 1, 0, 1 );
    geyesl->addWidget ( new QLabel ( "theme" ), 0, 1, 1, 2 );
    geyesl->addWidget ( new QLabel ( "eye size" ), 2, 3, 0, 1 );
    geyesl->addWidget ( new QLabel ( "mirror size" ), 2, 3, 1, 2 );
    geyesl->addWidget ( new QLabel ( "width" ), 2, 3, 2, 3 );
    geyesl->addWidget ( new QLabel ( "height" ), 2, 3, 3, 4 );
    geyes->setLayout  ( geyesl );

    QGridLayout * glookerl = new QGridLayout ();
    glookerl->addWidget ( new QLabel ( "delta x" ), 0, 2, 0, 1 );
    glookerl->addWidget ( new QLabel ( "min" ), 0, 1, 1, 2 );
    glookerl->addWidget ( new QLabel ( "max" ), 0, 1, 2, 3 );
    glookerl->addWidget ( new QLabel ( "delta y" ), 2, 4, 0, 1 );
    glookerl->addWidget ( new QLabel ( "min" ), 2, 3, 1, 2 );
    glookerl->addWidget ( new QLabel ( "max" ), 2, 3, 2, 3 );
    glookerl->addWidget ( new QLabel ( "delay" ), 4, 6, 0, 1 );
    glookerl->addWidget ( new QLabel ( "min" ), 4, 5, 1, 2 );
    glookerl->addWidget ( new QLabel ( "max" ), 4, 5, 2, 3 );
    glooker->setLayout  ( glookerl );

    QGridLayout * gclapperl = new QGridLayout ();
    gclapperl->addWidget ( new QLabel ( "delay" ), 0, 2, 0, 1 );
    gclapperl->addWidget ( new QLabel ( "min" ), 0, 1, 1, 2 );
    gclapperl->addWidget ( new QLabel ( "max" ), 0, 1, 2, 3 );
    gclapper->setLayout  ( gclapperl );
}
