#ifndef CONTEXT_HPP_INCLUDED__
#define CONTEXT_HPP_INCLUDED__ 1

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "ExprFwd.hpp"

class Context
{
    typedef std::pair<std::vector<std::string>, ConstExprPtr> Func;

    std::map<std::string, ExprValue> variables_;
    std::map<std::string, Func> functions_;

    static std::vector<std::string> dummyArgs;

public:
    Context();

    bool has(const std::string & n) const;
    ExprValue get(const std::string & n) const;
    void set(const std::string & n, ExprValue e);
    
    bool hasFunc(const std::string & n) const;
    const std::vector<std::string> & funcArgs(const std::string & n)
        const;
    ConstExprPtr getFunc(const std::string & n) const;
    void setFunc(
            const std::string & name,
            const std::vector<std::string> & args,
            ConstExprPtr val
            );
};

#endif

