#include <QDebug>
#include <QStack>

#include "QCalculatorDec.h"

QCalculatorDec::QCalculatorDec()
{
  m_expresion = "";
  m_result = "";

  QQueue<QString> res = split("+9.11 + ( -3 - 1 ) * -5 ");
  for (int i = 0; i < res.length(); i++) {
    qDebug() << res[i];
  }
}

bool QCalculatorDec::isDigitOrDot(const QChar &c)
{
  return ((c >= '0' && c <= '9') || ('.' == c));
}

bool QCalculatorDec::isSymbol(const QChar &c)
{
  return (isOperator(c) || (c == '(') || (c == ')'));
}

bool QCalculatorDec::isOperator(const QString &c)
{
  return ((c == "+") || (c == "-") || (c == "*") || (c == "/"));
}

bool QCalculatorDec::isSign(const QChar &c)
{
  return ((c == '+') || (c == '-'));
}

bool QCalculatorDec::isLeft(const QString &s)
{
  return ("(" == s);
}

bool QCalculatorDec::isRight(const QString &s)
{
  return (")" == s);
}
/*
 *  功能函数:将中缀表达式中数字和符号进行分离
 *    一个中缀表达式中有效字符应当分为以下4类:
 *      数字和小数点[0-9或.]
 *      符号位[+ 或 -]
 *      运算符[+, -, *, /]
 *      括号[( 或 )]
 *
 *   算法思路:对输入表达式逐个扫描,如果是符号位和数字(包括小数点),则缓存起来,如果是括号或运算符,则可以分离数来数字和符号了，分别缓存起来
 *   此处有一个关键点，如何区分符号位和运算符中的加法减法?
 *      +9 - -8.0 * (-3 * 3) - 10
 *    上述表达式已经包含所有情况,其中 +9 -8.0 -3前面的加号/减号不是运算符,分别代表三种情况，满足这三种情况之一则说明是正负号
 *      表达式的第一个字符
 *      操作符的后一个字符
 *      左括号的后一个字符
 *    从上述的归纳可看出来，实际上都是通过分析当前+/-的前一个字符来进行判断(将前一个符号初始化为""即可判断首字符情况)
 *
 */
QQueue<QString> QCalculatorDec::split(const QString &exp)
{
  QQueue<QString> ret;
  QString num = ""; /*多个数字字符组成一个操作数，这里先缓存中间的数字字符*/
  QString prev = "";

  for (int i = 0; i < exp.length(); i++) {
    if (isDigitOrDot(exp[i])) {
      /*如果是数字或小数点,则续接至中间值sum中*/
      num += exp[i];
      prev = exp[i];
    } else if (isSymbol(exp[i])) {
      /*到此可以将num变量缓存的数字先分离出来了*/
      if (!num.isEmpty()) {
        ret.enqueue(num);
        num.clear();
      }

      /*区分符号是操作符还是正负号*/
      if (isSign(exp[i]) && ((isOperator(prev)) || ("(" == prev) || ("" == prev))) {
        num += exp[i];
      } else {
        /*不是正负号，那么就是操作符，直接将操作符存入输出队列即可*/
        ret.enqueue(exp[i]);
      }

      prev = exp[i];
    }
  }

  /*最后还得检查num变量是否缓存了值,如果有则是表达式中最后一个操作数*/
  if (!num.isEmpty()) {
    ret.enqueue(num);
  }

  return ret;
}

bool QCalculatorDec::match(const QQueue<QString> &exp)
{
  bool ret = true;
  QStack<QString> s;

  for (int i = 0; i < exp.length(); i++) {
    if (isLeft(exp[i])) {
      s.push(exp[i]);
    } else if (isRight(exp[i])) {
      if (!s.isEmpty() && isLeft(s.top())) {
        s.pop();
      } else {
        ret = false;
        break;
      }
    }
  }

  return ret;
}

/*
 *  功能函数:中缀表达式转换为后缀表达式
 *  算法思路:
 *    转换过程类似于编译过程:
 *      四则运算表达式中的括号必须匹配(优先检查项，后缀表达式中不含括号，因此括号必须成对出现，同时括号指定了运行关系，因此也需要能定位括号的位置)
 *      根据运算符优先级进行转换(由子函数priority实现优先级判断)
 *      转换后的表达式中不再含有括号
 *      转换后可以顺序的计算出最终结果
 * 
 *    转换过程:
 *      当前元素e为数字:直接输出(直接输出的意思是当前元素是后缀表达式的一部分)
 *      当前元素e为运算符:
 *        1.与栈顶运算符进行优先级比较
 *        2.小于等于:将栈顶运算符输出，转步骤1
 *        3.大于：将当前元素e入栈
 *      当前元素是左括号，入栈
 *      当前元素是右括号:
 *        1.弹出栈顶元素并输出，直至栈顶元素为左括号
 *        2.将栈顶左括号从栈中弹出
 * 
 *      问题点:整个转换过程括号必须匹配,否则栈的操作就会出问题,于是需要先对输入的求值表达式先判断括号是否匹配(match函数)
*/
bool QCalculatorDec::transform(QQueue<QString> &exp, QQueue<QString> &output)
{
  bool ret = true;



  return;
}

QCalculatorDec::~QCalculatorDec()
{
}