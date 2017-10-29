'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }(); //
// modelController.js
//


var _variables = require('./common/variables.js');

var _variables2 = _interopRequireDefault(_variables);

var _observer = require('./common/observer.js');

var _observer2 = _interopRequireDefault(_observer);

var _process = require('./Model/process.js');

var _process2 = _interopRequireDefault(_process);

var _task = require('./Model/task.js');

var _task2 = _interopRequireDefault(_task);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

var ModelController = function () {
	function ModelController() {
		_classCallCheck(this, ModelController);

		this.time = 1000;
		this.processor = new _process2.default('Single Process');

		this.queues = [{
			name: '1',
			tasks: [new _task2.default('1', 200), new _task2.default('2', 150), new _task2.default('2', 150)]
		}, {
			name: '2',
			tasks: [new _task2.default('1', 100), new _task2.default('2', 50), new _task2.default('3', 200)]
		}, {
			name: '3',
			tasks: [new _task2.default('1', 50), new _task2.default('2', 25), new _task2.default('3', 100)]
		}, {
			name: '4',
			tasks: [new _task2.default('1', 55), new _task2.default('2', 65), new _task2.default('3', 40)]
		}, {
			name: '5',
			tasks: [new _task2.default('1', 80), new _task2.default('2', 35), new _task2.default('3', 40)]
		}, {
			name: '6',
			tasks: [new _task2.default('2', 75), new _task2.default('3', 100)]
		}, {
			name: '7',
			tasks: [new _task2.default('1', 150)]
		}, {
			name: '8',
			tasks: [new _task2.default('1', 50), new _task2.default('2', 250)]
		}, {
			name: '9',
			tasks: [new _task2.default('1', 75), new _task2.default('2', 25), new _task2.default('3', 10)]
		}, {
			name: '10',
			tasks: [new _task2.default('1', 20), new _task2.default('3', 10)]
		}];
	}

	_createClass(ModelController, [{
		key: 'runProcess',
		value: function runProcess(queues) {
			var _this = this;

			var counterDone = 0;
			this.processor.beginTime = performance.now();

			var intervalTasks = setInterval(function () {
				if (!_this.processor.isBusy) {
					_this._carriage(queues[counterDone], _this.processor);
					counterDone++;
				}

				if (counterDone === queues.length) {
					_this.processor.finishTime = performance.now();
					clearInterval(intervalTasks);
					_observer2.default.emit(_variables2.default.tasksIsDone, queues, _this.processor);
				}
			}, 0);
		}
	}, {
		key: '_carriage',
		value: function _carriage(queue, process) {
			queue.beginTime = performance.now();
			queue.delayTime = queue.beginTime - process.beginTime;

			var _iteratorNormalCompletion = true;
			var _didIteratorError = false;
			var _iteratorError = undefined;

			try {
				for (var _iterator = queue.tasks[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
					var task = _step.value;

					task.beginTime = performance.now();
					process.makingFIFO(task);
					task.done = true;
				}
			} catch (err) {
				_didIteratorError = true;
				_iteratorError = err;
			} finally {
				try {
					if (!_iteratorNormalCompletion && _iterator.return) {
						_iterator.return();
					}
				} finally {
					if (_didIteratorError) {
						throw _iteratorError;
					}
				}
			}

			queue.finishTime = performance.now();
			queue.totalTime = queue.finishTime - queue.beginTime;
			queue.isDone = true;
			_observer2.default.emit(_variables2.default.isFreeTaskFIFO, queue.tasks);
		}
	}]);

	return ModelController;
}();

exports.default = ModelController;