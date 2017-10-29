"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

//
// task.js
//

var Task = function Task(title, time, priority) {
  _classCallCheck(this, Task);

  this.title = title;
  this.time = time;
  this.done = false;
  this.durationTime = 0;
};

exports.default = Task;