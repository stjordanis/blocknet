//*****************************************************************************
//*****************************************************************************

#include "xbridgewalletconnector.h"
#include "xbridgetransactiondescr.h"
#include "base58.h"

//*****************************************************************************
//*****************************************************************************
namespace xbridge
{

//*****************************************************************************
//*****************************************************************************
namespace wallet
{

//*****************************************************************************
//*****************************************************************************
std::string UtxoEntry::toString() const
{
    std::ostringstream o;
    o << txId << ":" << vout << ":" << amount << ":" << address;
    return o.str();
}

} // namespace wallet

//*****************************************************************************
//*****************************************************************************
WalletConnector::WalletConnector()
{
}

//******************************************************************************
//******************************************************************************
bool WalletConnector::lockCoins(const std::vector<wallet::UtxoEntry> & inputs,
                                const bool lock)
{
    boost::lock_guard<boost::mutex> l(lockedCoinsLocker);

    if (!lock)
    {
        for (const wallet::UtxoEntry & entry : inputs)
        {
            lockedCoins.erase(entry);
        }
    }
    else
    {
        // check duplicates
        for (const wallet::UtxoEntry & entry : inputs)
        {
            if (lockedCoins.count(entry))
            {
                return false;
            }
        }

        lockedCoins.insert(inputs.begin(), inputs.end());
    }

    return true;
}

//******************************************************************************
//******************************************************************************
void WalletConnector::removeLocked(std::vector<wallet::UtxoEntry> & inputs) const
{
    boost::lock_guard<boost::mutex> lock(lockedCoinsLocker);

    for (auto it = inputs.begin(); it != inputs.end(); )
    {
        if (lockedCoins.count(*it))
        {
            it = inputs.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

} // namespace xbridge
