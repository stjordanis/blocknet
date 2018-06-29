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

public:
    // reimplement for currency
    std::string fromXAddr(const std::vector<unsigned char> & xaddr) const;
    std::vector<unsigned char> toXAddr(const std::string & addr) const;

public:
    bool requestAddressBook(std::vector<wallet::AddressBookEntry> & entries);

    bool getUnspent(std::vector<wallet::UtxoEntry> & inputs) const;

    bool lockCoins(const std::vector<wallet::UtxoEntry> & inputs, const bool lock = true) const;

    bool getRawTransaction(const std::string & /*txid*/, const bool /*verbose*/, std::string & /*tx*/);

    bool getNewAddress(std::string & /*addr*/);

    bool getTxOut(wallet::UtxoEntry & entry);

    bool sendRawTransaction(const std::string & rawtx,
                            std::string & txid,
                            int32_t & errorCode);

    bool signMessage(const std::string & address, const std::string & message, std::string & signature);

    bool verifyMessage(const std::string & address, const std::string & message, const std::string & signature);

public:
    bool newKeyPair(std::vector<unsigned char> & pubkey, std::vector<unsigned char> & privkey);

    std::vector<unsigned char> getKeyId(const std::vector<unsigned char> & /*pubkey*/);
    std::vector<unsigned char> getScriptId(const std::vector<unsigned char> & /*script*/);
    std::string scriptIdToString(const std::vector<unsigned char> & /*id*/) const;

    double minTxFee1(const uint32_t inputCount, const uint32_t outputCount);
    double minTxFee2(const uint32_t inputCount, const uint32_t outputCount);

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

    bool initiate(const bytes & hashedSecret, const bytes & responderAddress, const uint256 & refundDuration);
    bool respond(const bytes & hashedSecret, const bytes & initiatorAddress, const uint256 & refundDuration);
    bool refund(const bytes & hashedSecret);
    bool redeem(const bytes & hashedSecret, const bytes & secret);

    bool installFilter(const bytes & hashedSecret, uint256 & filterId);
    bool deleteFilter(const uint256 & filterId);

    bool isInitiated(const uint256 & filterId, const bytes & hashedSecret, const bytes & initiatorAddress, const bytes & responderAddress, const uint256 value);
    bool isResponded(const uint256 & filterId, const bytes & hashedSecret, const bytes & initiatorAddress, const bytes & responderAddress, const uint256 value);
    bool isRefunded(const uint256 & filterId, const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value);
    bool isRedeemed(const uint256 & filterId, const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value);

private:
    const std::string contractAddress = "";
};

}

#endif // XBRIDGEWALLETCONNECTORETH_H
