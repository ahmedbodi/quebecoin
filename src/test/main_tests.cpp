// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <validation.h>
#include <net.h>

#include <test/test_bitcoin.h>

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

static void TestBlockSubsidyHalvings(const Consensus::Params& consensusParams)
{
    CAmount nSubsidy = 0.25 * COIN;
    BOOST_CHECK_EQUAL(GetBlockSubsidy(consensusParams.nSubsidyHalvingInterval, consensusParams), nSubsidy);
}

/* Myriadcoin: we don't test other intervals due to MIP3-longblocks
static void TestBlockSubsidyHalvings(int nSubsidyHalvingInterval)
{
    Consensus::Params consensusParams;
    consensusParams.nSubsidyHalvingInterval = nSubsidyHalvingInterval;
    TestBlockSubsidyHalvings(consensusParams);
}
*/

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    // Myriadcoin: we don't test other intervals due to MIP3-longblocks
    //const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const auto chainParamsMain = CreateChainParams(CBaseChainParams::MAIN);
    const auto chainParamsTestnet = CreateChainParams(CBaseChainParams::TESTNET);
    const auto chainParamsRegtest = CreateChainParams(CBaseChainParams::REGTEST);
    //TestBlockSubsidyHalvings(chainParams->GetConsensus()); // As in main
    TestBlockSubsidyHalvings(chainParamsMain->GetConsensus()); // As in main
    TestBlockSubsidyHalvings(chainParamsTestnet->GetConsensus()); // As in testnet
    TestBlockSubsidyHalvings(chainParamsRegtest->GetConsensus()); // As in regtest
    //TestBlockSubsidyHalvings(150); // As in regtest
    //TestBlockSubsidyHalvings(1000); // Just another interval
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    CAmount nSum = 0;
    // Myriadcoin: step must be integer factor of our MIP3 120960 halving intervals, all interval steps are divisible by 945
    for (int nHeight = 0; nHeight < 14000000; nHeight += 945) {
        CAmount nSubsidy = GetBlockSubsidy(nHeight, chainParams->GetConsensus());
        BOOST_CHECK(nSubsidy <= 1000 * COIN);
        nSum += nSubsidy * 945;
        BOOST_CHECK(MoneyRange(nSum));
    }
    BOOST_CHECK_EQUAL(nSum, 194452744500000000ULL);
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}
BOOST_AUTO_TEST_SUITE_END()
