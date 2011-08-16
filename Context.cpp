#include "Context.hpp"

#include <string>
#include <vector>

#include "Expression.hpp"
#include "Op0.hpp"

std::vector<std::string> Context::dummyArgs;

Context::Context() {}

bool Context::has(const std::string & n) const
{
    return variables_.find(n) != variables_.end();
}
ExprValue Context::get(const std::string & n) const
{
    auto it = variables_.find(n);
    if (it == variables_.end()) {
        return 0;
    } else {
        return it->second;
    }
}
void Context::set(const std::string & n, ExprValue e)
{
    variables_[n] = e;
}

bool Context::hasFunc(const std::string & n) const
{
    return functions_.find(n) != functions_.end();
}
const std::vector<std::string> & Context::funcArgs(const std::string & n)
    const
{
    auto it = functions_.find(n);
    if (it == functions_.end()) {
        return dummyArgs;
    } else {
        return it->second.first;
    }
}
ConstExprPtr Context::getFunc(const std::string & n) const
{
    auto it = functions_.find(n);
    if (it == functions_.end()) {
        return New<Const>(0);
    } else {
        return it->second.second;
    }
}
void Context::setFunc(
        const std::string & name,
        const std::vector<std::string> & args,
        ConstExprPtr val
        )
{
    functions_[name] = {args, val};
}
