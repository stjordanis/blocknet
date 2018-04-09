#pragma once

#include "uint256.h"
#include <string>

namespace xbridge {
namespace eth {

bool get_balance(std::string rpc_user, std::string rpc_password,
        std::string rpc_ip, std::string rpc_port,
        std::string account, uint256& balance);

} // namespace eth
} // namespace xbridge
