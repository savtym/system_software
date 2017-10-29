'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

//
// viewController.js
//

var ViewController = function () {
	function ViewController(model) {
		_classCallCheck(this, ViewController);

		this.model = model;
	}

	_createClass(ViewController, [{
		key: 'taskIsDone',
		value: function taskIsDone(queues, process) {
			var data = [];

			// for average time
			var averageTime = 0;
			var _iteratorNormalCompletion = true;
			var _didIteratorError = false;
			var _iteratorError = undefined;

			try {
				for (var _iterator = queues[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
					var queue = _step.value;

					averageTime += queue.delayTime;
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

			averageTime /= queues.length;

			for (var i = queues.length; i > 0; i--) {
				data.push({ 'x': averageTime / i, 'y': i });
			}
			this._generateSVG([data], 0, averageTime + 100, queues.length + 1, '"Average time" >> Time (ms)');

			// for percent data
			data = [];
			var deltaTime = process.finishTime - process.beginTime;
			for (var _i = queues.length; _i > 0; _i--) {
				data.push({ 'x': queues[_i - 1].delayTime / deltaTime * 100, 'y': _i });
			}
			this._generateSVG([data], 0, 100, queues.length + 1, '"t(delay)/t(total)" >> %');

			// for waiting time
			data = [];
			for (var _i2 = queues.length; _i2 > 0; _i2--) {
				data.push({ 'x': queues[_i2 - 1].delayTime, 'y': _i2 });
			}

			this._generateSVG([data], 0, queues[queues.length - 1].delayTime + 10, queues.length + 1, '"Waiting time" >> Time (ms)');
		}
	}, {
		key: '_generateSVG',
		value: function _generateSVG(data, startX, finishX, lengthY, textX) {
			var _this = this;

			var colors = [];

			for (var i = 0; i < data.length; i++) {
				colors.push(this._getRandomColor());
			}

			//************************************************************
			// Create Margins and Axis and hook our zoom function
			//************************************************************
			var margin = { top: 20, right: 30, bottom: 50, left: 50 },
			    width = window.innerWidth - margin.left - margin.right * 2,
			    height = 250 - margin.top - margin.bottom;

			var x = d3.scale.linear().domain([startX, finishX]).range([0, width]);

			var y = d3.scale.linear().domain([0, lengthY]).range([height, 0]);

			var xAxis = d3.svg.axis().scale(x).tickSize(-height).tickPadding(10).tickSubdivide(true).orient("bottom");

			var yAxis = d3.svg.axis().scale(y).tickPadding(10).tickSize(-width).tickSubdivide(true).orient("left");

			var svg = d3.select("body").append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");

			svg.append("g").attr("class", "x axis").attr("transform", "translate(0," + height + ")").call(xAxis);

			svg.append("g").attr("class", "y axis").call(yAxis);

			svg.append("g").attr("class", "y axis").append("text").attr("class", "axis-label").attr("transform", "rotate(-90)").attr("y", -margin.left + 10).attr("x", -height / 2).text('# queue');

			svg.append("g").attr("class", "x axis").append("text").attr("class", "axis-label").attr("y", height + margin.top + 25).attr("x", width / 2).text(textX);

			svg.append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);

			//************************************************************
			// Create D3 line object and draw data on our SVG object
			//************************************************************
			var line = d3.svg.line().interpolate("linear").x(function (d) {
				return x(d.x);
			}).y(function (d) {
				return y(d.y);
			});

			svg.selectAll('.line').data(data).enter().append("path").attr("class", "line").attr("clip-path", "url(#clip)").attr('stroke', function () {
				return _this._getRandomColor();
			}).attr("d", line);

			//************************************************************
			// Draw points on SVG object based on the data given
			//************************************************************
			var points = svg.selectAll('.dots').data(data).enter().append("g").attr("class", "dots").attr("clip-path", "url(#clip)");

			points.selectAll('.dot').data(function (d, index) {
				var a = [];
				d.forEach(function (point) {
					a.push({ 'index': index, 'point': point });
				});
				return a;
			}).enter().append('circle').attr('class', 'dot').attr("r", 2.5).attr('fill', function (d) {
				return colors[d.index % colors.length];
			}).attr("transform", function (d) {
				return "translate(" + x(d.point.x) + "," + y(d.point.y) + ")";
			});
		}
	}, {
		key: '_getRandomColor',
		value: function _getRandomColor() {
			var letters = '0123456789ABC';
			var color = '#';
			for (var i = 0; i < 6; i++) {
				color += letters[Math.floor(Math.random() * 13)];
			}
			return color;
		}
	}]);

	return ViewController;
}();

exports.default = ViewController;