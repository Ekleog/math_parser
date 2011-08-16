#ifndef EXPRFWD_HPP_INCLUDED__
#define EXPRFWD_HPP_INCLUDED__ 1

#include <memory>

typedef double ExprValue;

struct Expression;

typedef std::shared_ptr<Expression> ExprPtr;
typedef std::shared_ptr<const Expression> ConstExprPtr;

typedef std::vector<ConstExprPtr> ConstExprList;

#endif

