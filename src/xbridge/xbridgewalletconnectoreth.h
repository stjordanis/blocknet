#ifndef XBRIDGEWALLETCONNECTORETH_H
#define XBRIDGEWALLETCONNECTORETH_H

#include "xbridgewalletconnector.h"
#include "xbridgeethencoder.h"

namespace xbridge
{

class EthWalletConnector : public WalletConnector
{
public:
    EthWalletConnector();

    bool init() { return true; }
public:
    // reimplement for currency
    std::string fromXAddr(const std::vector<unsigned char> & xaddr) const;
    std::vector<unsigned char> toXAddr(const std::string & addr) const;

public:
    bool getNewAddress(std::string & /*addr*/);

    bool requestAddressBook(std::vector<wallet::AddressBookEntry> & entries);

    double getWalletBalance(const std::string &addr = "") const;

    bool getInfo(rpc::WalletInfo & info) const;

    bool getUnspent(std::vector<wallet::UtxoEntry> & /*inputs*/, const bool /*withoutDust*/ = true) const;

    bool lockCoins(const std::vector<wallet::UtxoEntry> & /*inputs*/, const bool /*lock*/ = true) const;

    bool getTxOut(wallet::UtxoEntry & /*entry*/);

    bool sendRawTransaction(const std::string & /*rawtx*/,
                            std::string & /*txid*/,
                            int32_t & /*errorCode*/,
                            std::string & /*message*/);

    bool signMessage(const std::string & /*address*/, const std::string & /*message*/, std::string & /*signature*/);
    bool verifyMessage(const std::string & /*address*/, const std::string & /*message*/, const std::string & /*signature*/);

public:
    bool isDustAmount(const double & /*amount*/) const { return false; }

    bool newKeyPair(std::vector<unsigned char> & pubkey, std::vector<unsigned char> & privkey);

    std::vector<unsigned char> getKeyId(const std::vector<unsigned char> & pubkey);
    std::vector<unsigned char> getScriptId(const std::vector<unsigned char> & /*script*/);
    std::string scriptIdToString(const std::vector<unsigned char> & /*id*/) const;

    double minTxFee1(const uint32_t inputCount, const uint32_t outputCount) const;
    double minTxFee2(const uint32_t inputCount, const uint32_t outputCount) const;

    bool checkTransaction(const std::string & depositTxId,
                          const std::string & /*destination*/,
                          const uint64_t & /*amount*/,
                          bool & isGood);

    uint32_t lockTime(const char role) const;

    bool createDepositUnlockScript(const std::vector<unsigned char> & /*myPubKey*/,
                                   const std::vector<unsigned char> & /*otherPubKey*/,
                                   const std::vector<unsigned char> & /*xdata*/,
                                   const uint32_t /*lockTime*/,
                                   std::vector<unsigned char> & /*resultSript*/) { return false; }

    bool createDepositTransaction(const std::vector<std::pair<std::string, int> > & /*inputs*/,
                                  const std::vector<std::pair<std::string, double> > & /*outputs*/,
                                  std::string & /*txId*/,
                                  std::string & /*rawTx*/) { return false; }

    bool createRefundTransaction(const std::vector<std::pair<std::string, int> > & /*inputs*/,
                                 const std::vector<std::pair<std::string, double> > & /*outputs*/,
                                 const std::vector<unsigned char> & /*mpubKey*/,
                                 const std::vector<unsigned char> & /*mprivKey*/,
                                 const std::vector<unsigned char> & /*innerScript*/,
                                 const uint32_t /*lockTime*/,
                                 std::string & /*txId*/,
                                 std::string & /*rawTx*/) { return false; }

    bool createPaymentTransaction(const std::vector<std::pair<std::string, int> > & /*inputs*/,
                                  const std::vector<std::pair<std::string, double> > & /*outputs*/,
                                  const std::vector<unsigned char> & /*mpubKey*/,
                                  const std::vector<unsigned char> & /*mprivKey*/,
                                  const std::vector<unsigned char> & /*xpubKey*/,
                                  const std::vector<unsigned char> & /*innerScript*/,
                                  std::string & /*txId*/,
                                  std::string & /*rawTx*/) { return false; }

    bool getAccounts(std::vector<std::string> & accounts);
    bool getBalance(const std::string & account, uint256 & balance) const;
    bool getGasPrice(uint256 & gasPrice) const;
    bool getEstimateGas(const bytes & myAddress,
                        const bytes & data,
                        const uint256 & value,
                        uint256 & estimateGas) const;

    bytes createInitiateData(const bytes & hashedSecret,
                             const bytes & responderAddress,
                             const uint256 & refundDuration) const;

    bytes createRespondData(const bytes & hashedSecret,
                            const bytes & initiatorAddress,
                            const uint256 & refundDuration) const;

    bytes createRefundData(const bytes & hashedSecret) const;
    bytes createRedeemData(const bytes & hashedSecret, const bytes & secret) const;

    bool callContractMethod(const bytes & myAddress,
                            const bytes & data,
                            const uint256 & value,
                            const uint256 & gas,
                            uint256 & transactionHash) const;

    bool installFilter(const bytes & hashedSecret, uint256 & filterId) const;
    bool deleteFilter(const uint256 & filterId) const;

    bool isInitiated(const uint256 & filterId, const bytes & hashedSecret, bytes& initiatorAddress, const bytes & responderAddress, const uint256 value) const;
    bool isResponded(const uint256 & filterId, const bytes & hashedSecret, const bytes & initiatorAddress, bytes & responderAddress, const uint256 value) const;
    bool isRefunded(const uint256 & filterId, const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value) const;
    bool isRedeemed(const uint256 & filterId, const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value) const;

private:
    const std::string contractAddress = "0xb4573a002e0762c52ef07948bc1cb779ef4e3b10";
};

}

#endif // XBRIDGEWALLETCONNECTORETH_H
