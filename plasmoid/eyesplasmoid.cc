#include "eyesplasmoid.hh"

EyesPlasmoid::EyesPlasmoid (QObject *parent, const QVariantList &args)
: Plasma::Applet ( parent, args )
{
  setBackgroundHints ( DefaultBackground );

}

EyesPlasmoid::~EyesPlasmoid ()
{
  if ( hasFailedToLaunch () )
  {}
  else
  {}
}

void EyesPlasmoid::init ()
{
}

void EyesPlasmoid::paintInterface ( QPainter * painter, const QStyleOptionGraphicsItem * option, const QRect &contentsRect )
{
}

K_EXPORT_PLASMA_APPLET ( eyes, EyesPlasmoid )
