// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_MAIN_H
#define BITCOIN_MAIN_H

#include "sync.h"
#include "net.h"
#include "script.h"
#include "scrypt.h"

#include "global_objects_noui.hpp"

#include <map>
#include <set>

class CWallet;
class CBlock;
class CBlockIndex;
class CKeyItem;
class CReserveKey;
class COutPoint;
class CAddress;
class CInv;
class CNode;
class CTxMemPool;

static const int LAST_POW_BLOCK = 2050;
extern unsigned int REORGANIZE_FAILED;
extern unsigned int CHECKPOINT_VIOLATIONS;
static const int MAX_NEWBIE_BLOCKS = 200;
static const int MAX_NEWBIE_BLOCKS_LEVEL2 = 500;
static const int CHECKPOINT_DISTRIBUTED_MODE = 50;
static const int CONSENSUS_LOOKBACK = 5;  //Amount of blocks to go back from best block, to avoid counting forked blocks
static const int BLOCK_GRANULARITY = 10;   //Consensus block divisor 

static const double NeuralNetworkMultiplier = 115000;

extern int64_t nLastBlockSolved;
extern int64_t nLastBlockSubmitted;

extern uint256 muGlobalCheckpointHashRelayed;
extern std::string msMasterProjectPublicKey;
extern std::string msMasterMessagePublicKey;
extern std::string msMasterMessagePrivateKey;
extern bool bNewUserWizardNotified;

/** The maximum allowed size for a serialized block, in bytes (network rule) */
static const unsigned int MAX_BLOCK_SIZE = 1000000;
/** Target Blocks Per day */
static const unsigned int BLOCKS_PER_DAY = 1000;
/** The maximum size for mined blocks */
static const unsigned int MAX_BLOCK_SIZE_GEN = MAX_BLOCK_SIZE/2;
/** The maximum size for transactions we're willing to relay/mine **/
static const unsigned int MAX_STANDARD_TX_SIZE = MAX_BLOCK_SIZE_GEN/5;
/** The maximum allowed number of signature check operations in a block (network rule) */
static const unsigned int MAX_BLOCK_SIGOPS = MAX_BLOCK_SIZE/50;
/** The maximum number of orphan transactions kept in memory */
static const unsigned int MAX_ORPHAN_TRANSACTIONS = MAX_BLOCK_SIZE/100;
/** The maximum number of entries in an 'inv' protocol message */
static const unsigned int MAX_INV_SZ = 50000;
/** Fees smaller than this (in satoshi) are considered zero fee (for transaction creation) */
static const int64_t MIN_TX_FEE = 10000;
/** Fees smaller than this (in satoshi) are considered zero fee (for relaying) */
static const int64_t MIN_RELAY_TX_FEE = MIN_TX_FEE;
/** No amount larger than this (in satoshi) is valid */
static const int64_t MAX_MONEY = 2000000000 * COIN;
inline bool MoneyRange(int64_t nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }
/** Threshold for nLockTime: below this value it is interpreted as block number, otherwise as UNIX timestamp. */
static const unsigned int LOCKTIME_THRESHOLD = 500000000; // Tue Nov  5 00:53:20 1985 UTC
static const unsigned int MINIMUM_CHECKPOINT_TRANSMISSION_BALANCE = 4000000;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Genesis - MainNet - Production Genesis: as of 10-20-2014:
static const uint256 hashGenesisBlock("0x000005a247b397eadfefa58e872bc967c2614797bdc8d4d0e6b09fea5c191599");

//TestNet Genesis:
static const uint256 hashGenesisBlockTestNet("0x00006e037d7b84104208ecf2a8638d23149d712ea810da604ee2f2cb39bae713");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsProtocolV2(int nHeight) 
{ 
	return (fTestNet ?  nHeight > 2060 : nHeight > 85400); 
}

inline bool IsResearchAgeEnabled(int nHeight)
{
	return (fTestNet ?  nHeight > 0 : nHeight > 364500); 
}

inline int GetSuperblockAgeSpacing(int nHeight)
{
	return (fTestNet ? 86400 : (nHeight > 364500) ? 86400 : 43200);
}

inline bool AreBinarySuperblocksEnabled(int nHeight) 
{ 
	return (fTestNet ? nHeight > 10000 : nHeight > 725000); 
}


inline int64_t PastDrift(int64_t nTime, int nHeight)   { return IsProtocolV2(nHeight) ? nTime - 20 * 60  : nTime - 20 * 60; }
inline int64_t FutureDrift(int64_t nTime, int nHeight) { return IsProtocolV2(nHeight) ? nTime + 20 * 60  : nTime + 20 * 60; }
inline unsigned int GetTargetSpacing(int nHeight) { return IsProtocolV2(nHeight) ? 90 : 60; }

extern bool IsNeuralNodeParticipant(const std::string& addr, int64_t locktime);

extern std::map<std::string, double> mvNeuralNetworkHash;
extern std::map<std::string, double> mvCurrentNeuralNetworkHash;
extern std::map<std::string, double> mvNeuralVersion;

typedef std::set<uint256> HashSet;

extern CScript COINBASE_FLAGS;
extern CCriticalSection cs_main;
extern std::map<uint256, CBlockIndex*> mapBlockIndex;
extern std::set<std::pair<COutPoint, unsigned int> > setStakeSeen;
extern CBlockIndex* pindexGenesisBlock;
extern unsigned int nStakeMinAge;
extern unsigned int nStakeMaxAge;
extern unsigned int nNodeLifespan;
extern int nCoinbaseMaturity;
extern uint256 nBestInvalidTrust;
extern unsigned int nTransactionsUpdated;
extern uint64_t nLastBlockTx;
extern uint64_t nLastBlockSize;
extern int64_t nLastCoinStakeSearchInterval;
extern const std::string strMessageMagic;
extern int64_t nTimeBestReceived;
extern CCriticalSection cs_setpwalletRegistered;
extern std::set<CWallet*> setpwalletRegistered;
extern unsigned char pchMessageStart[4];
extern std::map<uint256, CBlock*> mapOrphanBlocks;

extern bool bOPReturnEnabled;

// Settings
extern int64_t nTransactionFee;
extern int64_t nReserveBalance;
extern int64_t nMinimumInputValue;
extern int64_t nLastTallied;
extern int64_t nLastPing;
extern int64_t nLastAskedForBlocks;
extern int64_t nBootup;
extern int64_t nLastCalculatedMedianTimePast;
extern double nLastBlockAge;
extern int64_t nLastCalculatedMedianPeerCount;
extern int nLastMedianPeerCount;
extern int64_t nLastTalliedNeural;
extern int64_t nCPIDsLoaded;
extern int64_t nLastGRCtallied;
extern int64_t nLastCleaned;
extern int64_t nLastTallyBusyWait;

extern bool fUseFastIndex;
extern unsigned int nDerivationMethodIndex;

extern bool fEnforceCanonical;

// Minimum disk space required - used in CheckDiskSpace()
static const uint64_t nMinDiskSpace = 52428800;

// PoB Miner Global Vars:
extern std::string 	msMiningCPID;
extern std::string  msPrimaryCPID;

extern double       mdPORNonce;
extern double       mdLastPorNonce;
extern double       mdMachineTimerLast;

extern std::string  msMiningErrors;
extern std::string  msPoll;
extern std::string  msMiningErrors5;
extern std::string  msMiningErrors6;
extern std::string  msMiningErrors7;
extern std::string  msMiningErrors8;
extern std::string  msPeek;
extern std::string  msLastCommand;
extern std::string  msAttachmentGuid;

extern std::string  msMiningErrorsIncluded;
extern std::string  msMiningErrorsExcluded;

extern std::string  msNeuralResponse;
extern std::string  msHDDSerial;
extern bool         mbBlocksDownloaded;
extern int nGrandfather;
extern int nNewIndex;
extern int nNewIndex2;

// Stats for Main Screen:
struct globalStatusType
{
    CCriticalSection lock;
    std::string blocks;
    std::string difficulty;
    std::string netWeight;
    std::string dporWeight;
    std::string magnitude;
    std::string project;
    std::string cpid;
    std::string status;
    std::string poll;
    std::string errors;
    std::string rsaOverview;
};

extern globalStatusType GlobalStatusStruct;


class CReserveKey;
class CTxDB;
class CTxIndex;

void RegisterWallet(CWallet* pwalletIn);
void UnregisterWallet(CWallet* pwalletIn);
void SyncWithWallets(const CTransaction& tx, const CBlock* pblock = NULL, bool fUpdate = false, bool fConnect = true);
bool ProcessBlock(CNode* pfrom, CBlock* pblock, bool Generated_By_Me);
bool CheckDiskSpace(uint64_t nAdditionalBytes=0);
FILE* OpenBlockFile(unsigned int nFile, unsigned int nBlockPos, const char* pszMode="rb");
FILE* AppendBlockFile(unsigned int& nFileRet);
bool LoadBlockIndex(bool fAllowNew=true);
void PrintBlockTree();

bool ProcessMessages(CNode* pfrom);
bool SendMessages(CNode* pto, bool fSendTrickle);
bool LoadExternalBlockFile(FILE* fileIn);
bool WriteKey(std::string sKey, std::string sValue);

bool CheckProofOfWork(uint256 hash, unsigned int nBits);
unsigned int GetNextTargetRequired(CBlockIndex* pindexLast, bool fProofOfStake);
int64_t GetProofOfWorkReward(int64_t nFees, int64_t locktime, int64_t height);

int64_t GetProofOfStakeReward(int64_t nCoinAge, int64_t nFees, std::string cpid,
	bool VerifyingBlock, int VerificationPhase, int64_t nTime, CBlockIndex* pindexLast, std::string operation,
	double& OUT_POR, double& OUT_INTEREST, double& dAccrualAge, double& dMagnitudeUnit, double& AvgMagnitude);

MiningCPID DeserializeBoincBlock(std::string block, int BlockVersion);
std::string SerializeBoincBlock(MiningCPID mcpid, int BlockVersion);


unsigned int ComputeMinWork(unsigned int nBase, int64_t nTime);
unsigned int ComputeMinStake(unsigned int nBase, int64_t nTime, unsigned int nBlockTime);
int GetNumBlocksOfPeers();
bool IsInitialBlockDownload();
std::string GetWarnings(std::string strFor);
bool GetTransaction(const uint256 &hash, CTransaction &tx, uint256 &hashBlock);
uint256 WantedByOrphan(const CBlock* pblockOrphan);

CBlockIndex* GetLastBlockIndex(CBlockIndex* pindex, bool fProofOfStake);
void StakeMiner(CWallet *pwallet);
void ResendWalletTransactions(bool fForce = false);

std::string DefaultWalletAddress();


/** (try to) add transaction to memory pool **/
bool AcceptToMemoryPool(CTxMemPool& pool, CTransaction &tx,
                        bool* pfMissingInputs);

std::string GetBackupFilename(const std::string& basename, const std::string& suffix = "");

bool GetWalletFile(CWallet* pwallet, std::string &strWalletFileOut);
StructCPID GetInitializedStructCPID2(const std::string& name, std::map<std::string, StructCPID>& vRef);

/** Position on disk for a particular transaction. */
class CDiskTxPos
{
public:
    unsigned int nFile;
    unsigned int nBlockPos;
    unsigned int nTxPos;

    CDiskTxPos()
    {
        SetNull();
    }

    CDiskTxPos(unsigned int nFileIn, unsigned int nBlockPosIn, unsigned int nTxPosIn)
    {
        nFile = nFileIn;
        nBlockPos = nBlockPosIn;
        nTxPos = nTxPosIn;
    }

    IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this)); )
    void SetNull() { nFile = (unsigned int) -1; nBlockPos = 0; nTxPos = 0; }
    bool IsNull() const { return (nFile == (unsigned int) -1); }

    friend bool operator==(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return (a.nFile     == b.nFile &&
                a.nBlockPos == b.nBlockPos &&
                a.nTxPos    == b.nTxPos);
    }

    friend bool operator!=(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return !(a == b);
    }


    std::string ToString() const
    {
        if (IsNull())
            return "null";
        else
            return strprintf("(nFile=%u, nBlockPos=%u, nTxPos=%u)", nFile, nBlockPos, nTxPos);
    }

    void print() const
    {
        printf("%s", ToString().c_str());
    }
};



/** An inpoint - a combination of a transaction and an index n into its vin */
class CInPoint
{
public:
    CTransaction* ptx;
    unsigned int n;

    CInPoint() { SetNull(); }
    CInPoint(CTransaction* ptxIn, unsigned int nIn) { ptx = ptxIn; n = nIn; }
    void SetNull() { ptx = NULL; n = (unsigned int) -1; }
    bool IsNull() const { return (ptx == NULL && n == (unsigned int) -1); }
};



/** An outpoint - a combination of a transaction hash and an index n into its vout */
class COutPoint
{
public:
    uint256 hash;
    unsigned int n;

    COutPoint() { SetNull(); }
    COutPoint(uint256 hashIn, unsigned int nIn) { hash = hashIn; n = nIn; }
    IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this)); )
    void SetNull() { hash = 0; n = (unsigned int) -1; }
    bool IsNull() const { return (hash == 0 && n == (unsigned int) -1); }

    friend bool operator<(const COutPoint& a, const COutPoint& b)
    {
        return (a.hash < b.hash || (a.hash == b.hash && a.n < b.n));
    }

    friend bool operator==(const COutPoint& a, const COutPoint& b)
    {
        return (a.hash == b.hash && a.n == b.n);
    }

    friend bool operator!=(const COutPoint& a, const COutPoint& b)
    {
        return !(a == b);
    }

    std::string ToString() const
    {
        return strprintf("COutPoint(%s, %u)", hash.ToString().substr(0,10).c_str(), n);
    }

    void print() const
    {
        printf("%s\n", ToString().c_str());
    }
};




/** An input of a transaction.  It contains the location of the previous
 * transaction's output that it claims and a signature that matches the
 * output's public key.
 */
class CTxIn
{
public:
    COutPoint prevout;
    CScript scriptSig;
    unsigned int nSequence;

    CTxIn()
    {
        nSequence = std::numeric_limits<unsigned int>::max();
    }

    explicit CTxIn(COutPoint prevoutIn, CScript scriptSigIn=CScript(), unsigned int nSequenceIn=std::numeric_limits<unsigned int>::max())
    {
        prevout = prevoutIn;
        scriptSig = scriptSigIn;
        nSequence = nSequenceIn;
    }

    CTxIn(uint256 hashPrevTx, unsigned int nOut, CScript scriptSigIn=CScript(), unsigned int nSequenceIn=std::numeric_limits<unsigned int>::max())
    {
        prevout = COutPoint(hashPrevTx, nOut);
        scriptSig = scriptSigIn;
        nSequence = nSequenceIn;
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(prevout);
        READWRITE(scriptSig);
        READWRITE(nSequence);
    )

    bool IsFinal() const
    {
        return (nSequence == std::numeric_limits<unsigned int>::max());
    }

    friend bool operator==(const CTxIn& a, const CTxIn& b)
    {
        return (a.prevout   == b.prevout &&
                a.scriptSig == b.scriptSig &&
                a.nSequence == b.nSequence);
    }

    friend bool operator!=(const CTxIn& a, const CTxIn& b)
    {
        return !(a == b);
    }

    std::string ToStringShort() const
    {
        return strprintf(" %s %d", prevout.hash.ToString().c_str(), prevout.n);
    }

    std::string ToString() const
    {
        std::string str;
        str += "CTxIn(";
        str += prevout.ToString();
        if (prevout.IsNull())
            str += strprintf(", coinbase %s", HexStr(scriptSig).c_str());
        else
            str += strprintf(", scriptSig=%s", scriptSig.ToString().substr(0,24).c_str());
        if (nSequence != std::numeric_limits<unsigned int>::max())
            str += strprintf(", nSequence=%u", nSequence);
        str += ")";
        return str;
    }

    void print() const
    {
        printf("%s\n", ToString().c_str());
    }
};




/** An output of a transaction.  It contains the public key that the next input
 * must be able to sign with to claim it.
 */
class CTxOut
{
public:
    int64_t nValue;
    CScript scriptPubKey;

    CTxOut()
    {
        SetNull();
    }

    CTxOut(int64_t nValueIn, CScript scriptPubKeyIn)
    {
        nValue = nValueIn;
        scriptPubKey = scriptPubKeyIn;
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(nValue);
        READWRITE(scriptPubKey);
    )

    void SetNull()
    {
        nValue = -1;
        scriptPubKey.clear();
    }

    bool IsNull()
    {
        return (nValue == -1);
    }

    void SetEmpty()
    {
        nValue = 0;
        scriptPubKey.clear();
    }

    bool IsEmpty() const
    {
        return (nValue == 0 && scriptPubKey.empty());
    }

    uint256 GetHash() const
    {
        return SerializeHash(*this);
    }

    friend bool operator==(const CTxOut& a, const CTxOut& b)
    {
        return (a.nValue       == b.nValue &&
                a.scriptPubKey == b.scriptPubKey);
    }

    friend bool operator!=(const CTxOut& a, const CTxOut& b)
    {
        return !(a == b);
    }

    std::string ToStringShort() const
    {
        return strprintf(" out %s %s", FormatMoney(nValue).c_str(), scriptPubKey.ToString(true).c_str());
    }

    std::string ToString() const
    {
        if (IsEmpty()) return "CTxOut(empty)";
        return strprintf("CTxOut(nValue=%s, scriptPubKey=%s)", FormatMoney(nValue).c_str(), scriptPubKey.ToString().c_str());
    }

    void print() const
    {
        printf("%s\n", ToString().c_str());
    }
};




enum GetMinFee_mode
{
    GMF_BLOCK,
    GMF_RELAY,
    GMF_SEND,
};

typedef std::map<uint256, std::pair<CTxIndex, CTransaction> > MapPrevTx;

/** The basic transaction that is broadcasted on the network and contained in
 * blocks.  A transaction can contain multiple inputs and outputs.
 */
class CTransaction
{
public:
    static const int CURRENT_VERSION=1;
    int nVersion;
    unsigned int nTime;
    std::vector<CTxIn> vin;
    std::vector<CTxOut> vout;
    unsigned int nLockTime;

    // Denial-of-service detection:
    mutable int nDoS;
    bool DoS(int nDoSIn, bool fIn) const { nDoS += nDoSIn; return fIn; }
	std::string hashBoinc;

    CTransaction()
    {
        SetNull();
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(nTime);
        READWRITE(vin);
        READWRITE(vout);
        READWRITE(nLockTime);

		READWRITE(hashBoinc);
    )

    void SetNull()
    {
        nVersion = CTransaction::CURRENT_VERSION;
        nTime = GetAdjustedTime();
        vin.clear();
        vout.clear();
        nLockTime = 0;
        nDoS = 0;  // Denial-of-service prevention
		hashBoinc="";
    }

    bool IsNull() const
    {
        return (vin.empty() && vout.empty());
    }

    uint256 GetHash() const
    {
        return SerializeHash(*this);
    }

    bool IsNewerThan(const CTransaction& old) const
    {
        if (vin.size() != old.vin.size())
            return false;
        for (unsigned int i = 0; i < vin.size(); i++)
            if (vin[i].prevout != old.vin[i].prevout)
                return false;

        bool fNewer = false;
        unsigned int nLowest = std::numeric_limits<unsigned int>::max();
        for (unsigned int i = 0; i < vin.size(); i++)
        {
            if (vin[i].nSequence != old.vin[i].nSequence)
            {
                if (vin[i].nSequence <= nLowest)
                {
                    fNewer = false;
                    nLowest = vin[i].nSequence;
                }
                if (old.vin[i].nSequence < nLowest)
                {
                    fNewer = true;
                    nLowest = old.vin[i].nSequence;
                }
            }
        }
        return fNewer;
    }

    bool IsCoinBase() const
    {
        return (vin.size() == 1 && vin[0].prevout.IsNull() && vout.size() >= 1);
    }

    bool IsCoinStake() const
    {
        // ppcoin: the coin stake transaction is marked with the first output empty
        return (vin.size() > 0 && (!vin[0].prevout.IsNull()) && vout.size() >= 2 && vout[0].IsEmpty());
    }

    /** Check for standard transaction types
        @param[in] mapInputs	Map of previous transactions that have outputs we're spending
        @return True if all inputs (scriptSigs) use only standard transaction forms
        @see CTransaction::FetchInputs
    */
    bool AreInputsStandard(const MapPrevTx& mapInputs) const;

    /** Count ECDSA signature operations the old-fashioned (pre-0.6) way
        @return number of sigops this transaction's outputs will produce when spent
        @see CTransaction::FetchInputs
    */
    unsigned int GetLegacySigOpCount() const;

    /** Count ECDSA signature operations in pay-to-script-hash inputs.

        @param[in] mapInputs	Map of previous transactions that have outputs we're spending
        @return maximum number of sigops required to validate this transaction's inputs
        @see CTransaction::FetchInputs
     */
    unsigned int GetP2SHSigOpCount(const MapPrevTx& mapInputs) const;

    /** Amount of bitcoins spent by this transaction.
        @return sum of all outputs (note: does not include fees)
     */
    int64_t GetValueOut() const
    {
        int64_t nValueOut = 0;
        BOOST_FOREACH(const CTxOut& txout, vout)
        {
            nValueOut += txout.nValue;
            if (!MoneyRange(txout.nValue) || !MoneyRange(nValueOut))
                throw std::runtime_error("CTransaction::GetValueOut() : value out of range");
        }
        return nValueOut;
    }

	



    /** Amount of bitcoins coming in to this transaction
        Note that lightweight clients may not know anything besides the hash of previous transactions,
        so may not be able to calculate this.

        @param[in] mapInputs	Map of previous transactions that have outputs we're spending
        @return	Sum of value of all inputs (scriptSigs)
        @see CTransaction::FetchInputs
     */
    int64_t GetValueIn(const MapPrevTx& mapInputs) const;

    int64_t GetMinFee(unsigned int nBlockSize=1, enum GetMinFee_mode mode=GMF_BLOCK, unsigned int nBytes = 0) const;

    bool ReadFromDisk(CDiskTxPos pos, FILE** pfileRet=NULL)
    {
        CAutoFile filein = CAutoFile(OpenBlockFile(pos.nFile, 0, pfileRet ? "rb+" : "rb"), SER_DISK, CLIENT_VERSION);
        if (!filein)
            return error("CTransaction::ReadFromDisk() : OpenBlockFile failed");

        // Read transaction
        if (fseek(filein, pos.nTxPos, SEEK_SET) != 0)
            return error("CTransaction::ReadFromDisk() : fseek failed");

        try {
            filein >> *this;
        }
        catch (std::exception &e) {
            return error("%s() : deserialize or I/O error", __PRETTY_FUNCTION__);
        }

        // Return file pointer
        if (pfileRet)
        {
            if (fseek(filein, pos.nTxPos, SEEK_SET) != 0)
                return error("CTransaction::ReadFromDisk() : second fseek failed");
            *pfileRet = filein.release();
        }
        return true;
    }

    friend bool operator==(const CTransaction& a, const CTransaction& b)
    {
        return (a.nVersion  == b.nVersion &&
                a.nTime     == b.nTime &&
                a.vin       == b.vin &&
                a.vout      == b.vout &&
                a.nLockTime == b.nLockTime);
    }

    friend bool operator!=(const CTransaction& a, const CTransaction& b)
    {
        return !(a == b);
    }

    std::string ToStringShort() const
    {
        std::string str;
        str += strprintf("%s %s", GetHash().ToString().c_str(), IsCoinBase()? "base" : (IsCoinStake()? "stake" : "user"));
        return str;
    }

    std::string ToString() const
    {
        std::string str;
        str += IsCoinBase()? "Coinbase" : (IsCoinStake()? "Coinstake" : "CTransaction");
        str += strprintf("(hash=%s, nTime=%d, ver=%d, vin.size=%" PRIszu ", vout.size=%" PRIszu ", nLockTime=%d)\n",
            GetHash().ToString().substr(0,10).c_str(),
            nTime,
            nVersion,
            vin.size(),
            vout.size(),
            nLockTime);
        for (unsigned int i = 0; i < vin.size(); i++)
            str += "    " + vin[i].ToString() + "\n";
        for (unsigned int i = 0; i < vout.size(); i++)
            str += "    " + vout[i].ToString() + "\n";
        return str;
    }

    void print() const
    {
        printf("%s", ToString().c_str());
    }


    bool ReadFromDisk(CTxDB& txdb, COutPoint prevout, CTxIndex& txindexRet);
    bool ReadFromDisk(CTxDB& txdb, COutPoint prevout);
    bool ReadFromDisk(COutPoint prevout);
    bool DisconnectInputs(CTxDB& txdb);

    /** Fetch from memory and/or disk. inputsRet keys are transaction hashes.

     @param[in] txdb	Transaction database
     @param[in] mapTestPool	List of pending changes to the transaction index database
     @param[in] fBlock	True if being called to add a new best-block to the chain
     @param[in] fMiner	True if being called by CreateNewBlock
     @param[out] inputsRet	Pointers to this transaction's inputs
     @param[out] fInvalid	returns true if transaction is invalid
     @return	Returns true if all inputs are in txdb or mapTestPool
     */
    bool FetchInputs(CTxDB& txdb, const std::map<uint256, CTxIndex>& mapTestPool,
                     bool fBlock, bool fMiner, MapPrevTx& inputsRet, bool& fInvalid);

    /** Sanity check previous transactions, then, if all checks succeed,
        mark them as spent by this transaction.

        @param[in] inputs	Previous transactions (from FetchInputs)
        @param[out] mapTestPool	Keeps track of inputs that need to be updated on disk
        @param[in] posThisTx	Position of this transaction on disk
        @param[in] pindexBlock
        @param[in] fBlock	true if called from ConnectBlock
        @param[in] fMiner	true if called from CreateNewBlock
        @return Returns true if all checks succeed
     */
    bool ConnectInputs(MapPrevTx inputs,
                       std::map<uint256, CTxIndex>& mapTestPool, const CDiskTxPos& posThisTx,
                       CBlockIndex* pindexBlock, bool fBlock, bool fMiner);
    bool CheckTransaction() const;
    bool GetCoinAge(CTxDB& txdb, uint64_t& nCoinAge) const;  // ppcoin: get transaction coin age

protected:
    const CTxOut& GetOutputFor(const CTxIn& input, const MapPrevTx& inputs) const;
};

/** Check for standard transaction types
    @return True if all outputs (scriptPubKeys) use only standard transaction forms
*/
bool IsStandardTx(const CTransaction& tx);

bool IsFinalTx(const CTransaction &tx, int nBlockHeight = 0, int64_t nBlockTime = 0);



/** A transaction with a merkle branch linking it to the block chain. */
class CMerkleTx : public CTransaction
{
private:
    int GetDepthInMainChainINTERNAL(CBlockIndex* &pindexRet) const;
public:
    uint256 hashBlock;
    std::vector<uint256> vMerkleBranch;
    int nIndex;

    // memory only
    mutable bool fMerkleVerified;


    CMerkleTx()
    {
        Init();
    }

    CMerkleTx(const CTransaction& txIn) : CTransaction(txIn)
    {
        Init();
    }

    void Init()
    {
        hashBlock = 0;
        nIndex = -1;
        fMerkleVerified = false;
    }


    IMPLEMENT_SERIALIZE
    (
        nSerSize += SerReadWrite(s, *(CTransaction*)this, nType, nVersion, ser_action);
        nVersion = this->nVersion;
        READWRITE(hashBlock);
        READWRITE(vMerkleBranch);
        READWRITE(nIndex);
    )


    int SetMerkleBranch(const CBlock* pblock=NULL);

    // Return depth of transaction in blockchain:
    // -1  : not in blockchain, and not in memory pool (conflicted transaction)
    //  0  : in memory pool, waiting to be included in a block
    // >=1 : this many blocks deep in the main chain
    int GetDepthInMainChain(CBlockIndex* &pindexRet) const;
    int GetDepthInMainChain() const { CBlockIndex *pindexRet; return GetDepthInMainChain(pindexRet); }
    bool IsInMainChain() const { CBlockIndex *pindexRet; return GetDepthInMainChainINTERNAL(pindexRet) > 0; }
    int GetBlocksToMaturity() const;
    bool AcceptToMemoryPool();
};




/**  A txdb record that contains the disk location of a transaction and the
 * locations of transactions that spend its outputs.  vSpent is really only
 * used as a flag, but having the location is very helpful for debugging.
 */
class CTxIndex
{
public:
    CDiskTxPos pos;
    std::vector<CDiskTxPos> vSpent;

    CTxIndex()
    {
        SetNull();
    }

    CTxIndex(const CDiskTxPos& posIn, unsigned int nOutputs)
    {
        pos = posIn;
        vSpent.resize(nOutputs);
    }

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(pos);
        READWRITE(vSpent);
    )

    void SetNull()
    {
        pos.SetNull();
        vSpent.clear();
    }

    bool IsNull()
    {
        return pos.IsNull();
    }

    friend bool operator==(const CTxIndex& a, const CTxIndex& b)
    {
        return (a.pos    == b.pos &&
                a.vSpent == b.vSpent);
    }

    friend bool operator!=(const CTxIndex& a, const CTxIndex& b)
    {
        return !(a == b);
    }
    int GetDepthInMainChain() const;

};





/** Nodes collect new transactions into a block, hash them into a hash tree,
 * and scan through nonce values to make the block's hash satisfy proof-of-work
 * requirements.  When they solve the proof-of-work, they broadcast the block
 * to everyone and the block is added to the block chain.  The first transaction
 * in the block is a special one that creates a new coin owned by the creator
 * of the block.
 *
 * Blocks are appended to blk0001.dat files on disk.  Their location on disk
 * is indexed by CBlockIndex objects in memory.
 */
class CBlock
{
public:
    // header
    static const int CURRENT_VERSION = 8;
    int nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    unsigned int nTime;
    unsigned int nBits;

    unsigned int nNonce;
	
    // network and disk
    std::vector<CTransaction> vtx;

    // ppcoin: block signature - signed by one of the coin base txout[N]'s owner
    std::vector<unsigned char> vchBlockSig;




    // memory only
    mutable std::vector<uint256> vMerkleTree;

    // Denial-of-service detection:
    mutable int nDoS;
    bool DoS(int nDoSIn, bool fIn) const { nDoS += nDoSIn; return fIn; }

	//Gridcoin - 7/27/2014
	/////////////////////////////////////////
	
	
    CBlock()
    {
        SetNull();
    }

    IMPLEMENT_SERIALIZE
    (
		//Gridcoin
	   
	    //


        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
	

        // ConnectBlock depends on vtx following header to generate CDiskTxPos
        if (!(nType & (SER_GETHASH|SER_BLOCKHEADERONLY)))
        {
            READWRITE(vtx);

            READWRITE(vchBlockSig);

			/*
			
			*/



        }
        else if (fRead)
        {
            const_cast<CBlock*>(this)->vtx.clear();
            const_cast<CBlock*>(this)->vchBlockSig.clear();
        }





    )

    void SetNull()
    {
        nVersion = CBlock::CURRENT_VERSION;
        hashPrevBlock = 0;
        hashMerkleRoot = 0;
        nTime = 0;
        nBits = 0;
        nNonce = 0;
        vtx.clear();
        vchBlockSig.clear();
	    vMerkleTree.clear();
        nDoS = 0;
    }

    bool IsNull() const
    {
        return (nBits == 0);
    }

    uint256 GetHash() const
    {
        if (nVersion > 6)
            return Hash(BEGIN(nVersion), END(nNonce));
        else
            return GetPoWHash();
    }

    uint256 GetPoWHash() const
    {
        return scrypt_blockhash(CVOIDBEGIN(nVersion));
    }

    int64_t GetBlockTime() const
    {
        return (int64_t)nTime;
    }

    void UpdateTime(CBlockIndex* pindexPrev);

    // entropy bit for stake modifier if chosen by modifier
    unsigned int GetStakeEntropyBit() const
    {
        // Take last bit of block hash as entropy bit
        unsigned int nEntropyBit = ((GetHash().Get64()) & 1llu);
        if (fDebug && GetBoolArg("-printstakemodifier"))
            printf("GetStakeEntropyBit: hashBlock=%s nEntropyBit=%u\n", GetHash().ToString().c_str(), nEntropyBit);
        return nEntropyBit;
    }

    // ppcoin: two types of block: proof-of-work or proof-of-stake
    bool IsProofOfStake() const
    {
        return (vtx.size() > 1 && vtx[1].IsCoinStake());
    }

    bool IsProofOfWork() const
    {
        //must at least contain coinbase
        return !vtx.empty() && !IsProofOfStake();
    }

    std::pair<COutPoint, unsigned int> GetProofOfStake() const
    {
        return IsProofOfStake()? std::make_pair(vtx[1].vin[0].prevout, vtx[1].nTime) : std::make_pair(COutPoint(), (unsigned int)0);
    }

    // ppcoin: get max transaction timestamp
    int64_t GetMaxTransactionTime() const
    {
        int64_t maxTransactionTime = 0;
        BOOST_FOREACH(const CTransaction& tx, vtx)
            maxTransactionTime = std::max(maxTransactionTime, (int64_t)tx.nTime);
        return maxTransactionTime;
    }

    uint256 BuildMerkleTree() const
    {
        vMerkleTree.clear();
        BOOST_FOREACH(const CTransaction& tx, vtx)
            vMerkleTree.push_back(tx.GetHash());
        int j = 0;
        for (int nSize = vtx.size(); nSize > 1; nSize = (nSize + 1) / 2)
        {
            for (int i = 0; i < nSize; i += 2)
            {
                int i2 = std::min(i+1, nSize-1);
                vMerkleTree.push_back(Hash(BEGIN(vMerkleTree[j+i]),  END(vMerkleTree[j+i]),
                                           BEGIN(vMerkleTree[j+i2]), END(vMerkleTree[j+i2])));
            }
            j += nSize;
        }
        return (vMerkleTree.empty() ? 0 : vMerkleTree.back());
    }

    std::vector<uint256> GetMerkleBranch(int nIndex) const
    {
        if (vMerkleTree.empty())
            BuildMerkleTree();
        std::vector<uint256> vMerkleBranch;
        int j = 0;
        for (int nSize = vtx.size(); nSize > 1; nSize = (nSize + 1) / 2)
        {
            int i = std::min(nIndex^1, nSize-1);
            vMerkleBranch.push_back(vMerkleTree[j+i]);
            nIndex >>= 1;
            j += nSize;
        }
        return vMerkleBranch;
    }

    static uint256 CheckMerkleBranch(uint256 hash, const std::vector<uint256>& vMerkleBranch, int nIndex)
    {
        if (nIndex == -1)
            return 0;
        BOOST_FOREACH(const uint256& otherside, vMerkleBranch)
        {
            if (nIndex & 1)
                hash = Hash(BEGIN(otherside), END(otherside), BEGIN(hash), END(hash));
            else
                hash = Hash(BEGIN(hash), END(hash), BEGIN(otherside), END(otherside));
            nIndex >>= 1;
        }
        return hash;
    }


    void print() const
    {
        printf("CBlock(hash=%s, ver=%d, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%" PRIszu ", vchBlockSig=%s)\n",
            GetHash().ToString().c_str(),
            nVersion,
            hashPrevBlock.ToString().c_str(),
            hashMerkleRoot.ToString().c_str(),
            nTime, nBits, nNonce,
            vtx.size(),
            HexStr(vchBlockSig.begin(), vchBlockSig.end()).c_str());
        for (unsigned int i = 0; i < vtx.size(); i++)
        {
            printf("  ");
            vtx[i].print();
        }
        printf("  vMerkleTree: ");
        for (unsigned int i = 0; i < vMerkleTree.size(); i++)
            printf("%s ", vMerkleTree[i].ToString().substr(0,10).c_str());
        printf("\n");
    }


    bool WriteToDisk(unsigned int& nFileRet, unsigned int& nBlockPosRet);
    bool ReadFromDisk(unsigned int nFile, unsigned int nBlockPos, bool fReadTransactions=true);
    bool DisconnectBlock(CTxDB& txdb, CBlockIndex* pindex);
    bool ConnectBlock(CTxDB& txdb, CBlockIndex* pindex, bool fJustCheck=false, bool fReorganizing=false);
    bool ReadFromDisk(CBlockIndex* pindex, bool fReadTransactions=true);
    bool SetBestChain(CTxDB& txdb, CBlockIndex* pindexNew);
    CBlockIndex* AddToBlockIndex(CTxDB &txdb, unsigned int nFile, unsigned int nBlockPos);
    bool CheckBlock(std::string sCaller, int height1) const;
    bool AcceptBlock(CBlockIndex** out_pinex,bool generated_by_me, CBlockIndex* pindexPrev);
    bool GetCoinAge(uint64_t& nCoinAge) const; // ppcoin: calculate total coin age spent in block
    bool CheckBlockSignature() const;

private:
    bool SetBestChainInner(CTxDB& txdb, CBlockIndex *pindexNew, bool fReorganizing);
};





class CSuperblock;
/** The block chain is a tree shaped structure starting with the
 * genesis block at the root, with each block potentially having multiple
 * candidates to be the next block.  pprev and pnext link a path through the
 * main/longest chain.  A blockindex may have multiple pprev pointing back
 * to it, but pnext will only point forward to the longest branch, or will
 * be null if the block is not part of the longest chain.
 */
class CBlockIndex
{
private:
    CBlockIndex* pprev;
    CBlockIndex* pnext;
public:
    const uint256* phashBlock;
    uint256 hashPrev;
    uint256 hashNext;
    unsigned int nFile;
    unsigned int nBlockPos;
    uint256 nChainTrust; // ppcoin: trust score of block chain
    int nHeight;

    int64_t nMint;
    int64_t nMoneySupply;
	// Gridcoin (7-11-2015) Add new Accrual Fields to block index
    uint128 cpid;
	double nResearchSubsidy;
	double nInterestSubsidy;
	double nMagnitude;
	std::string sGRCAddress;
	// Indicators (9-13-2015) (+Brod)
    bool IsSuperBlock:1;
    bool IsContract:1;
    bool HasCPID:1;

    // proof-of-stake specific fields
    bool StakeEntropy:1;
    bool IsProofOfStake:1;
    bool GeneratedStakeModifier:1;
    bool DetectedInvalid:1;
    uint64_t nStakeModifier; // hash modifier for proof-of-stake
    COutPoint prevoutStake;
    unsigned int nStakeTime;

    uint256 hashProof;

    // block header
    int nVersion;
    uint256 hashMerkleRoot;
    unsigned int nTime;
    unsigned int nBits;
    unsigned int nNonce;

    void Clear()
    {
        phashBlock = NULL;
        pprev = NULL;
        pnext = NULL;
        hashPrev = 0;
        hashNext = 0;
        nFile = 0;
        nBlockPos = 0;
        nHeight = 0;
        nChainTrust = 0;
        nMint = 0;
        nMoneySupply = 0;
        nStakeModifier = 0;
        StakeEntropy = 0;
        IsProofOfStake = false;
        GeneratedStakeModifier = false;
        DetectedInvalid = false;
        hashProof = 0;
        prevoutStake.SetNull();
        nStakeTime = 0;

        nVersion       = 0;
        hashMerkleRoot = 0;
        nTime          = 0;
        nBits          = 0;
        nNonce         = 0;
		//7-11-2015 - Gridcoin - New Accrual Fields
		nResearchSubsidy = 0;
		nInterestSubsidy = 0;
		nMagnitude = 0;
		IsSuperBlock = false;
		IsContract = false;
        HasCPID = false;
		sGRCAddress = "";
    }

    CBlockIndex()
    {
        Clear();
    }

    CBlockIndex(unsigned int nFileIn, unsigned int nBlockPosIn, CBlock& block)
    {
        Clear();
        nFile = nFileIn;
        nBlockPos = nBlockPosIn;
        if (block.IsProofOfStake())
        {
            IsProofOfStake = true;
            prevoutStake = block.vtx[1].vin[0].prevout;
            nStakeTime = block.vtx[1].nTime;
        }

        nVersion       = block.nVersion;
        hashMerkleRoot = block.hashMerkleRoot;
        nTime          = block.nTime;
        nBits          = block.nBits;
        nNonce         = block.nNonce;
    }

    static CBlockIndex *GetByHash(uint256 hash, void* func = NULL);

    CBlockIndex *GetPrev()
    {
        if(pprev)
            return pprev;
        pprev = CBlockIndex::GetByHash(hashPrev);
        return pprev;
    }
    CBlockIndex *GetNext()
    {
        if(pnext)
            return pnext;
        pnext = CBlockIndex::GetByHash(hashNext);
        return pnext;
    }
    bool HasPrev() const
    {
        return (NULL!=pprev || 0!=hashPrev);
    }
    bool HasNext() const
    {
        return (NULL!=pnext || 0!=hashNext);
    }
    void SetPrev(CBlockIndex *i_prev)
    {
        pprev = i_prev;
        hashPrev = *(i_prev->phashBlock);
    }
    void MakePrevNextConnection(bool connect)
    {
        GetPrev();
        assert(pprev);
        pprev->pnext = connect? this : NULL;
        pprev->hashNext = connect? *phashBlock : 0;
    }

    CBlock GetBlockHeader() const
    {
        CBlock block;
        block.nVersion       = nVersion;
        if (pprev)
            block.hashPrevBlock = pprev->GetBlockHash();
        block.hashMerkleRoot = hashMerkleRoot;
        block.nTime          = nTime;
        block.nBits          = nBits;
        block.nNonce         = nNonce;
        return block;
    }

    uint256 GetBlockHash() const
    {
        return *phashBlock;
    }

    int64_t GetBlockTime() const
    {
        return (int64_t)nTime;
    }

    uint256 GetBlockTrust() const;
    bool IsInMainChain() const;

    int64_t GetPastTimeLimit()
    {
        if (IsProtocolV2(nHeight))
	         return GetMedianTimePast();
        else
            return GetMedianTimePast();
    }

    enum { nMedianTimeSpan=11 };

    int64_t GetMedianTimePast()
    {
        int64_t pmedian[nMedianTimeSpan];
        int64_t* pbegin = &pmedian[nMedianTimeSpan];
        int64_t* pend = &pmedian[nMedianTimeSpan];

        CBlockIndex* pindex = this;
        for (int i = 0; i < nMedianTimeSpan && pindex; i++, pindex = pindex->GetPrev())
            *(--pbegin) = pindex->GetBlockTime();

        std::sort(pbegin, pend);
        return pbegin[(pend - pbegin)/2];
    }

    /**
     * Returns true if there are nRequired or more blocks of minVersion or above
     * in the last nToCheck blocks, starting at pstart and going backwards.
     */
    static bool IsSuperMajority(int minVersion, CBlockIndex* pstart,
                                unsigned int nRequired, unsigned int nToCheck);


    bool IsProofOfWork() const
    {
        return !IsProofOfStake;
    }

    bool IsUserCPID() const
    {        
        return HasCPID;
    }

    unsigned int GetStakeEntropyBit() const
    {
        return StakeEntropy? 1 : 0;
    }

    void SetStakeModifier(uint64_t nModifier, bool fGeneratedStakeModifier)
    {
        nStakeModifier = nModifier;
        GeneratedStakeModifier= fGeneratedStakeModifier;
    }

    void SetCPID(const std::string& cpid_hex)
    {
        if(cpid_hex.empty())
        {
            HasCPID= false;
            cpid = 0;
        }
        else if(cpid_hex == "INVESTOR")
        {
            HasCPID= false;
            cpid = 1;
        }
        else if(cpid_hex == "POOL")
        {
            HasCPID= false;
            cpid = 2;
        }
        else
        {
            HasCPID= true;
            cpid.SetHex(cpid_hex);
        }
    }

    std::string GetCPID() const
    {
        if(HasCPID)
            return cpid.GetHex();
        if(cpid==1)
            return "INVESTOR";
        if(cpid==2)
            return "POOL";
        return "";
    }

    std::string ToString() const
    {
        return strprintf("CBlockIndex(nprev=%p, pnext=%p, nFile=%u, nBlockPos=%-6d nHeight=%d, nMint=%s, nMoneySupply=%s, nFlags=(%s)(%d)(%s), nStakeModifier=%016" PRIx64 ", hashProof=%s, prevoutStake=(%s), nStakeTime=%d merkle=%s, hashBlock=%s, hashPrev=%s, hashNext=%s)",
            pprev, pnext, nFile, nBlockPos, nHeight,
            FormatMoney(nMint).c_str(), FormatMoney(nMoneySupply).c_str(),
            GeneratedStakeModifier ? "MOD" : "-", GetStakeEntropyBit(), IsProofOfStake? "PoS" : "PoW",
            nStakeModifier,
            hashProof.ToString().c_str(),
            prevoutStake.ToString().c_str(), nStakeTime,
            hashMerkleRoot.ToString().c_str(),
            GetBlockHash().ToString().c_str(),
            hashPrev.ToString().c_str(),
            hashNext.ToString().c_str());
    }    

    void print() const
    {
        printf("%s\n", ToString().c_str());
    }

    enum  
    {
        BLOCK_PROOF_OF_STAKE = (1 << 0), // is proof-of-stake block
        BLOCK_STAKE_ENTROPY  = (1 << 1), // entropy bit for stake modifier
        BLOCK_STAKE_MODIFIER = (1 << 2), // regenerated stake modifier
        BLOCK_HAS_CPID       = (1 << 3), // CPID not empty nor "INVESTOR"
        BLOCK_SUPER          = (1 << 5), // has superblock data
        BLOCK_CONTRACT       = (1 << 6), // has contract data
        BLOCK_INVALID        = (1 << 7), // found invalid by ConnectBlock()
    };

    template<typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const;
    template<typename Stream>
    void Unserialize(Stream& s, int nType, int nVersion);

};

class StructCPID2
{
    public:
    // main
    /* blocks staked by this cpid on main chain
       bound by p6m and top
       sorted by height asc */
    std::deque <CBlockIndex*> vpBlocks;
    // utility
    uint128 *cpid = NULL;
    std::string GRCAddress; // last address used
    // current superblock data
    double SbMagnitude; // magnitude as in last superblock
    // beacon data (cache)
    std::vector<unsigned char> BeaconPublicKey; // pk for signing
    // lifetime averages for current reward calc
    // Lft=lifetime, D14=14day
    double LftFirstBlockTime; // time of first staked block
    double LftSumReward; // dpor payment of all time
    double LftSumMagnitude; // sum magnitude of all blocks
    double LftCntMagnitude; // count magnitude of all blocks
    double LftCountReward; // count of all blocks
    // averages for stats
    double D14SumReward;
    double LftSumInterest;
    double D14SumInterest;
    double D14SumMagnitude;
    double D14CountReward;

    void SaveDb(CTxDB& batch);
    StructCPID2()
    {
        //note, object instances are initialized auto
        // primitive types would be undefined
        cpid=NULL;
        SbMagnitude= LftFirstBlockTime= LftSumReward= LftSumMagnitude=0;
        LftCountReward= D14SumReward= LftSumInterest= D14SumInterest=0;
        D14SumMagnitude= LftCntMagnitude= D14CountReward=0;
    }
    bool LoadAccountKey(const MiningCPID& bb, CBlockIndex* pindex);
};

class CTxMessage
{
public:
    bool fLoaded;
    unsigned int nTime;
    // transactions that have message with same type and identifier
    // bound by p6m and top
    // sorted by height asc
    std::deque <uint256> vTxHash;
    void SaveDb(CTxDB& batch);
    //following only valid if fLoaded
    bool fDelete;
    unsigned int nBlock;
    std::string sType;
    std::string sName;
    std::string sValue;
};

class CBestChain
{
public:
    CBlockIndex* top;  // ptr to top of best chain
    CBlockIndex* p6m;  // 6 months ago
    CBlockIndex* p14d; // 14 days ago
    CBlockIndex* p10b; // 10 blocks ago
    CBlockIndex* ppSuper; // superblock
    //Data related to CPID
    // index: binary cpid; data: researcher info
    std::map<uint128, StructCPID2> cpid;
    // 14 day summary
    struct {
        double Research;
        double Interest;
        unsigned CpidCount; //WTF?
        //...
        unsigned blocks;
    } sum;
    // network-wide superblock data
    struct {
        double Magnitude; //TotalNetworkMagnitude
        unsigned CpidCount;
        unsigned ProjectCount;
        uint256 hashBlock; // blockhash of superblock loaded
        uint256 hash; // hash of superblock data loaded
        std::deque <CBlockIndex*> vpBlocks; //list of SBs
    } super;
    // Network messages (cpid, project, poll, vote)
    std::map<std::string, CTxMessage> msg;
    std::map<uint256, CTxMessage*> MsgByTx;
    // Neural things
    //...

    CBestChain()
    {
        top= p6m= p14d= p10b= ppSuper= NULL;
        sum.Research= sum.Interest= 0;
        super.Magnitude= super.CpidCount= super.ProjectCount= 0;
        super.hashBlock= super.hash= 0;
        cpid.clear();
        msg.clear();
    }
    int GetHeight() const
    {
        return top ? top->nHeight : 0;
    }

    // Access methods
    StructCPID2* GetCPID(const uint128& bincpid);
    StructCPID2* GetCPID(const std::string& strcpid);
    void SetCPID(const uint128& bincpid, StructCPID2* newvalue);

    CTxMessage* GetMessage(std::string sType, std::string sName, unsigned int nCurTime);
    
    // Load CBestChain persisted data
    void LoadDb(CTxDB& batch);
    void LoadDbBeacons(CTxDB& batch);
    void LoadDbCpids(CTxDB& batch);
    void SaveDb(CTxDB& batch);
};

extern CBestChain Best;




/** Describes a place in the block chain to another node such that if the
 * other node doesn't have the same branch, it can find a recent common trunk.
 * The further back it is, the further before the fork it may be.
 */
class CBlockLocator
{
protected:
    std::vector<uint256> vHave;
public:

    CBlockLocator()
    {
    }

    explicit CBlockLocator(CBlockIndex* pindex)
    {
        Set(pindex);
    }

    explicit CBlockLocator(uint256 hashBlock)
    {
        std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(hashBlock);
        if (mi != mapBlockIndex.end())
            Set((*mi).second);
    }

    CBlockLocator(const std::vector<uint256>& vHaveIn)
    {
        vHave = vHaveIn;
    }

    IMPLEMENT_SERIALIZE
    (
        if (!(nType & SER_GETHASH))
            READWRITE(nVersion);
        READWRITE(vHave);
    )

    void SetNull()
    {
        vHave.clear();
    }

    bool IsNull()
    {
        return vHave.empty();
    }

    void Set(CBlockIndex* pindex)
    {
        vHave.clear();
        int nStep = 1;
        while (pindex)
        {
            vHave.push_back(pindex->GetBlockHash());

            // Exponentially larger steps back
            for (int i = 0; pindex && i < nStep; i++)
                pindex = pindex->GetPrev();
            if (vHave.size() > 10)
                nStep *= 2;
        }
        vHave.push_back((!fTestNet ? hashGenesisBlock : hashGenesisBlockTestNet));
    }

    int GetDistanceBack()
    {
        // Retrace how far back it was in the sender's branch
        int nDistance = 0;
        int nStep = 1;
        BOOST_FOREACH(const uint256& hash, vHave)
        {
            std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(hash);
            if (mi != mapBlockIndex.end())
            {
                CBlockIndex* pindex = (*mi).second;
                if (pindex->IsInMainChain())
                    return nDistance;
            }
            nDistance += nStep;
            if (nDistance > 10)
                nStep *= 2;
        }
        return nDistance;
    }

    CBlockIndex* GetBlockIndex()
    {
        // Find the first block the caller has in the main chain
        BOOST_FOREACH(const uint256& hash, vHave)
        {
            std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(hash);
            if (mi != mapBlockIndex.end())
            {
                CBlockIndex* pindex = (*mi).second;
                if (pindex->IsInMainChain())
                    return pindex;
            }
        }
        return pindexGenesisBlock;
    }

    uint256 GetBlockHash()
    {
        // Find the first block the caller has in the main chain
        BOOST_FOREACH(const uint256& hash, vHave)
        {
            std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(hash);
            if (mi != mapBlockIndex.end())
            {
                CBlockIndex* pindex = (*mi).second;
                if (pindex->IsInMainChain())
                    return hash;
            }
        }
        return (!fTestNet ? hashGenesisBlock : hashGenesisBlockTestNet);
    }

    int GetHeight()
    {
        CBlockIndex* pindex = GetBlockIndex();
        if (!pindex)
            return 0;
        return pindex->nHeight;
    }
};






class CTxMemPool
{
public:
    mutable CCriticalSection cs;
    std::map<uint256, CTransaction> mapTx;
    std::map<COutPoint, CInPoint> mapNextTx;

    bool addUnchecked(const uint256& hash, CTransaction &tx);
    bool remove(const CTransaction &tx, bool fRecursive = false);
    bool removeConflicts(const CTransaction &tx);
    void clear();
    void queryHashes(std::vector<uint256>& vtxid);

    unsigned long size() const
    {
        LOCK(cs);
        return mapTx.size();
    }

    bool exists(uint256 hash) const
    {
        LOCK(cs);
        return (mapTx.count(hash) != 0);
    }

    bool lookup(uint256 hash, CTransaction& result) const
    {
        LOCK(cs);
        std::map<uint256, CTransaction>::const_iterator i = mapTx.find(hash);
        if (i == mapTx.end()) return false;
        result = i->second;
        return true;
    }
};

extern CTxMemPool mempool;

CBlockIndex* GetHistoricalMagnitude(StructCPID2* stc);

#endif
