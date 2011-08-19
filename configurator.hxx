#ifndef _eyes_configurator_hxx
#define _eyes_configurator_hxx

#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

class Configurator : public QDialog
{
    Q_OBJECT
public:
    explicit    Configurator    ( QWidget * parent = 0 );
    void        set_icon        ( QIcon * icn );
private:
    QIcon         * icon;
    QTabWidget    * tabs;
};

class ConfTabUi : public QWidget
{
    Q_OBJECT
public:
    explicit    ConfTabUi   ( QWidget * parent = 0 );
private:
    QGroupBox     * geyes,
                  * glooker,
                  * gclapper;
    QComboBox     * ccolor,
                  * ctheme;
    QSpinBox      * seye,
                  * smirror,
                  * swidth,
                  * sheight,
                  * sldxmax,
                  * sldxmin,
                  * sldymax,
                  * sldymin,
                  * sldmax,
                  * sldmin,
                  * scdmax,
                  * scdmin;
};

#endif //eyes_configurator_hxx
