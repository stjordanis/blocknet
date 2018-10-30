#include "data/currency_trades.json.h"

#include "rpcserver.h"
#include "rpcclient.h"

#include "base58.h"
#include "netbase.h"

#include <xbridge/xbridgeapp.h>
#include <xbridge/util/xseries.h>

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_utils.h"
#include "json/json_spirit_writer_template.h"

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>

using namespace std;
using namespace json_spirit;

extern Value CallRPC(string args);
extern Array read_json(const std::string& jsondata);

namespace {
    uint64_t get_seconds(int year, int month, int day) {
        auto time = ptime{boost::gregorian::date{year, month, day}} - from_time_t(0);
        return time.total_seconds();
    }
    std::string get_time_str(int y, int m, int d) { return std::to_string(get_seconds(y, m, d)); }
    std::string rpc_to_str(const std::string& cmd) {
        return write_string(CallRPC(cmd), json_spirit::single_line_arrays, 6);
    }
    std::vector<CurrencyPair> test_trades{};
    std::vector<CurrencyPair> find_blockchain_trading_data(time_period query)
    {
        const auto& begin = test_trades.begin();
        const auto& end = test_trades.end();

        const auto low = std::lower_bound(begin, end, query.begin(),
                                    [](const CurrencyPair& a, const ptime& b) {
                                        return a.timeStamp < b; });
        const auto up = std::upper_bound(low, end, query.end(),
                                   [](const ptime& period_end, const CurrencyPair& b) {
                                       return period_end <= b.timeStamp; });
        std::vector<CurrencyPair> records{low, up};
        std::sort(records.begin(), records.end(), // ascending by updated time
                  [](const CurrencyPair& a, const CurrencyPair& b) {
                      return a.timeStamp < b.timeStamp; });
        return records;
    }
    std::vector<CurrencyPair> find_local_trading_data(const xQuery& query)
    {
        (void)query;
        return {};
    }
    void setup_dx_get_order_history()
    {
        constexpr ccy::Basis basis{1000000};
        const auto& trades = json_tests::currency_trades;
        std::string trades_string{trades, trades + sizeof(trades)};
        Array tradingdata = read_json(trades_string);
        BOOST_FOREACH(Value& tv, tradingdata)
        {
            Object trade = tv.get_obj();
            ccy::Asset from{ccy::Currency{find_value(trade, "from").get_str(),basis},
                    static_cast<ccy::Amount>(find_value(trade, "fromAmount").get_real()*basis)};
            ccy::Asset to{ccy::Currency{find_value(trade, "to").get_str(),basis},
                    static_cast<ccy::Amount>(find_value(trade, "toAmount").get_real()*basis)};
            test_trades.emplace_back(find_value(trade, "txid").get_str(), from, to,
                                     ptime{from_time_t(find_value(trade, "timestamp").get_uint64())});
        }
        std::sort(test_trades.begin(), test_trades.end(),
                  [](const CurrencyPair& a, const CurrencyPair& b) {
                      return a.timeStamp < b.timeStamp; });

        auto& xseries = xbridge::App::instance().getXSeriesCache();
        xseries.set_find_blockchain_trades(find_blockchain_trading_data);
        xseries.set_find_local_trades(find_local_trading_data);
    }
}


BOOST_AUTO_TEST_SUITE(dxapi_tests)


BOOST_AUTO_TEST_CASE(dx_get_local_tokens)
{
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetLocalTokens SYS LTC"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetLocalTokens"));
}

BOOST_AUTO_TEST_CASE(dx_get_network_tokens)
{
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetNetworkTokens SYS 1 []"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetNetworkTokens"));
}

BOOST_AUTO_TEST_CASE(dx_get_token_balances)
{
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetTokenBalances txid"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetTokenBalances"));
}

BOOST_AUTO_TEST_CASE(dx_make_order)
{
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS LTC "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS 1 LTC 1 dryrun "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS LTC "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS LTC "), runtime_error);
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp -1 LTC LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP 0.01 "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxMakeOrder SYS 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp -1 LTC LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP 0.0 "), runtime_error);
    BOOST_CHECK_NO_THROW(CallRPC("dxMakeOrder SYS 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp 1 LTC LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP 0.01 "));
    BOOST_CHECK_NO_THROW(CallRPC("dxMakeOrder SYS 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp 2 LTC LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP 0.02 dryrun"));
#endif /* FIXME(unit test) */
}

BOOST_AUTO_TEST_CASE(dx_take_order)
{
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxTakeOrder SYS LTC "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxTakeOrder SYS 06cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a5965 1 LTC 1 dryrun "), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxTakeOrder 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp 06cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a5965 LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxTakeOrder 06cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a5965 1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp  LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP"));
    BOOST_CHECK_NO_THROW(CallRPC("dxTakeOrder 5eed087e8ef3b3c91a5c6e302d1f8b037543a40beb5d69a3158be4a5181608cb LZwAVhrTUkYspqRjcCGGiFHMcWNxxsgnqP  1NDqZ7piDqyDhNveWS48kDSwPdyJLEEcCp"));
}

BOOST_AUTO_TEST_CASE(dx_get_my_orders)
{
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetMyOrders txid"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetMyOrders"));
    const Value value = CallRPC("dxGetOrderFills LTC SYS");
    const Array arr = value.get_array();
    for(const Value& tv : arr)
    {
        const Object order = tv.get_obj();
        BOOST_CHECK(find_value(order, "id").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "maker").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "maker_address").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "maker_size").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "taker").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "taker_size").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "taker_address").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "updated_at").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "created_at").get_str().empty() == false);
    }
}

#if 0 /* FIXME(unit test) */
BOOST_AUTO_TEST_CASE(dx_cancel_order)
{
    BOOST_CHECK_THROW(CallRPC("dxCancelOrder txid"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxCancelOrder"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxCancelOrder 06cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a5965"), runtime_error);
}
#endif /* FIXME(unit test) */

#if 0 /* FIXME(unit test) */
BOOST_AUTO_TEST_CASE(dx_get_order)
{
    BOOST_CHECK_THROW(CallRPC("dxGetOrder txid"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxGetOrder"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxGetOrder 01cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a6559"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxGetOrder 01cdb308781f2729052d926868a6559"), runtime_error);
}
#endif /* FIXME(unit test) */


BOOST_AUTO_TEST_CASE(dx_get_order_fills)
{
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderFills LTC SYS"));
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderFills LTC SYS true"));
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderFills LTC SYS false"));
#endif /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetOrderFills LTC SYS no_bool"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxGetOrderFills LTC SYS 1"), runtime_error);
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetOrderFills 01cdb308781f2729052d926868a6559"), runtime_error);
#endif /* FIXME(unit test) */
    const Value value = CallRPC("dxGetOrderFills LTC SYS");
    const Array arr = value.get_array();
    for(const Value& tv : arr)
    {
        const Object order = tv.get_obj();
        BOOST_CHECK(find_value(order, "id").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "time").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "maker").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "maker_size").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "taker").get_str().empty() == false);
        BOOST_CHECK(find_value(order, "taker_size").get_str().empty() == false);
    }
}


BOOST_AUTO_TEST_CASE(dx_get_order_book)
{
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderBook 1 LTC SYS 10"));
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderBook 2 LTC SYS "));
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderBook 3 LTC SYS 50 "));
    BOOST_CHECK_NO_THROW(CallRPC("dxGetOrderBook 4 LTC SYS "));
#endif /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetOrderBook LTC SYS false"), runtime_error);
    BOOST_CHECK_THROW(CallRPC("dxGetOrderBook -1 LTC SYS no_bool"), runtime_error);
#if 0 /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetOrderBook 1 LTC SYS -1"), runtime_error);
#endif /* FIXME(unit test) */
    BOOST_CHECK_THROW(CallRPC("dxGetOrderBook SYS LTC"), runtime_error);
#if 0 /* FIXME(unit test) */
    Value value = CallRPC("dxGetOrderBook 2 LTC SYS ");
    BOOST_CHECK(find_value(value.get_obj(), "bids").get_bool() == true);
    BOOST_CHECK(find_value(value.get_obj(), "asks").get_bool() == true);
#endif /* FIXME(unit test) */
}

BOOST_AUTO_TEST_CASE(dx_get_locked_utxos)
{
	BOOST_CHECK_NO_THROW(CallRPC("dxGetLockedUtxos 01cdb308781f2729052d9d2ed4ee2ea1ee5ad0d4ef9c978796d49826868a6559"));
#if 0 /* FIXME(unit test) */
	BOOST_CHECK_THROW(CallRPC("dxGetLockedUtxos uint256()"), runtime_error);
	BOOST_CHECK_THROW(CallRPC("dxGetLockedUtxos 01cdb308781f978796d49826868a6559"), runtime_error);
#endif /* FIXME(unit test) */
}


BOOST_AUTO_TEST_CASE(dx_get_order_history)
{
    setup_dx_get_order_history();

    std::string response{};
    BOOST_CHECK_NO_THROW(response = rpc_to_str("dxGetOrderHistory BLOCK LTC " + get_time_str(2018,9,24) +  " " + get_time_str(2018,9,30) + " 86400"));
    BOOST_CHECK(response==
                "[\n"
                "    [ \"2018-09-24T00:00:00Z\", 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 ],\n"
                "    [ \"2018-09-25T00:00:00Z\", 0.012001, 0.028311, 0.012881, 0.012001, 0.000948 ],\n"
                "    [ \"2018-09-26T00:00:00Z\", 0.010000, 0.010000, 0.010000, 0.010000, 0.000010 ],\n"
                "    [ \"2018-09-27T00:00:00Z\", 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 ],\n"
                "    [ \"2018-09-28T00:00:00Z\", 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 ],\n"
                "    [ \"2018-09-29T00:00:00Z\", 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 ]\n"
                "]");
    BOOST_CHECK_NO_THROW(response = rpc_to_str("dxGetOrderHistory BLOCK LTC " + get_time_str(2018,9,25) +  " " + get_time_str(2018,9,27) + " 86400"));
    BOOST_CHECK(response==
                "[\n"
                "    [ \"2018-09-25T00:00:00Z\", 0.012001, 0.028311, 0.012881, 0.012001, 0.000948 ],\n"
                "    [ \"2018-09-26T00:00:00Z\", 0.010000, 0.010000, 0.010000, 0.010000, 0.000010 ]\n"
                "]");
    BOOST_CHECK_NO_THROW(response = rpc_to_str("dxGetOrderHistory BLOCK LTC " + get_time_str(2018,9,25) +  " " + get_time_str(2018,9,27) + " 86401"));
    BOOST_CHECK(response==
                "{\n"
                "    \"error\" : \"Invalid parameters: granularity=86401 must be one of: 60,300,900,3600,21600,86400\",\n"
                "    \"code\" : 1025,\n"
                "    \"name\" : \"dxGetOrderHistory\"\n"
                "}");
    BOOST_CHECK_NO_THROW(response = rpc_to_str("dxGetOrderHistory BLOCK LTC " + get_time_str(2018,9,25) +  " " + get_time_str(2018,9,27) + " 21600 true"));
    BOOST_CHECK(response==
                "[\n"
                "    [\n        \"2018-09-25T00:00:00Z\",\n        0.012881,\n        0.028311,\n        0.012881,\n        0.028311,\n        0.000798,\n"
                "        [ \"373965686da91833db4fa9d5acfb3928c410b5caea349ba4333843e5674a0829\", \"f3e8a5ab54caafe98b0d3ac5f775d88ea218213b70eed7547cbf467920d27ba8\" ]\n    ],\n"
                "    [\n        \"2018-09-25T06:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-25T12:00:00Z\",\n        0.012001,\n        0.012001,\n        0.012001,\n        0.012001,\n        0.000150,\n"
                "        [ \"3a8610444b6c091c2752c53b6f32fc3740134effdb07615733c28fb46bbf1e87\" ]\n    ],\n"
                "    [\n        \"2018-09-25T18:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-26T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-26T06:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-26T12:00:00Z\",\n        0.010000,\n        0.010000,\n        0.010000,\n        0.010000,\n        0.000010,\n"
                "        [ \"18aa87b9c7c8e13b64e7958f1daae41730f0c22f8a8dbc0b4a39ccd7948cd676\" ]\n    ],\n"
                "    [\n        \"2018-09-26T18:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ]\n"
                "]");
    BOOST_CHECK_NO_THROW(response = rpc_to_str("dxGetOrderHistory BLOCK SYS " + get_time_str(2018,9,25) +  " " + get_time_str(2018,10,22) + " 86400 true true"));
    BOOST_CHECK(response==
                "[\n"
                "    [\n        \"2018-09-25T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-26T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-27T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-28T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-29T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-09-30T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-01T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-02T00:00:00Z\",\n        183.673469,\n        183.673469,\n        183.673469,\n        183.673469,\n        0.009000,\n"
                "        [ \"eda29c055fbad93c16e4ba8963c584e898dcf923d6b72710df9cddd9fd49f368\" ]\n    ],\n"
                "    [\n        \"2018-10-03T00:00:00Z\",\n        45.226131,\n        45.226131,\n        45.226131,\n        45.226131,\n        0.018000,\n"
                "        [ \"a9e920201e05493dabb00fb00168d059dbe439403cc0cdf0d84bb06b937ed787\", \"52a15a65f4504aa5798f005c0c746c21282fe3a879e340919ab683e3613bacc6\" ]\n    ],\n"
                "    [\n        \"2018-10-04T00:00:00Z\",\n        0.714286,\n        50.505051,\n        10.101010,\n        0.714286,\n        0.016188,\n"
                "        [ \"e11fc50043474c62153cc914d811e7ebe27a11e07baabe817407937f111ebef5\", \"db924d9e20fb140a3a2e9f7381aa3cf14e875771f36ad95ff1dc9bdb69cec887\", \"49f45179fd9edfb38c622d71b57db7642923394b7979b384edbde26eba1584c1\", \"8bded8dd33fc5a5575d37a3c44fe31bb9a0756871022a0eb56d18bd34f241963\", \"3f3b1da90b47bee9f277f5306c71f29bc50d4b2b34e622258f6826df48b567b8\", \"c97c3e53cc2f11b3970678afa86734d01ee807d8770fafee56937fe014ef0536\", \"045efacd29e46d7b8b3a5482b1a5daabeefc5d8fa63434ed2e94ee2ceae1a42d\", \"75da4767ee9fa4795bc9cb2e0ae8be22c93ced9337ac7fcd494c7523108d394a\", \"e593ddbc3809a306df37d6f529060228013cd889c545a84f26f820075e892cf2\", \"f29e1f7a23a32f1c014e225fa1c5f185b4a8b1e0fb4e821f5196f824ed28dc22\", \"800cd58f76fe94993c84a21f1c9271ba465659e97e5433ef127f9af9043333f8\" ]\n    ],\n"
                "    [\n        \"2018-10-05T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-06T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-07T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-08T00:00:00Z\",\n        10.101010,\n        50.591926,\n        10.101010,\n        49.900200,\n        0.027107,\n"
                "        [ \"6d8762553e966e7fd9ea44b5c51fb11f0530a219d65a38e4a0d8821b9930c6ff\", \"53c53fc59ffebf510a199421a8e28ceeaa4c1c8a8d7522ff35b354eccf4b6bc5\", \"83d86b80ce62916ef83a3c110cb994b8cf45a74505b9b03c1dee14a2423495b5\", \"be41e9f1705c3721d806d3c5e2f1a6e6a06d2df3ca9fd1f72dcf4cffac8e1d20\", \"e6d7ccf19f57b124a42b6ceb9a30b9f28a4a97c787717b8f61525feaba1a8e22\", \"616e1a4b1febccf2d5c24b73cea11c0049c2fb874bbc9b91aeda0654205f9ada\", \"ba1a58bb1ea655444f8864d07bf499c85bcb51d52d4f48a4b73645f8fbe0d054\", \"a3347f102bc05d2674052f699875154a43086e4e6f123d4a0ff2bd461ebbceaf\", \"76f3ebd03f47931a2211ad284072d1ea779de8fe4d936f4ea030ae9b5b59a51d\", \"67db700b230a68531858deddfd6bbb009fabeb312be1fba6e43058b0f7ebae57\" ]\n    ],\n"
                "    [\n        \"2018-10-09T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-10T00:00:00Z\",\n        1.000000,\n        48.130653,\n        1.000000,\n        44.083936,\n        0.039177,\n"
                "        [ \"578a2bf9b418fd952a0eda7501dccc0aa5040b493429e588024fe1c08dc974be\", \"300821b2eaa78ff348d2a5473c7f3394254488f7f6677852a69b7e1f0361ac78\", \"0a50dc6269e18e21a5d0efda185594f6ac7f55fc862991b22ceb1c75cc2b2d0a\", \"868c41764f1fca9f0049dd1d1260edc19658ac589e7b5a4bb879d647a6ab6dd5\", \"78051add5cc1f3f0560e14dc3cd3f5de04a956529ebb823537233ef65c6078da\", \"c6af8f285a9accd44ed503498aecedf488d8c92f085173349faca2b91c0605ec\", \"161444d2d3d2f4815287c4745bd4bb34dee121305944a0a4697ca5ea6d48a3fc\", \"46b37b912cbe128d5b68fd52215c1a8618ab1becd8ae019b86680d022d7f9f54\", \"51e731b0965b8b5ec71d2dc6dc3816c33c9f300860367c61be6c4db7c6b01ace\", \"604fbbb6ff09fad07468256e5f00c98d3f40f247f08693e2aae52cdd2b128a83\", \"2279285f77f18effbe8a2ae994982edcbcb5579f6bb4e93c873d4701c46c7699\", \"e61e41718be48be47976d6cee8ade5f08adf75f1dd77bdcbb660e555601c9c29\", \"405f147f52f1f00a6ad3b14fde51f1a329af5be0385e2695c4710dacea8be083\" ]\n    ],\n"
                "    [\n        \"2018-10-11T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-12T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-13T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-14T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-15T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-16T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-17T00:00:00Z\",\n        1.000000,\n        1.000000,\n        1.000000,\n        1.000000,\n        0.000398,\n"
                "        [ \"ff025ed59d35628cde91a50dacc20766b00bbe25a711053b8ea577682e6c2112\", \"ee0a26c7a071413b505f1fffc989041ab872f793ec5f6b3aa592072814f18aea\" ]\n    ],\n"
                "    [\n        \"2018-10-18T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-19T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-20T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ],\n"
                "    [\n        \"2018-10-21T00:00:00Z\",\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        0.000000,\n        [ ]\n    ]\n"
                "]");

	BOOST_CHECK_THROW(CallRPC("dxGetOrderHistory SYS LTC 22.03.18-0:00:00 22.03.18-0:01:00 86400"), runtime_error);
#if 0 /* FIXME(unit test) */
	BOOST_CHECK_THROW(CallRPC("dxGetOrderHistory SYS LTC 1519540000 1529540000 -200"), runtime_error);
#endif /* FIXME(unit test) */
}

BOOST_AUTO_TEST_SUITE_END()
