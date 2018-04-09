#include "rpc.h"
#include "json/json_spirit.h"
#include "json/json_spirit_writer_template.h"
#include "xbridge/util/logger.h"
#include <stdexcept>

namespace xbridge {
namespace eth {

using namespace json_spirit;

Object CallRPC(const std::string& rpc_user, const std::string& rpc_passwd,
               const std::string& rpc_ip, const std::string& rpc_port,
               const std::string& strMethod, const Array& params);

bool get_balance(std::string rpc_user, std::string rpc_password,
        std::string rpc_ip, std::string rpc_port,
        std::string account, uint256& balance)
{
    try {
        LOG() << "rpc call <get_balance(" << account << ")>";
        Array params;
        params.push_back(account);
        params.push_back("latest");
        Object reply = CallRPC(rpc_user, rpc_password, rpc_ip, rpc_port,
                "eth_getBalance", params);
        const Value& error = find_value(reply, "error");
        if (error.type() != null_type) {
            LOG() << "rpc error: " << write_string(error, false);
            return false;
        }
        const Value& result = find_value(reply, "result");
        if (result.type() != str_type) {
            LOG() << "rpc result not string: " << write_string(result, false);
            return false;
        }
        balance = uint256(result.get_str());
    } catch (std::exception& e) {
        LOG() << "get_balance exception: " << e.what();
        return false;
    }
    return true;
}

} // namespace xbridge
} // namespace eth
