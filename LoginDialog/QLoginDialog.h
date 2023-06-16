#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QString>

class QLoginDialog : public QDialog
{
  Q_OBJECT
public:
  QLoginDialog(QWidget *parent = nullptr);
  ~QLoginDialog();

  QString getUsername(void);
  QString getPassword(void);

private slots:
  void loginBtnClicked(void);
  void cancelBtnClicked(void);

private:
  QString m_username;
  QString m_password;
  QLabel m_label_username;
  QLabel m_label_password;
  QLineEdit m_edit_username;
  QLineEdit m_edit_password;
  QPushButton m_btn_login;
  QPushButton m_btn_cancel;
};