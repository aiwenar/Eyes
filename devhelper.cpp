#include "devhelper.h"
#include "ui_devhelper.h"

devHelper::devHelper (QWidget *parent) :
    QWidget(parent),
    ui(new Ui::devHelper)
{
    ui->setupUi(this);
}

devHelper::~devHelper()
{
    delete ui;
}

