#include <QDebug>

#include "Widget.h"
#include "QLoginDialog.h"

Widget::Widget(QWidget *parent)
  : QWidget(parent, Qt::WindowCloseButtonHint), m_test_btn(this)
{
  m_test_btn.resize(200, 40);
  m_test_btn.setText("Login");

  QObject::connect(&m_test_btn, SIGNAL(clicked()), this, SLOT(testBtnClicked()));
}

void Widget::testBtnClicked(void)
{
  qDebug() << "testBtnClicked ...";

  /*创建登录对话框*/
  QLoginDialog dlg;

  int result = dlg.exec();

  /*获取对话框结果*/
  if (QDialog::Accepted == result) {
    qDebug() << "login!";
    qDebug() << "username: " << dlg.getUsername();
    qDebug() << "password: " << dlg.getPassword();
  }
}

Widget::~Widget()
{
}