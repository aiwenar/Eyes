#ifndef CDBG_HXX
#define CDBG_HXX

#include <QTimer>

#include "eyes.hxx"

void spacefill ( int, unsigned short );

class cdbg : public QObject
{
  Q_OBJECT
public:
    cdbg  ( Core * );

    void  chck_s ();
public slots:
    void  on_timer_tick ();

private:
    Core          * core;
    QTimer          timer;
    unsigned  short max_s,
                    line,
                    spacer;
};

#endif // CDBG_HXX
