var ACCT = artifacts.require("ACCT");

contract('ACCT', function(accounts) {
    it("Should swap 10 Ether (initiator)", function() {
        var acct;
        var secret = "zzz";
        var hash = "0xb2241f38404eef4c16fb75124e5a680d601cb5fc";
        var amount = web3.toWei(10, "ether");
        var hasEvInitiated = false;
        var hasEvRedeemed = false;
        var initiator = accounts[0];
        var initiator_balance_before;
        var initiator_balance_after;
        var responder = accounts[1];
        var responder_balance_before;
        var responder_balance_after;

        ACCT.deployed().then(function(instance) {
            acct = instance;
            return web3.eth.getBalance(initiator);
            
        }).then(function(balance) {
            initiator_balance_before = new BigNumber(balance);
            return web3.eth.getBalance(responder);

        }).then(function(balance) {
            responder_balance_before = new BigNumber(balance);
            return acct.initiate(hash, responder, 60,
                    {from: initiator, value: amount});

        }).then(function(result) {
            for (var i = 0; (i < result.logs.length) && !hasEvInitiated; i++) {
                var log = result.logs[i];
                if (    (log.event == "Initiated")
                     && (log.args.hasOwnProperty("hashedSecret"))
                     && (log.args.hashedSecret == hash)
                     && (log.args.hasOwnProperty("initiator"))
                     && (log.args.initiator == initiator)
                     && (log.args.hasOwnProperty("responder"))
                     && (log.args.responder ==  responder)
                     && (log.args.hasOwnProperty("value"))
                     && (log.args.value == amount)) {
                     hasEvInitiated = true;
                }
            }
            return web3.eth.getBalance(initiator);

        }).then(function(balance) {
            initiator_balance_after = new BigNumber(balance);
            return acct.redeem(hash, secret, {from: responder});

        }).then(function(result) {
            for (var i = 0; (i < result.logs.length) & !hasEvRedeemed; i++) {
                var log = result.logs[i];
                if (    (log.event == "Redeemed")
                     && (log.args.hasOwnProperty("hashedSecret"))
                     && (log.args.hashedSecret == hash)
                     && (log.args.hasOwnProperty("recipient"))
                     && (log.args.recipient == responder)
                     && (log.args.hasOwnProperty("value"))
                     && (log.args.value == amount)) {
                     hasEvRedeemed = true;
                }
            }
            return web3.eth.getBalance(responder);

        }).then(function(balance) {
            responder_balance_after = new BigNumber(balance);
        });

        assert.isTrue(hasEvInitiated, "No 'Initiated' event received");
        assert.isTrue(hasEvRedeemed, "No 'Redemmed' event received");
        assert.isAtMost(initiator_balance_after,
                initiator_balance_before - amount,
                "Initiator balance did not decrease");
        assert.isAtLeast(responder_balance_after,
                responder_balance_before + (amount * 0.9),
                "Responder balance did not increase");
    });
});
