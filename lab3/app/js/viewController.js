//
// viewController.js
//

export default class ViewController {

  constructor(model) {
  	this.model = model;
  }

  taskIsDone(queues, process) {
  	let data = [];

		// for average time
		let averageTime = 0;
		for (let queue of queues) {
			averageTime += queue.delayTime;
		}
		averageTime /= queues.length;

		for (let i = queues.length; i > 0; i--) {
			data.push({'x': averageTime / i, 'y': i});
		}
		this._generateSVG([data], 0, averageTime + 100, queues.length + 1, '"Average time" >> Time (ms)');


		// for percent data
		data = [];
		const deltaTime = process.finishTime - process.beginTime;
		for (let i = queues.length; i > 0; i--) {
			data.push({'x': (queues[i-1].delayTime / deltaTime) * 100, 'y': i});
		}
		this._generateSVG([data], 0, 100, queues.length + 1, '"t(delay)/t(total)" >> %');


		// for waiting time
		data = [];
		for (let i = queues.length; i > 0; i--) {
			data.push({'x': queues[i-1].delayTime, 'y': i});
		}

		this._generateSVG([data], 0, queues[queues.length-1].delayTime + 10, queues.length + 1, '"Waiting time" >> Time (ms)');
	}

	_generateSVG(data, startX, finishX, lengthY, textX) {
		const colors = [];

		for (let i = 0; i < data.length; i++) {
			colors.push(this._getRandomColor());
		}

		//************************************************************
		// Create Margins and Axis and hook our zoom function
		//************************************************************
		const margin = {top: 20, right: 30, bottom: 50, left: 50},
			width = window.innerWidth - margin.left - margin.right * 2,
			height = 250 - margin.top - margin.bottom;

		const x = d3.scale.linear()
			.domain([startX, finishX])
			.range([0, width]);

		const y = d3.scale.linear()
			.domain([0, lengthY])
			.range([height, 0]);

		const xAxis = d3.svg.axis()
			.scale(x)
			.tickSize(-height)
			.tickPadding(10)
			.tickSubdivide(true)
			.orient("bottom");

		const yAxis = d3.svg.axis()
			.scale(y)
			.tickPadding(10)
			.tickSize(-width)
			.tickSubdivide(true)
			.orient("left");


  	const svg = d3.select("body").append("svg")
			.attr("width", width + margin.left + margin.right)
			.attr("height", height + margin.top + margin.bottom)
			.append("g")
			.attr("transform", "translate(" + margin.left + "," + margin.top + ")");

		svg.append("g")
			.attr("class", "x axis")
			.attr("transform", "translate(0," + height + ")")
			.call(xAxis);

		svg.append("g")
			.attr("class", "y axis")
			.call(yAxis);

		svg.append("g")
			.attr("class", "y axis")
			.append("text")
			.attr("class", "axis-label")
			.attr("transform", "rotate(-90)")
			.attr("y", (-margin.left) + 10)
			.attr("x", -height/2)
			.text('# queue');

		svg.append("g")
			.attr("class", "x axis")
			.append("text")
			.attr("class", "axis-label")
			.attr("y", (height + margin.top) + 25)
			.attr("x", width/2)
			.text(textX);

		svg.append("clipPath")
			.attr("id", "clip")
			.append("rect")
			.attr("width", width)
			.attr("height", height);

		//************************************************************
		// Create D3 line object and draw data on our SVG object
		//************************************************************
		const line = d3.svg.line()
			.interpolate("linear")
			.x((d) => x(d.x))
			.y((d) => y(d.y));

		svg.selectAll('.line')
			.data(data)
			.enter()
			.append("path")
			.attr("class", "line")
			.attr("clip-path", "url(#clip)")
			.attr('stroke', () => this._getRandomColor())
			.attr("d", line);

		//************************************************************
		// Draw points on SVG object based on the data given
		//************************************************************
		const points = svg.selectAll('.dots')
			.data(data)
			.enter()
			.append("g")
			.attr("class", "dots")
			.attr("clip-path", "url(#clip)");

		points.selectAll('.dot')
			.data((d, index) => {
				const a = [];
				d.forEach((point) => {
					a.push({'index': index, 'point': point});
				});
				return a;
			})
			.enter()
			.append('circle')
			.attr('class','dot')
			.attr("r", 2.5)
			.attr('fill', (d,) => {
				return colors[d.index%colors.length];
			})
			.attr("transform", (d) =>
				"translate(" + x(d.point.x) + "," + y(d.point.y) + ")"
			);

	}

	_getRandomColor() {
		const letters = '0123456789ABC';
		let color = '#';
		for (let i = 0; i < 6; i++) {
			color += letters[Math.floor(Math.random() * 13)];
		}
		return color;
	}
}