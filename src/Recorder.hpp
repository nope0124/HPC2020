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
#include "Stage.hpp"

//------------------------------------------------------------------------------
namespace hpc {

/// ゲーム進行を記録するクラス
class Recorder
{
public:
    /// @name コンストラクタ
    //@{
    Recorder();
    //@}

    /// @name ダンプ
    //@{
    /// 最終結果を出力する
    void dumpResult(bool aIsSilent) const;
    /// 記録内容をJSONとして出力する
    void dumpJson() const;
    //@}

    /// @name 記録
    //@{
    /// ステージの初期化後に実行される関数
    void afterInitializeStage(const Stage& aStage);
    /// ターンを進めた後に実行される関数
    void afterAdvanceTurn(const Stage& aStage);
    /// ステージが終了した後に実行される関数
    void afterFinishStage(const Stage& aStage);
    /// 全ステージが終了した後に実行される関数
    void afterFinishAllStages();
    //@}

    /// @name 問い合わせ
    //@{
    /// ゲーム全体での総ターン数
    int totalTurn() const;
    //@}

private:
    /// @name 内部構造体
    //@{
    /// ターン情報
    struct TurnRecord
    {
        /// プレイヤー座標
        Vector2 pos;
        /// 巻物取得状況
        int leftover;
    };

    /// ステージ情報
    struct StageRecord
    {
        /// ターン数
        int turn;
        /// スコアとなるターン数
        int scoreTurn;
        /// 地形情報
        int map[Parameter::StageHeight][Parameter::StageWidth];
        /// 巻物
        Vector2 scrolls[Parameter::MaxScrollCount];
        /// 巻物の数
        int scrollCount;
        /// ターンごとの情報
        TurnRecord turnRecords[Parameter::GameTurnLimit + 1];
    };
    /// ゲーム情報。
    struct GameRecord
    {
        /// ゲーム全体での総ターン数
        int totalTurn;
        /// ステージ情報
        StageRecord stageRecords[Parameter::GameStageCount];
    };
    //@}

    /// @name 内部実装
    //@{
    /// ステージ情報を JSON として出力する
    void dumpJsonStage(const StageRecord& aRecord) const;
    /// ターン情報を JSON として出力する
    void dumpJsonTurn(const TurnRecord& aRecord) const;
    /// 現在のターン情報を記録する
    void writeTurnRecord(int aStageNumber, int aTurn, const Stage& aStage);
    //@}

    /// @name プライベートメンバ変数
    //@{
    /// ゲーム情報。
    GameRecord mGameRecord;
    /// 現在記録中のステージ番号
    int mCurrentStageNumber;
    /// 現在記録中のターン番号
    int mCurrentTurn;
    //@}

};

} // namespace
// EOF
