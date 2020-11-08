//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "RandomSeed.hpp"

//------------------------------------------------------------------------------
namespace hpc {

//------------------------------------------------------------------------------
RandomSeed RandomSeed::DefaultSeed()
{
    // ここの乱数シード値に関する注意事項:
    //   ・ここの乱数シード値は
    //     サーバーの評価環境と配布パッケージとで同一です。
    //   ・ここの乱数シード値を変更しても
    //     ゲームが使っている乱数は変化しません。
    //     - ゲームが使っている乱数を変更したい場合は
    //       次のいずれかの対応が必要です:
    //       - Game::run にある randomSeed を変更する。
    //       - 起動引数で乱数シードを渡すようにする。
    //         - 起動引数への乱数シードの渡し方については
    //           README.txt を参照してください。
    return RandomSeed(
//        0x81eacc80,
        0x00000000,
        0xcb9412ed,
        0x1acb27dc,
        0x21a234f3
        );
}

//------------------------------------------------------------------------------
RandomSeed::RandomSeed(uint aX, uint aY, uint aZ, uint aW)
: x(aX)
, y(aY)
, z(aZ)
, w(aW)
{
}

} // namespace
// EOF
