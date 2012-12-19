#ifndef DEVHELPER_H
#define DEVHELPER_H

#include "core.hxx"

namespace Ui {
class devHelper;
}

class devHelper : public QWidget
{
    Q_OBJECT
public:
    explicit devHelper(QWidget *parent = 0);
    ~devHelper();

private:
    Ui::devHelper *ui;
};

#endif // DEVHELPER_H
