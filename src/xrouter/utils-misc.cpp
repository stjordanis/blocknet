//******************************************************************************
//******************************************************************************
#include "xrouterutils.h"

#include "rpcprotocol.h"


#ifdef _WIN32
#include <objbase.h>

namespace xrouter {
std::string generateUUID()
{
    GUID guid;
	CoCreateGuid(&guid);
    char guid_string[37];
    sprintf(guid_string,
          "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
          guid.Data1, guid.Data2, guid.Data3,
          guid.Data4[0], guid.Data4[1], guid.Data4[2],
          guid.Data4[3], guid.Data4[4], guid.Data4[5],
          guid.Data4[6], guid.Data4[7]);
    return guid_string;
}
}
    
#else

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace xrouter {
std::string generateUUID()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::uuids::to_string(uuid);
}
}

#endif 

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
