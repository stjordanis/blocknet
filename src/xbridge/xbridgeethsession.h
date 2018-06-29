#ifndef XBRIDGEETHSESSION_H
#define XBRIDGEETHSESSION_H

#include "xbridgeethencoder.h"

class EthSession
{
public:
    bool initiate(const bytes & hashedSecret, const bytes & responderAddress, const uint256 & refundDuration);
    bool respond(const bytes & hashedSecret, const bytes & initiatorAddress, const uint256 & refundDuration);
    bool refund(const bytes & hashedSecret);
    bool redeem(const bytes & hashedSecret, const bytes & secret);

    bool isInitiated(const bytes & hashedSecret, const bytes & initiatorAddress, const bytes & responderAddress, const uint256 value);
    bool isResponded(const bytes & hashedSecret, const bytes & initiatorAddress, const bytes & responderAddress, const uint256 value);
    bool isRefunded(const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value);
    bool isRedeemed(const bytes & hashedSecret, const bytes & recipientAddress, const uint256 value);
};

#endif // XBRIDGEETHSESSION_H
