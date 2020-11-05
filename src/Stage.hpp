//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
#include "Array.hpp"
#include "Scroll.hpp"
#include "Parameter.hpp"
#include "Rabbit.hpp"
#include "Random.hpp"
#include "Terrain.hpp"
#include "Vector2.hpp"

//------------------------------------------------------------------------------
namespace hpc {
/// 巻物の配列
using Scrolls = Array<Scroll, Parameter::MaxScrollCount>;

/// ステージ
class Stage
{
public:
    /// @name コンストラクタ
    //@{
    Stage();
    //@}

    /// @name 問い合わせ
    //@{
    /// 現在のターン数
    int turn() const;
    /// プレイヤー
    const Rabbit& rabbit() const;
    /// 巻物の配列
    const Scrolls& scrolls() const;
    /// 地形
    Terrain terrain(const Vector2& aPos) const;
    /// 
    bool isOutOfBounds(const Vector2 aPos) const;
    /// ジャンプ後の座標を得る
    Vector2 getNextPos(const Vector2& aPos, float aPower, const Vector2& aTargetPos) const;
    //@}

    /// @name 内部関数用
    //@{
    /// 初期化する
    void initialize(int aScrollCount, const RandomSeed& aStageRandomSeed);
    /// 更新する
    void update(const Vector2& aTargetPos);
    /// ターンを進める
    void advanceTurn();
    /// 終了したかどうか
    bool isEnd() const;
    //@}

private:
    /// @name プライベートメンバ変数
    //@{
    /// 現在のターン数
    int mTurn;
    /// プレイヤー
    Rabbit mRabbit;
    /// 巻物の配列
    Scrolls mScrolls;
    /// 地形
    Terrain mMap[Parameter::StageHeight][Parameter::StageWidth];
    //@}
};

} // namespace
// EOF
