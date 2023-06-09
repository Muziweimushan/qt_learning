#include <QDebug>
#include <QStack>

#include "QCalculatorDec.h"

namespace
{
constexpr char kEvaluateErrMsg[] = "Error";
}

QCalculatorDec::QCalculatorDec()
{
  m_expresion = "";
  m_result = "";

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

bool QCalculatorDec::isNumber(const QString &s)
{
  bool ret = false;
  s.toDouble(&ret);

  return ret;
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

  return (ret && s.isEmpty());
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
 *        2.优先级小于等于栈顶运算符:将栈顶运算符输出，转步骤1----当前运算符小于等于栈顶运算符，说明可以计算栈顶运算符对应的运算了
 *        3.大于：将当前元素e入栈----不能将当前元素输出，因为暂时无法判断当前运算符后面是否还有运算符，无法确定优先级,需要先缓存起来
 *      当前元素是左括号，入栈
 *      当前元素是右括号:
 *        1.弹出栈顶元素并输出，直至栈顶元素为左括号
 *        2.将栈顶左括号从栈中弹出
 * 
 *      问题点:整个转换过程括号必须匹配,否则栈的操作就会出问题,于是需要先对输入的求值表达式先判断括号是否匹配(match函数)
*/
bool QCalculatorDec::transform(QQueue<QString> &exp, QQueue<QString> &output)
{
  bool ret = match(exp);
  QStack<QString> s;

  output.clear();

  while (ret && !exp.isEmpty()) {
    QString e = exp.dequeue();
    if (isNumber(e)) {
      output.enqueue(e);
    } else if (isOperator(e)) {
      while (!s.isEmpty() && priority(e) <= priority(s.top())) {
        output.enqueue(s.pop());
      }

      /*当前运算符入栈缓存*/
      s.push(e);
    } else if (isLeft(e)) {
      /*左括号入栈*/
      s.push(e);
    } else if (isRight(e)) {
      /*右括号*/
      while (!s.isEmpty() && !isLeft(s.top())) {
        output.enqueue(s.pop());
      }

      if (!s.isEmpty()) {
        s.pop();
      }
    } else {
      /*不应该进到这个分支,出错了*/
      ret = false;
    }
  }

  while (ret && !s.isEmpty()) {
    output.enqueue(s.pop());
  }

  return ret;
}

int QCalculatorDec::priority(const QString &s)
{
  int ret = 0;

  if ("+" == s || "-" == s) {
    ret = 1;
  } else if ("*" == s || "/" == s) {
    ret = 2;
  }

  return ret;
}

/*
 *  功能函数:对后缀表达式进行求值
 *  算法思路:
 *    遍历后缀表达式中的数字和运算符：
 *      当前元素为数字: 进栈
 *      当前元素为运算符：
 *        1.从栈中弹出右操作数
 *        2.从栈中弹出左操作数
 *        3.根据符号进行计算
 *        4.将计算结果压入栈中
 * 
 *    遍历结束：
 *      栈中的唯一数字为运算结果
 * 
 *    注意的点：
 *      1.参加运算的两个操作数的顺序，因为扫描是从前向后扫描，而栈的特性是后进先出，因此先取出来的是右操作数
 *      2.实现四则运算时要特别注意运算数是0的情况
 *      3.当遍历结束时，栈中元素个数必然为1，否则说明输入的表示式是有问题的
 *
*/
QString QCalculatorDec::evaluate(QQueue<QString> &exp)
{
  QString ret = "";
  QStack<QString> s;

  for (int i = 0; i < exp.length(); i++) {
    qDebug() << exp[i];
  }

  while (!exp.isEmpty()) {
    QString e = exp.dequeue();

    if (isNumber(e)) {
      s.push(e);
    } else if (isOperator(e)) {
      /*注意顺序，先弹出右操作数*/
      QString op2 = (!s.isEmpty()) ? s.pop() : "";
      QString op1 = (!s.isEmpty()) ? s.pop() : "";
      QString result = calculate(op1, e, op2);
      if (kEvaluateErrMsg != result) {
        s.push(result);
      } else {
        break;
      }
    } else {
      /*后缀表达式中应该只含有数字和运算符，进到这里说明有问题*/
      break;
    }
  }

  /*最后要来检查栈中元素，它必须只有一个元素，而且这个元素必须是个数字,而且输入的后缀表达式必须遍历完成*/
  if ((exp.isEmpty()) && (1 == s.length()) && (isNumber(s.top()))) {
    ret = s.pop();
  } else {
    qDebug() << "exp.empty: " << exp.isEmpty() << ", s.length: " << s.length();
    if (!exp.isEmpty()) {
      qDebug() << "exp: ";
      for (int i = 0; i < exp.length(); i++) {
        qDebug() << exp[i];
      }
    }
    ret = kEvaluateErrMsg;
  }

  return ret;
}

/*功能函数:四则运算的实现,当计算出错或输入运算数非法时返回error*/
QString QCalculatorDec::calculate(const QString &op1, const QString &oprand, const QString &op2)
{
  QString ret = kEvaluateErrMsg;

  if (isNumber(op1) && isNumber(op2)) {
    double lhs = op1.toDouble();
    double rhs = op2.toDouble();
    if ("+" == oprand) {
      ret.sprintf("%f", lhs + rhs);
    } else if ("-" == oprand) {
      ret.sprintf("%f", lhs - rhs);
    } else if ("*" == oprand) {
      ret.sprintf("%f", lhs * rhs);
    } else if ("/" == oprand) {
      /*除0操作...*/
      constexpr double esiplon = 0.0000000001;
      if (-esiplon < rhs && rhs < esiplon) {
        ret = kEvaluateErrMsg;
      } else {
        ret.sprintf("%f", lhs / rhs);
      }
    } else {
      /*不明运算符...*/
      ret = kEvaluateErrMsg;
    }
  } else {
    qInfo() << "not a number ...";
  }

  return ret;
}

/*外部接口:对用户输入表达式进行求值计算*/
bool QCalculatorDec::expression(const QString &exp)
{
  bool ret = false;
  QQueue<QString> exp_split;
  QQueue<QString> rpn;

  m_expresion = exp;
  exp_split = split(exp);
  if (transform(exp_split, rpn)) {
    m_result = evaluate(rpn);
    ret = (kEvaluateErrMsg == m_result);
  } else {
    qInfo() << "tranform to RPN failed ...";
    m_result = kEvaluateErrMsg;
  }
  return ret;
}

QString QCalculatorDec::result(void)
{
  return m_result;
}

QCalculatorDec::~QCalculatorDec()
{
}
