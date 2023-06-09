#include <QDebug>
#include <QString>

#include "QCalculatorUI.h"

QCalculatorUI::QCalculatorUI() : QWidget(nullptr, Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint)
{
}

QCalculatorUI *QCalculatorUI::createNew(void)
{
  QCalculatorUI *ret = new QCalculatorUI();
  if (ret && ret->initialize()) {
  } else {
    qDebug() << "Create QCalculator object failed ...";
    delete ret, ret = nullptr;
  }

  return ret;
}

bool QCalculatorUI::initialize(void)
{
  bool ret = true;
  const char *btn_labels[20] = {
    "7", "8", "9", "+", "(",
    "4", "5", "6", "-", ")",
    "1", "2", "3", "*", "<-",
    "0", ".", "=", "/", "C",
  };

  m_lineedit = new QLineEdit(this);
  m_lineedit->move(10, 10);
  m_lineedit->resize(240, 30);
  m_lineedit->setAlignment(Qt::AlignRight);
  m_lineedit->setReadOnly(true);
  
  /*创建按键*/
  /*每个按键是40x40的*/
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      QPushButton *btn = new QPushButton(this);
      btn->resize(40, 40);
      btn->move(10 + 50 * j, 50 + 50 * i);
      btn->setText(btn_labels[i * 5 + j]);

      QObject::connect(btn, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
      m_btn[i * 5 + j] = btn;
    }
  }

  QWidget::setWindowTitle("QtDemo");

  return ret;
}

void QCalculatorUI::show(void)
{
  QWidget::show();
  setFixedSize(width(), height());
}

void QCalculatorUI::onButtonClicked(void)
{
  QPushButton *btn = (QPushButton *)sender();

  qDebug() << btn->text();

  QString clicked_text = btn->text();

  if ("<-" == clicked_text) {
    QString text = m_lineedit->text();
    if (!text.isEmpty()) {
      text.remove(text.length() - 1, 1);
      m_lineedit->setText(text);
    }
  } else if ("C" == clicked_text) {
    m_lineedit->setText("");
  } else if ("=" == clicked_text) {
    qDebug() << "evaluation ...";
  } else {
    m_lineedit->setText(m_lineedit->text() + clicked_text);
  }
}

QCalculatorUI::~QCalculatorUI()
{
}
