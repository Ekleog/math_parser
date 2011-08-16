#ifndef OP0_HPP_INCLUDED__
#define OP0_HPP_INCLUDED__ 1

#include <string>

#include "Expression.hpp"

class Op0 : public Expression {
    ExprValue val_;

public:
    Op0(ExprValue val)
        : val_(val)
    { }

    virtual ConstExprList subs() const
    {
        return { };
    }

    virtual ExprValue operator () (const Context &) const
    {
        return val_;
    }
};

class Variable : public Expression {
    std::string name_;

public:
    Variable(const std::string & name) : name_(name)
    { }

    virtual ConstExprList subs() const
    {
        return { };
    }
    
    virtual ExprValue operator () (const Context & c) const
    {
        return c.get(name_);
    }
};

#include <sstream>

struct Const : public Op0 {
    Const(ExprValue val)
        : Op0(val)
    { }
};

struct Pi : public Op0 {
    Pi()
        : Op0(3.141592653589793238462643383279)
    { } // Thanks http://jlsigrist.com/pi.html for the value.
};

struct E : public Op0 {
    E()
        : Op0(2.718281828459045235360287471352)
    { } // Thanks http://jlsigrist.com/e.html for the value.
};

#endif

