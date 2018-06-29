#include "xbridgeethsession.h"

bool EthSession::initiate(const bytes& hashedSecret, const bytes& responderAddress, const uint256& refundDuration)
{

}

bool EthSession::respond(const bytes& hashedSecret, const bytes& initiatorAddress, const uint256& refundDuration)
{

}

bool EthSession::refund(const bytes& hashedSecret)
{

}

bool EthSession::redeem(const bytes& hashedSecret, const bytes& secret)
{

}

bool EthSession::isInitiated(const bytes& hashedSecret, const bytes& initiatorAddress, const bytes& responderAddress, const uint256 value)
{

}

bool EthSession::isResponded(const bytes& hashedSecret, const bytes& initiatorAddress, const bytes& responderAddress, const uint256 value)
{

}

bool EthSession::isRefunded(const bytes& hashedSecret, const bytes& recipientAddress, const uint256 value)
{

}

bool EthSession::isRedeemed(const bytes& hashedSecret, const bytes& recipientAddress, const uint256 value)
{

}
