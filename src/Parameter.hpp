//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once

#include "Terrain.hpp"

//------------------------------------------------------------------------------
namespace hpc {

class Parameter
{
public:
    /// ステージ数
    static const int GameStageCount = 200;
    /// 各ステージのターン数
    static const int GameTurnLimit = 1000;
    /// ステージの横幅
    static const int StageWidth = 50;
    /// ステージの縦幅
    static const int StageHeight = 50;
    /// 巻物の最大数
    static const int MaxScrollCount = 20;
    /// ジャンプ力のパワーアップ
    static const float JumpPowerUpRate;
    /// 地形ごとのジャンプ力にかかる係数
    static const float JumpTerrianCoefficient[];
};

} // namespace
// EOF
