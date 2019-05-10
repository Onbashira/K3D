struct EmitterHeader
{
    uint EmtBinHead;
    uint PtBinHead;
    uint PtIdxHead;
    uint PtSize;
};

struct EmitterRange
{
    uint AliveBegin;
    uint AliveEnd;
    uint DeadBegin;
    uint DeadEnd;
};

struct PaticleData
{
    uint Key;
    float Depth;
};