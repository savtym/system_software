"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

//
// process.js
//

var Process = function () {
  function Process(title) {
    var performance = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 100;

    _classCallCheck(this, Process);

    this.title = title;
    this.performance = performance;
    this._isBusy = false;
  }

  _createClass(Process, [{
    key: "makingFIFO",
    value: function makingFIFO(task) {
      this._isBusy = true;
      this._delay(task);
      this._isBusy = false;
    }
  }, {
    key: "_delay",
    value: function _delay(task) {
      task.runningTime = task.time;
      task.startTime = performance.now();
      var finishTime = performance.now() + task.runningTime;

      while (performance.now() <= finishTime) {}

      task.finishTime = performance.now();
      task.delayTime = Math.abs(task.runningTime - (task.finishTime - task.beginTime));
      task.totalTime = task.finishTime - task.beginTime;
    }
  }, {
    key: "isBusy",
    get: function get() {
      return this._isBusy;
    }
  }]);

  return Process;
}();

exports.default = Process;