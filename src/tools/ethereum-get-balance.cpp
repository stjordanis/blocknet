#include "xbridge/ethereum/rpc.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cerr << "Usage: " << argv[0] << " ADDRESS" << std::endl;
        return 1;
    }
    uint256 balance;
    bool success = xbridge::eth::get_balance(
            "", "", "127.0.0.1", "8545", argv[1], balance);
    if (!success) {
        std::cerr << "RPC failed" << std::endl;
        return 1;
    }
    std::cout << balance.ToString() << std::endl;
    return 0;
}
