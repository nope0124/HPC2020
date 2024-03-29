﻿//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "Timer.hpp"

//------------------------------------------------------------------------------
namespace hpc {

//------------------------------------------------------------------------------
Timer::Timer()
: mTimeBegin(std::clock_t())
, mTimeEnd(std::clock_t())
{
}

//------------------------------------------------------------------------------
void Timer::start()
{
    mTimeBegin = ::std::clock();
}

//------------------------------------------------------------------------------
void Timer::stop()
{
    mTimeEnd = ::std::clock();
}

//------------------------------------------------------------------------------
double Timer::elapsedSec() const
{
    return static_cast<double>(mTimeEnd - mTimeBegin) / CLOCKS_PER_SEC;
}

} // namespace
// EOF
