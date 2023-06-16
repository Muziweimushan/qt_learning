#include <QDebug>

#include "QLoginDialog.h"

QLoginDialog::QLoginDialog(QWidget *parent)
  : QDialog(parent),
    m_username(""),
    m_password(""),
    m_label_username(this),
    m_label_password(this),
    m_edit_username(this),
    m_edit_password(this),
    m_btn_login(this),
    m_btn_cancel(this)
{
    m_label_username.setText("User ID:");
    m_label_username.move(20, 30);
    m_label_username.resize(60, 25);

    m_edit_username.move(85, 30);
    m_edit_username.resize(180, 25);

    m_label_password.setText("Password:");
    m_label_password.move(20, 65);
    m_label_password.resize(60,25);

    m_edit_password.move(85, 65);
    m_edit_password.resize(180, 25);
    m_edit_password.setEchoMode(QLineEdit::Password);

    m_btn_cancel.setText("Cancel");
    m_btn_cancel.move(85, 110);
    m_btn_cancel.resize(85, 30);

    m_btn_login.setText("Login");
    m_btn_login.move(180, 110);
    m_btn_login.resize(85, 30);

    setWindowTitle("Login");
    setFixedSize(285, 170);

    QObject::connect(&m_btn_login, SIGNAL(clicked()), this, SLOT(loginBtnClicked()));
    QObject::connect(&m_btn_cancel, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()));
}

void QLoginDialog::loginBtnClicked(void)
{
  qDebug() << "loginBtnClicked ...";
  /*用一个内部成员保存用户的输入,by liweibin:为什么不直接返回QLineEdit的text呢???*/
  m_username = m_edit_username.text().trimmed();
  m_password = m_edit_password.text();
  QDialog::done(QDialog::Accepted);
  
  qDebug() << "loginBtnClicked end ...";
}

void QLoginDialog::cancelBtnClicked(void)
{
  qDebug() << "cancelBtnClicked ...";

  QDialog::done(QDialog::Rejected);

  qDebug() << "cancelBtnClicked end ...";
}

QString QLoginDialog::getUsername(void)
{
  return m_edit_username.text();
}

QString QLoginDialog::getPassword(void)
{
  return m_edit_password.text();
}


QLoginDialog::~QLoginDialog()
{
  qDebug() << "~QLoginDialog() ...";
}