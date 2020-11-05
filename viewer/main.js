//
// HAL Programming Contest 2020 Viewer
// (C)HAL Laboratory, Inc.
//
'use strict';
const vm = new Vue({
  el: 'body',
  data: {
    json: false, stage: 0, turn: 0,
    playType: 0, isPlay: false, timer: null, isDrag: false,
    minColorTurn: 100, maxColorTurn: 300
  },
  computed: {
    stages: function () { return this.json[1]; },
    stageWidth: function () { return this.stages[this.stage][1][0]; },
    stageHeight: function () { return this.stages[this.stage][1][1]; },
    turns: function () { return this.stages[this.stage][2]; },
    grounds: function () {
      return this.stages[this.stage][1][2].reduce((result, cols) => result.concat(cols), []).map(kind => ({ kind }));  
    },
    points: function () { return this.turns.map(turn => turn.slice(0, 2).join()).join(' '); },
    scrolls: function () {
      return this.stages[this.stage][1][3].filter((scroll, i) => this.turns[this.turn][2] & (1 << i));
    }
  },
  methods: {
    import: function (e) {
      const reader = new FileReader();
      reader.onload = (function () {
        return function (e) {
          try {
            vm.json = JSON.parse(e.target.result);
            vm.stage = 0;
          } catch (ee) {
            alert('JSONファイルが壊れています');
          }
        }
      })(e.target.files[0]);
      reader.readAsText(e.target.files[0]);
    },
    // ターンスライダーをドラッグしている際に呼ばれる関数
    drag: function (e) {
      const sliderRect = e.target.parentNode.getBoundingClientRect();
      this.turn = (e.clientX - sliderRect.left)*this.turns.length/sliderRect.width|0;
      this.isDrag = true;
    },
    // トータルターン
    totalTurn: function () {
      return this.stages.reduce(function (total, stage) { return total + stage[0]; }, 0).toLocaleString();
    },
    // ターン数に応じて、赤〜緑の色値を計算する関数
    stageColor: function (no) {
      if (this.stages[no][0] !== 1000) {
        var d = 1 - (this.stages[no][0] - this.minColorTurn)/(this.maxColorTurn - this.minColorTurn);
        d = Math.min(Math.max(d, 0), 1);
        return 'hsl(' + (d*120|0) + ', 100%, 50%)';
      } else return '#00f';
    },
    fieldStyle: function () {
      return {
        gridTemplate: 'repeat(' + this.stageHeight + ',20px) / repeat(' + this.stageWidth + ',20px)',
        MsGridRows: '(20px)[' + this.stageHeight + ']', MsGridColumns: '(20px)[' + this.stageWidth + ']',
        width: this.stageWidth*20 + 'px', height: this.stageHeight*20 + 'px'
      };
    },
    rabbitStyle: function () {
      return { left: this.turns[this.turn][0]*20 - 8 + 'px', top: this.turns[this.turn][1]*20 - 8 + 'px' };
    },
    scrollStyle: function (scroll) {
      return { left: scroll[0]*20 + 2 + 'px', top: scroll[1]*20 + 2 + 'px' };
    }
  },
  watch: {
    // ステージが変更されたら、ターンを0にリセット
    stage: function (stage) {
      this.stage = Math.min(Math.max(stage, 0), this.stages.length - 1);
      this.turn = 0;
    },
    // ターンが変更されたら、スライダーも更新
    turn: function (turn) {
      const slider = document.getElementById('slider'),
            control = slider.firstChild.nextSibling;
      this.turn = Math.min(Math.max(turn, 0), this.turns.length - 1);
      control.style.left = (
        (slider.getBoundingClientRect().width - control.getBoundingClientRect().width)
        *this.turn/this.turns.length|0
      ) + 'px';
    },
    // 再生ボタンが押されたら、再生状態を変更する
    isPlay: function (isPlay) {
      if (isPlay) {
        this.timer = setTimeout(function loop() {
          if (vm.turn < vm.turns.length - 1) {
            vm.turn++;
            vm.timer = setTimeout(loop, 100);
          } else if (vm.playType == 1) {
            vm.turn = 0;
            vm.timer = setTimeout(loop, 100);
          } else if(vm.playType == 2) {
            if (vm.stage < vm.stages.length - 1) {
              vm.stage++
            } else vm.stage = 0;
            vm.timer = setTimeout(loop, 100);
          } else vm.isPlay = false;
        }, 15);
      } else clearTimeout(this.timer);
    }
  }
})
