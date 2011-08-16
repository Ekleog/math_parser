#ifndef OP2_HPP_INCLUDED__
#define OP2_HPP_INCLUDED__ 1

#include "Expression.hpp"

template <typename Op>
class Op2 : public Expression
{
public:
    Op2(ConstExprPtr lhs, ConstExprPtr rhs, const Op & op = Op())
        : lhs_(lhs), rhs_(rhs), op_(op)
    { }

    virtual ConstExprList subs() const
    {
        return {lhs_, rhs_};
    }

    virtual ExprValue operator () (const Context & c) const
    {
        return op_((*lhs_)(c), (*rhs_)(c));
    }

private:
    ConstExprPtr lhs_, rhs_;
    Op op_;
};

#include <functional>

typedef Op2<std::plus<ExprValue>>       Plus;
typedef Op2<std::minus<ExprValue>>      Minus;
typedef Op2<std::multiplies<ExprValue>> Times;

#include <limits>

// Pas std::divides, parce qu'elle n'est pas "securisee" pour N/0
struct DividesOp {
    ExprValue operator () (ExprValue a, ExprValue b) const {
        if (b == 0) {
            if (std::numeric_limits<ExprValue>::has_infinity)
                return std::numeric_limits<ExprValue>::infinity();
            else
                return 0;
        }
        return a / b;
    }
};
typedef Op2<DividesOp> Divides;

#include <cmath>

struct ModuloOp {
    ExprValue operator () (ExprValue a, ExprValue b) const {
        if (b == 0) {
            if (std::numeric_limits<ExprValue>::has_infinity)
                return std::numeric_limits<ExprValue>::infinity();
            else
                return 0;
        }
        return std::fmod(a, b);
    }
};
typedef Op2<ModuloOp> Modulo;

struct PowOp {
    ExprValue operator () (ExprValue a, ExprValue b) const {
        return std::pow(a, b);
    }
};
typedef Op2<PowOp> Pow;

#endif

