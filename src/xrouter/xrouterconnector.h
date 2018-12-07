#ifndef _XROUTER_CONNECTOR_H_
#define _XROUTER_CONNECTOR_H_

#include <vector>
#include <string>
#include <cstdint>
#include "json/json_spirit.h"
#include "xbridge/xbridgewallet.h"
#include "streams.h"
#include "wallet.h"
#include <boost/container/map.hpp>

using namespace json_spirit;

namespace xrouter
{

struct PaymentChannel
{
    CKey key;
    CKeyID keyid;
    std::string raw_tx;
    std::string txid;
    CScript redeemScript;
    int vout;
    CAmount value;
    CAmount deposit;
    std::string latest_tx;
    int deadline;
};
    
std::string CallCMD(std::string cmd);
std::string CallURL(std::string ip, std::string port, std::string url);

Object CallRPC(const std::string & rpcuser, const std::string & rpcpasswd,
               const std::string & rpcip, const std::string & rpcport,
               const std::string & strMethod, const Array & params);

bool createAndSignTransaction(std::string address, CAmount amount, std::string & raw_tx);
bool createAndSignTransaction(boost::container::map<std::string, CAmount> addrs, string & raw_tx);
bool createAndSignTransaction(Array txparams, std::string & raw_tx);
void unlockOutputs(std::string tx);
std::string signTransaction(std::string& raw_tx);
bool sendTransactionBlockchain(std::string raw_tx, std::string & txid);
bool sendTransactionBlockchain(std::string address, CAmount amount, std::string & raw_tx);
PaymentChannel createPaymentChannel(CPubKey address, CAmount deposit, int date);
bool createAndSignChannelTransaction(PaymentChannel channel, std::string address, CAmount deposit, CAmount amount, std::string & raw_tx);
bool verifyChannelTransaction(std::string transaction);
bool finalizeChannelTransaction(PaymentChannel channel, CKey snodekey, std::string latest_tx, std::string & raw_tx);
std::string createRefundTransaction(PaymentChannel channel);
double getTxValue(std::string rawtx, std::string address, std::string type="address");
int getChannelExpiryTime(std::string rawtx);
std::string generateDomainRegistrationTx(std::string domain, std::string addr);
CAmount to_amount(double val);
bool is_number(std::string s);
bool is_hash(std::string s);
bool is_address(std::string s);

class WalletConnectorXRouter : public xbridge::WalletParam
{
public:
    WalletConnectorXRouter();

    WalletConnectorXRouter & operator = (const WalletParam & other)
    {
        *(WalletParam *)this = other;
        return *this;
    }

    virtual std::string getBlockCount() const = 0;
    virtual Object getBlockHash(const std::string & blockId) const = 0;
    virtual Object      getBlock(const std::string & blockHash) const = 0;
    virtual Object      getTransaction(const std::string & trHash) const = 0;
    virtual Array       getAllBlocks(const int number, int blocklimit=0) const = 0;
    virtual Array       getAllTransactions(const std::string & account, const int number, const int time=0, int blocklimit=0) const = 0;
    virtual std::string getBalance(const std::string & account, const int time=0, int blocklimit=0) const = 0;
    virtual std::string getBalanceUpdate(const std::string & account, const int number, const int time=0, int blocklimit=0) const = 0;
    virtual Array       getTransactionsBloomFilter(const int number, CDataStream & stream, int blocklimit=0) const = 0;
    virtual Object      sendTransaction(const std::string & transaction) const = 0;
    virtual std::string convertTimeToBlockCount(const std::string & timestamp) const = 0;
};


} // namespace xrouter


#endif
