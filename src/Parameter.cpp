//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "Parameter.hpp"

//------------------------------------------------------------------------------
#include <cmath>
#include <limits>
#include "Assert.hpp"

//------------------------------------------------------------------------------
namespace hpc {
const float Parameter::JumpPowerUpRate = 1.1f;
const float Parameter::JumpTerrianCoefficient[] = {
    1.0f,
    0.6f,
    0.3f,
    0.1f
};
} // namespace
// EOF
