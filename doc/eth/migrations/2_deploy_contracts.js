var Acct = artifacts.require("./ACCT.sol");

module.exports = function(deployer) {
    deployer.deploy(Acct);
};
