#pragma once

#include <QWidget>
#include <QPushButton>

class Widget : public QWidget
{
  Q_OBJECT
public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
private:
  QPushButton m_test_btn;

private slots:
  void testBtnClicked(void);
};
