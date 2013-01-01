#pragma once

#include <QWidget>

class eyes_menu : public QWidget
{
  Q_OBJECT
public:
  explicit eyes_menu  ( QWidget * parent = 0 );
  
public slots:
  void  paintEvent      ( QPaintEvent * );
  void  mousePressEvent ( QMouseEvent * );
  void  mouseMoveEvent  ( QMouseEvent * );
  
signals:
  void  quitting  ();

private:
  bool  quit_hover;
  
};
