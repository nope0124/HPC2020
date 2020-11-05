//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "Recorder.hpp"

//------------------------------------------------------------------------------
#include "Print.hpp"

//------------------------------------------------------------------------------
namespace hpc {

//------------------------------------------------------------------------------
Recorder::Recorder()
: mGameRecord()
, mCurrentStageNumber()
, mCurrentTurn()
{
}

//------------------------------------------------------------------------------
void Recorder::dumpJson() const
{
    HPC_PRINTF("["); {
        HPC_PRINTF("%d,", mGameRecord.totalTurn);
        HPC_PRINTF("["); {
            for (int i = 0; i < Parameter::GameStageCount; ++i) {
                if (i != 0) {
                    HPC_PRINTF(",");
                }
                dumpJsonStage(mGameRecord.stageRecords[i]);
            }
        } HPC_PRINTF("]");
    } HPC_PRINTF("]");

    HPC_PRINTF("\n");
}

//------------------------------------------------------------------------------
void Recorder::dumpResult(bool aIsSilent) const
{
    if (!aIsSilent) {
        HPC_PRINTF("stage | turn\n");
        for (int i = 0; i < Parameter::GameStageCount; ++i) {
            HPC_PRINTF("% 5d | % 4d\n", i, mGameRecord.stageRecords[i].scoreTurn);
        }
    }
    HPC_PRINTF("TotalTurn: %d\n", mGameRecord.totalTurn);
}

//------------------------------------------------------------------------------
void Recorder::afterInitializeStage(const Stage& aStage)
{ 
    StageRecord& stageRecord = mGameRecord.stageRecords[mCurrentStageNumber];

    mCurrentTurn = 0;

    for (int h = 0; h < Parameter::StageHeight; ++h)  {
        for (int w = 0; w < Parameter::StageWidth; ++w) {
            auto t = aStage.terrain(Vector2(static_cast<float>(w), static_cast<float>(h)));
            stageRecord.map[h][w] = static_cast<int>(t);
        }
    }

    for (int i = 0; i < aStage.scrolls().count(); ++i) {
        stageRecord.scrolls[i] = aStage.scrolls()[i].pos();
    }
    stageRecord.scrollCount = aStage.scrolls().count();

    writeTurnRecord(mCurrentStageNumber, 0, aStage);
}

//------------------------------------------------------------------------------
void Recorder::afterAdvanceTurn(const Stage& aStage)
{
    ++mCurrentTurn;
    writeTurnRecord(mCurrentStageNumber, mCurrentTurn, aStage);
}

//------------------------------------------------------------------------------
void Recorder::afterFinishStage(const Stage& aStage)
{
    StageRecord& stageRecord = mGameRecord.stageRecords[mCurrentStageNumber];
    stageRecord.turn = mCurrentTurn;
    int scoreTurn = aStage.isOutOfBounds(aStage.rabbit().pos()) ? Parameter::GameTurnLimit : mCurrentTurn;
    stageRecord.scoreTurn = scoreTurn;
    mGameRecord.totalTurn += scoreTurn;
    ++mCurrentStageNumber;
}

//------------------------------------------------------------------------------
void Recorder::afterFinishAllStages()
{
}

//------------------------------------------------------------------------------
int Recorder::totalTurn() const
{
    return mGameRecord.totalTurn;
}

//------------------------------------------------------------------------------
void Recorder::dumpJsonStage(const StageRecord& aRecord) const
{
    HPC_PRINTF("["); {
        HPC_PRINTF("%d,", aRecord.scoreTurn);
        HPC_PRINTF("["); {
            HPC_PRINTF("%d,", Parameter::StageWidth);
            HPC_PRINTF("%d,", Parameter::StageHeight);

            HPC_PRINTF("["); {
                for (int h = 0; h < Parameter::StageHeight; h++) {
                    if (h > 0) {
                        HPC_PRINTF(",");
                    }
                    HPC_PRINTF("["); {
                        for (int w = 0; w < Parameter::StageWidth; w++) {
                            if (w > 0) {
                                HPC_PRINTF(",");
                            }
                            HPC_PRINTF("%d",static_cast<int>(aRecord.map[h][w]));
                        }
                    } HPC_PRINTF("]");
                }
            } HPC_PRINTF("],");

            HPC_PRINTF("["); {
                for (int i = 0; i < aRecord.scrollCount; ++i) {
                    if (i != 0) {
                        HPC_PRINTF(",");
                    }
                    HPC_PRINTF("["); {
                        HPC_PRINTF("%d,", static_cast<int>(aRecord.scrolls[i].x));
                        HPC_PRINTF("%d", static_cast<int>(aRecord.scrolls[i].y));
                    } HPC_PRINTF("]");
                }
            } HPC_PRINTF("]");
        } HPC_PRINTF("],");

        // ターンのログ
        HPC_PRINTF("["); {
            for (int i = 0; i <= aRecord.turn; ++i) {
                if (i != 0) {
                    HPC_PRINTF(",");
                }
                dumpJsonTurn(aRecord.turnRecords[i]);
            }
        } HPC_PRINTF("]");
    } HPC_PRINTF("]");
}

//------------------------------------------------------------------------------
void Recorder::dumpJsonTurn(const TurnRecord& aRecord) const
{
    HPC_PRINTF("["); {
        HPC_PRINTF("%f,", aRecord.pos.x);
        HPC_PRINTF("%f,", aRecord.pos.y);
        HPC_PRINTF("%d", aRecord.leftover);
    } HPC_PRINTF("]");
}

//------------------------------------------------------------------------------
void Recorder::writeTurnRecord(int aStageNumber, int aTurn, const Stage& aStage)
{
    TurnRecord& turnRecord = mGameRecord.stageRecords[aStageNumber].turnRecords[aTurn];

    turnRecord.pos = aStage.rabbit().pos();
    int leftovers = 0;
    for (int i = 0; i < aStage.scrolls().count(); i++) {
        if(!aStage.scrolls()[i].isGotten()) {
            leftovers |= (1 << i);
        }
    }
    turnRecord.leftover = leftovers;
}

} // namespace
// EOF
