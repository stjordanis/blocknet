//******************************************************************************
//******************************************************************************
#include "xrouterutils.h"
#include "xrouterdef.h"

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

using namespace json_spirit;

//*****************************************************************************
//*****************************************************************************
namespace xrouter
{

std::string generateDomainRegistrationTx(std::string domain, std::string addr) {
    std::string raw_tx, txid;

    Array outputs;
    Object out;
    std::string domainstr = "blocknet://" + domain;
    vector<unsigned char> domain_enc(domainstr.begin(), domainstr.end());
    out.push_back(Pair("data", HexStr(domain_enc)));
    Object out2;
    out2.push_back(Pair("address", addr));
    out2.push_back(Pair("amount", XROUTER_DOMAIN_REGISTRATION_DEPOSIT));
    outputs.push_back(out);
    outputs.push_back(out2);
    Array inputs;

    Array params;
    params.push_back(inputs);
    params.push_back(outputs);    
    bool res = createAndSignTransaction(params, raw_tx);
    if (!res)
        return "";
    
    sendTransactionBlockchain(raw_tx, txid);
    return txid;
}
    
} // namespace xrouter
