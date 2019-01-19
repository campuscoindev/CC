// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2030 The CC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include "base58.h"
#include "bignum.h"
#include <assert.h>

#include <boost/assign/list_of.hpp>


using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    CBlock genesis;

    genesis.vtx.push_back(txNew);
    //genesis.hashPrevBlock.SetNull();
    genesis.hashPrevBlock=0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "campus genesis block";
	const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
		(0, uint256("0x"))
		;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

std::map<int, int> masternodeTiers = boost::assign::map_list_of (1, 500000) (2, 1000000) (3, 2000000);

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xd7;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0xa4;
        pchMessageStart[3] = 0xe6;
        vAlertPubKey = ParseHex("04a06bd2fbe2a058241779cfbdc1f2943148a4dad94f5038e64e30d00f8481043d3582001eaa81133c0618033b1243e7856e147551cbce5c010a92f997bb9619cf");
        //switch time for the old to new wallet
        nSwitchTime = 1550188800;//February 15th 2019
        nPremineTime = 1550188800;//February 15th 2019
        nSwitchBlock = 760000;
        nDefaultPort = 28195;
        bnProofOfWorkLimit =  ~uint256(0) >> 20; // CC starting difficulty is 1 / 2^12  //20
        //nLastPOWBlock = 900000;
        nLastPOWBlock = 770000;
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nMaturity = 15; 
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 3300000000 * COIN;
        nMaxMoneyOutOld = 2000000000 * COIN;
        int32_t nVersion = 1;
        uint32_t nTime = 1499360969;
        uint32_t nNonce = 876192;
        uint32_t nBits = 0x1e0ffff0;

        const CAmount& genesisReward = 1 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x2cadd7467aa4ca55a8be084aa8779da4f250981cbd7a7e377a4502bb58e5578b"));
        assert(genesis.hashMerkleRoot == uint256("0xfe793560988c86f809f33e79d78065962b0b4622d9d1e8f4c441c189b122cee0"));

        vSeeds.push_back(CDNSSeedData("seed1","CBPDS01.campuscoinproject.dns-cloud.net"));
        vSeeds.push_back(CDNSSeedData("seed2","CBPDS03.campuscoinproject.dns-cloud.net"));
        vSeeds.push_back(CDNSSeedData("seed3","CBPDS05.campuscoinproject.dns-cloud.net"));
        vSeeds.push_back(CDNSSeedData("seed4","CBPDS01.privatedns.org"));
        vSeeds.push_back(CDNSSeedData("seed5","CBPDS03.privatedns.org"));
        vSeeds.push_back(CDNSSeedData("seed6","CBPDS05.privatedns.org"));


        /**vSeeds.push_back(CDNSSeedData("seed1","149.28.86.37"));
        vSeeds.push_back(CDNSSeedData("seed2","45.32.195.245"));
        vSeeds.push_back(CDNSSeedData("seed3","194.88.105.45"));
        vSeeds.push_back(CDNSSeedData("seed4","192.99.224.33"));
        vSeeds.push_back(CDNSSeedData("seed5","217.69.15.209"));*/






	    base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 28);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 5);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 156);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x6d).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0451da181f2aa16784748ef435c4ea704a029a4a8e1c1893eb7c6045b328a4f536105373bfb682305b3b733dc84e2f046e10bdaaabea052cc348552db9898b0c91";
		nStartMasternodePayments = 1550188800; //02/15/2019 @ 12:00am (UTC)

		CBitcoinAddress address;
		std::string strAddress = "CTeD5GHUUEw5cs2fPKEkwimSJH1eNQwa6y";
        strFoundersAddress = "CNPL8EmrF22ZRXTUs1zfY8QWX3QjLkcyWb";
        strDevFeeAddress = "CXi3wiNRQACvdyFT21zZznht5p5RFmnAV3";
		SelectParams(CBaseChainParams::MAIN);
		assert(address.SetString(strAddress));
		masternodeTestDummyAddress = GetScriptForDestination(address.Get());
	}

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xbc;
        pchMessageStart[1] = 0xad;
        pchMessageStart[2] = 0xaf;
        pchMessageStart[3] = 0xc4;

        vAlertPubKey = ParseHex("04a06bd2fbe2a058241779cfbdc1f2943148a4dad94f5038e64e30d00f8481043d3582001eaa81133c0618033b1243e7856e147551cbce5c010a92f997bb9619cf");
        nDefaultPort = 33813;
        bnProofOfWorkLimit =  ~uint256(0) >> 20; // CC starting difficulty is 1 / 2^12  //20
        nLastPOWBlock = 100;
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nMaturityNew = 15;
        nMaturity = 60;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 3300000000 * COIN;
        int32_t nVersion = 1;
        uint32_t nTime = 1388880557;
        uint32_t nNonce = 387006691;
        uint32_t nBits = 0x1e0ffff0;
        const CAmount& genesisReward = 1 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0xb78197f0e175697646db1f738edc1ffdcb30588ebe70e7e16026489076577061"));
        assert(genesis.hashMerkleRoot == uint256("0xfe793560988c86f809f33e79d78065962b0b4622d9d1e8f4c441c189b122cee0"));

        vSeeds.push_back(CDNSSeedData("seed1","CBTDS01.campuscoinproject.dns-cloud.net"));
        vSeeds.push_back(CDNSSeedData("seed2","CBTDS02.campuscoinproject.dns-cloud.net"));
        vSeeds.push_back(CDNSSeedData("seed3","CBTDS03.campuscoinproject.dns-cloud.net"));
        /**
        vSeeds.push_back(CDNSSeedData("seed1","149.28.206.48"));
        vSeeds.push_back(CDNSSeedData("seed2","45.55.89.248"));
        vSeeds.push_back(CDNSSeedData("seed3","149.28.86.37"));*/



        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 136);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0xAD)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0451da181f2aa16784748ef435c4ea704a029a4a8e1c1893eb7c6045b328a4f536105373bfb682305b3b733dc84e2f046e10bdaaabea052cc348552db9898b0c91";
        nStartMasternodePayments = 1546344000; //01/01/2019 @ 12:00am (UTC)

        CBitcoinAddress address;
        std::string strAddress = "CTeD5GHUUEw5cs2fPKEkwimSJH1eNQwa6y";
        strFoundersAddress = "CNPL8EmrF22ZRXTUs1zfY8QWX3QjLkcyWb";
        strDevFeeAddress = "CXi3wiNRQACvdyFT21zZznht5p5RFmnAV3";
        SelectParams(CBaseChainParams::TESTNET);
        assert(address.SetString(strAddress));
        masternodeTestDummyAddress = GetScriptForDestination(address.Get());
       
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {

        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x76;
        pchMessageStart[1] = 0x9d;
        pchMessageStart[2] = 0x5e;
        pchMessageStart[3] = 0xd7;

        vAlertPubKey = ParseHex("04a06bd2fbe2a058241779cfbdc1f2943148a4dad94f5038e64e30d00f8481043d3582001eaa81133c0618033b1243e7856e147551cbce5c010a92f997bb9619cf");
        nDefaultPort = 33814;
        bnProofOfWorkLimit =  ~uint256(0) >> 20; // CC starting difficulty is 1 / 2^12  //20
        nLastPOWBlock = 1;
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nMaturity = 15;
        nMasternodeCountDrift = 20;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 3300000000 * COIN;
        int32_t nVersion = 1;
        uint32_t nTime = 1388880557;
        uint32_t nNonce = 387006691;
        uint32_t nBits = 0x1e0ffff0;
        const CAmount& genesisReward = 1 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256("0xfe793560988c86f809f33e79d78065962b0b4622d9d1e8f4c441c189b122cee0"));

        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.



        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 121);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 197);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 146);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0xAD)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

       
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
