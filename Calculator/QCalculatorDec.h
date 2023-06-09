#pragma once

#include <QQueue>
#include <QString>

/*
* 计算器求值类,负责对10进制中缀表达式进行求值
* 思路:
*   表达式求值的一般思路，就是将人使用的中缀表达式转换成计算机使用的后缀表达式(逆波兰式),然后对后缀表达式进行计算即可
*   总体分为3步:
*     1.将中缀表达式中的数字和符号分离开(split函数)
*     2.中缀表达式转后缀表达式(transform函数)
*     3.通过后缀表达式计算最终结果(calculate函数)
*/
class QCalculatorDec
{
public:
  QCalculatorDec();
  ~QCalculatorDec();

  /*外部接口:对用户输入的表达式进行求值，求值成功返回true*/
  bool expression(const QString &exp);
  QString result(void);

private:
  QString m_expresion;
  QString m_result;

  QQueue<QString> split(const QString &exp);
  bool transform(QQueue<QString> &exp, QQueue<QString> &output);
  bool match(const QQueue<QString> &exp);
  QString evaluate(QQueue<QString> &exp);

  bool isDigitOrDot(const QChar &c);
  bool isSymbol(const QChar &c);
  bool isOperator(const QString &c);
  bool isSign(const QChar &c);
  bool isLeft(const QString &s);
  bool isRight(const QString &s);
  bool isNumber(const QString &s);

  int priority(const QString &s);

  /*功能函数:四则运算的实现,当计算出错或输入运算数非法时返回error*/
  QString calculate(const QString &op1, const QString &oprand, const QString &op2);
};