#ifndef OP1_HPP_INCLUDED__
#define OP1_HPP_INCLUDED__ 1

#include "Expression.hpp"

template <typename Op>
class Op1 : public Expression
{
public:
    Op1(ConstExprPtr exp, const Op & op = Op())
        : exp_(exp), op_(op)
    { }

    virtual ConstExprList subs() const
    {
        return {exp_};
    }

    virtual ExprValue operator () (const Context & c) const
    {
        return op_((*exp_)(c));
    }

private:
    ConstExprPtr exp_;
    Op op_;
};

#include <cmath>
#include <limits>

struct SqrtOp {
    ExprValue operator () (ExprValue v) const {
        if (v < 0) {
            if (std::numeric_limits<ExprValue>::has_infinity) {
                return std::numeric_limits<ExprValue>::infinity();
            } else {
                return 0;
            }
        }
        return std::sqrt(v);
    }
};
typedef Op1<SqrtOp> Sqrt;

struct CosOp {
    ExprValue operator () (ExprValue v) const {
        return std::cos(v);
    }
};
typedef Op1<CosOp> Cos;

struct SinOp {
    ExprValue operator () (ExprValue v) const {
        return std::sin(v);
    }
};
typedef Op1<SinOp> Sin;

struct TanOp {
    ExprValue operator () (ExprValue v) const {
        return std::tan(v);
    }
};
typedef Op1<TanOp> Tan;

struct LnOp {
    ExprValue operator () (ExprValue v) const {
        if (v <= 0) {
            if (std::numeric_limits<ExprValue>::has_infinity) {
                return std::numeric_limits<ExprValue>::infinity();
            } else {
                return 0;
            }
        }
        return std::log(v);
    }
};
typedef Op1<LnOp> Ln;

#endif

