//******************************************************************************
//******************************************************************************
#include "xrouterutils.h"

#include "rpcprotocol.h"

//*****************************************************************************
//*****************************************************************************
namespace xrouter
{
bool is_number(std::string s)
{
    try {
        std::string::size_type idx;
        int res = std::stoi(s, &idx);
        if (res < 0)
            throw "";
        if (idx != s.size())
            throw "";
    } catch(...) {
        return false;
    }
    
    return true;
}

bool is_hash(std::string s)
{
    std::string symbols = "0123456789abcdef";
    for (size_t i = 0; i < s.size(); i++)
        if (symbols.find(s[i]) == std::string::npos)
            return false;
    return true;
}

bool is_address(std::string s)
{
    for (size_t i = 0; i < s.size(); i++)
        if (!std::isalnum(s[i]))
            return false;
    if (s.size() < 30)
        return false;
    return true;
}

// We need this to allow zero CAmount in xrouter
CAmount to_amount(double val)
{
    if (val < 0.0 || val > 21000000.0)
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    double tmp = val * COIN;
    CAmount nAmount = (int64_t)(tmp > 0 ? tmp + 0.5 : tmp - 0.5);
    if (!MoneyRange(nAmount))
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    return nAmount;
}

} // namespace xrouter
