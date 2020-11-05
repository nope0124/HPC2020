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
#include "Vector2.hpp"

//------------------------------------------------------------------------------
namespace hpc {

/// プレイヤー
class Rabbit
{
public:
    /// @name コンストラクタ
    //@{
    Rabbit();
    Rabbit(const Vector2& aPos);
    //@}

    /// @name 各種問い合わせ
    //@{
    /// プレイヤーの座標
    const Vector2& pos() const;
    /// 座標を更新
    void setPos(const Vector2& aPos);
    /// ジャンプ力
    float power() const;
    /// ジャンプ力を更新
    void setPower(float aPower);
    //@}

private:
    /// @name プライベートメンバ変数
    //@{
    /// 座標
    Vector2 mPosition;
    /// ジャンプ力
    float mPower;
    //@}
};

} // namespace
// EOF
