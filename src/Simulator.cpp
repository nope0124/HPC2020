//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "Simulator.hpp"

//------------------------------------------------------------------------------
#include "Answer.hpp"
#include "Print.hpp"

//------------------------------------------------------------------------------
namespace hpc {

//------------------------------------------------------------------------------
Simulator::Simulator()
: mGame()
{
}

//------------------------------------------------------------------------------
void Simulator::changeSeed(RandomSeed aSeed)
{
    mGame.changeSeed(aSeed);
}

//------------------------------------------------------------------------------
void Simulator::run()
{
    Answer answer;
    mGame.run(&answer);
}

//------------------------------------------------------------------------------
void Simulator::printResult(bool aIsSilent) const
{
    mGame.recorder().dumpResult(aIsSilent);
    HPC_PRINTF("Time: %.3f sec\n", mGame.timer().elapsedSec());
}

//------------------------------------------------------------------------------
void Simulator::printJson() const
{
    mGame.recorder().dumpJson();
}

//------------------------------------------------------------------------------
int Simulator::totalTurn() const
{
    return mGame.recorder().totalTurn();
}

//------------------------------------------------------------------------------
double Simulator::elapsedSec() const
{
    return mGame.timer().elapsedSec();
}

} // namespace
// EOF
