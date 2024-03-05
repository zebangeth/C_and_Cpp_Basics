#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <cstdlib>
#include <iostream>
#include <sstream>

class Expression {
 public:
  Expression() {};
  virtual std::string toString() const = 0;
  
  virtual ~Expression() {};
};

class NumExpression : public Expression {
 private:
  long num; 
 public: 
  NumExpression(long n) : num(n) {}; 
  virtual std::string toString() const {
    std::stringstream numStr;
    numStr << num;
    return numStr.str();
  }
  
  virtual ~NumExpression() {}
};

class PlusExpression : public Expression{
 private:
  Expression * a;
  Expression * b;
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : a(lhs), b(rhs) {}; 
  virtual std::string toString() const{
    std::stringstream numStr;
    numStr << "(" << a->toString() << " + " << b->toString() << ")";
    return numStr.str();  }

  virtual ~PlusExpression(){
    delete a;
    delete b;
  }
}; 

class MinusExpression : public Expression{
 private:
  Expression * a;
  Expression * b;
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : a(lhs), b(rhs) {}; 
  virtual std::string toString() const{
    std::stringstream numStr;
    numStr << "(" << a->toString() << " - " << b->toString() << ")";
    return numStr.str();  }

  virtual ~MinusExpression(){
    delete a;
    delete b;
  }
}; 

class TimesExpression : public Expression{
 private:
  Expression * a;
  Expression * b;
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : a(lhs), b(rhs) {}; 
  virtual std::string toString() const{
    std::stringstream numStr;
    numStr << "(" << a->toString() << " * " << b->toString() << ")";
    return numStr.str();  }

  virtual ~TimesExpression(){
    delete a;
    delete b;
  }
}; 

class DivExpression : public Expression{
 private:
  Expression * a;
  Expression * b;
 public:
  DivExpression(Expression * lhs, Expression * rhs) : a(lhs), b(rhs) {}; 
  virtual std::string toString() const{
    std::stringstream numStr;
    numStr << "(" << a->toString() << " / " << b->toString() << ")";
    return numStr.str();  }

  virtual ~DivExpression(){
    delete a;
    delete b;
  }
}; 
