//------------------------------------------------------------------------------
/// @file
/// @author   ハル研究所プログラミングコンテスト実行委員会
///
/// @copyright  (C)HAL Laboratory, Inc.
/// @attention  このファイルの利用は、同梱のREADMEにある
///             利用条件に従ってください。
//------------------------------------------------------------------------------

#pragma once
#include "Stage.hpp"

//------------------------------------------------------------------------------
#include "Assert.hpp"
#include "Math.hpp"
#include "Print.hpp"

//------------------------------------------------------------------------------
namespace hpc {

//------------------------------------------------------------------------------
Stage::Stage()
: mTurn()
, mRabbit()
, mScrolls()
, mMap()
{
}

//------------------------------------------------------------------------------
int Stage::turn() const
{
    return mTurn;
}

//------------------------------------------------------------------------------
const Rabbit& Stage::rabbit() const
{
    return mRabbit;
}

//------------------------------------------------------------------------------
const Scrolls& Stage::scrolls() const
{
    return mScrolls;
}

//------------------------------------------------------------------------------
Terrain Stage::terrain(const Vector2& aPos) const
{
    return mMap[static_cast<int>(aPos.y)][static_cast<int>(aPos.x)];
}

//------------------------------------------------------------------------------
bool Stage::isOutOfBounds(const Vector2 aPos) const
{
    if(aPos.x < 0.0f || Parameter::StageWidth <= aPos.x || aPos.y < 0.0f || Parameter::StageHeight <= aPos.y) {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------------------
Vector2 Stage::getNextPos(const Vector2& aPos, float aPower, const Vector2& aTargetPos) const
{
    auto t = terrain(aPos);
    auto maxlen = Parameter::JumpTerrianCoefficient[(int)t] * aPower;

    Vector2 dir(aTargetPos.x - aPos.x, aTargetPos.y - aPos.y);
    auto len = Math::Sqrt(dir.x * dir.x + dir.y * dir.y);
    if(len == 0.0f) {
        return aTargetPos;
    }
    if (len > maxlen) {
        dir = Vector2(dir.x * (maxlen / len), dir.y * (maxlen / len));
    }
    Vector2 nextPos(aPos.x + dir.x, aPos.y + dir.y);
    return nextPos;
}

//------------------------------------------------------------------------------
void Stage::initialize(int aScrollCount, const RandomSeed& aStageRandomSeed)
{
    Random random(aStageRandomSeed);
    mTurn = 0;

    auto startPosX = random.randMinTerm(1, Parameter::StageWidth - 1) + 0.5f;
    auto startPosY = random.randMinTerm(1, Parameter::StageHeight - 1) + 0.5f;
    Vector2 startPos(startPosX, startPosY);
    mRabbit.setPos(startPos);

    for (int h = 0; h < Parameter::StageHeight; h++) {
        for (int w = 0; w < Parameter::StageWidth; w++) {
            mMap[h][w] = Terrain::Plain;
        }
    }

    for (int i = 0; i < 40; i++) {
        auto t = (Terrain)random.randMinTerm(1, 4);
        auto w = random.randMinTerm(1, 16);
        auto h = random.randMinTerm(1, 16);
        auto x = random.randTerm(Parameter::StageWidth - (w - 1));
        auto y = random.randTerm(Parameter::StageHeight - (h - 1));
        for (int ii = y; ii < y + h; ii++) {
            for (int jj = x; jj < x + w; jj++) {
                mMap[ii][jj] = t;
            }
        }
    }

    // 初期位置は必ずPlain
    mMap[static_cast<int>(startPos.y)][static_cast<int>(startPos.x)] = Terrain::Plain;

    bool scrollMap[Parameter::StageHeight][Parameter::StageWidth] = {};
    // 初期位置には巻物は置かない
    scrollMap[static_cast<int>(startPos.y)][static_cast<int>(startPos.x)] = true;
    for (int i = 0; i < aScrollCount; i++) {
        for(; ;) {
            auto x = random.randMinTerm(0, Parameter::StageWidth);
            auto y = random.randMinTerm(0, Parameter::StageHeight);
            if (!scrollMap[y][x]) {
                mScrolls.add(Scroll(Vector2(x + 0.5f, y + 0.5f)));
                scrollMap[y][x] = true;
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------
void Stage::update(const Vector2& aTargetPos)
{
    auto nextPos = getNextPos(mRabbit.pos(), mRabbit.power(), aTargetPos);
    mRabbit.setPos(nextPos);

    for (auto& scroll : mScrolls) {
        if (scroll.isGotten()) {
            continue;
        }
        
        if (static_cast<int>(scroll.pos().x) == static_cast<int>(nextPos.x) 
            && static_cast<int>(scroll.pos().y) == static_cast<int>(nextPos.y)) {
            scroll.setIsGotten(true);
            
            auto nextPower = mRabbit.power() * Parameter::JumpPowerUpRate;
            mRabbit.setPower(nextPower);
        }
    }
}

//------------------------------------------------------------------------------
void Stage::advanceTurn()
{
    ++mTurn;
}

//------------------------------------------------------------------------------
bool Stage::isEnd() const
{
    for (auto scroll : mScrolls) {
        if (!scroll.isGotten()) {
            return false;
        }
    }
    return true;
}

} // namespace
// EOF
