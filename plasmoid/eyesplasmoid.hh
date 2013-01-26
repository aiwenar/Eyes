#pragma once

#include <kicon.h>
#include <plasma/applet.h>

class EyesPlasmoid : public Plasma::Applet
{
  Q_OBJECT

public:
  EyesPlasmoid  ( QObject * parent, const QVariantList &args );
  ~EyesPlasmoid ();

  void  paintInterface  ( QPainter * painter, const QStyleOptionGraphicsItem * option, const QRect &contentsRect );
  void  init            ();
};
