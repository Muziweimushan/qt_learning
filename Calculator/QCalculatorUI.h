#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class QCalculatorUI : public QWidget
{
  Q_OBJECT
public:
  static QCalculatorUI *createNew(void);

  void show(void);
  ~QCalculatorUI();
private:
  QPushButton *m_btn[20];
  QLineEdit *m_lineedit;

  bool initialize(void);
private slots:
  void onButtonClicked(void);

private:
  QCalculatorUI();
};
