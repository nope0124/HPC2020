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

/// 巻物
class Scroll
{
public:
    /// @name コンストラクタ
    //@{
    Scroll();
    Scroll(const Vector2& aPos);
    //@}

    /// @name 各種問い合わせ
    //@{
    /// 巻物の座標
    const Vector2& pos() const;
    /// すでに手に入れられたか
    bool isGotten() const;
    /// 手に入れられたかどうかを設定する
    void setIsGotten(bool aIsGotten);
    //@}

private:
    /// @name プライベートメンバ変数
    //@{
    /// 座標
    Vector2 mPosition;
    /// 手に入れられたかどうか。
    bool mIsGotten;
    //@}
};

} // namespace
// EOF
