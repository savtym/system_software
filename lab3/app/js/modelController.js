//
// modelController.js
//


import Variables from './common/variables.js';
import Observer from './common/observer.js';

import Process from './Model/process.js';
import Task from './Model/task.js';

export default class ModelController {

  constructor() {
    this.time = 1000;
    this.processor = new Process('Single Process');

		this.queues = [{
			name: '1',
			tasks: [
				new Task('1', 200),
				new Task('2', 150),
				new Task('2', 150)]
		}, {
			name: '2',
			tasks: [
				new Task('1', 100),
				new Task('2', 50),
				new Task('3', 200)]
		}, {
			name: '3',
			tasks: [
				new Task('1', 50),
				new Task('2', 25),
				new Task('3', 100)]
		}, {
			name: '4',
			tasks: [
				new Task('1', 55),
				new Task('2', 65),
				new Task('3', 40)]
		}, {
			name: '5',
			tasks: [
				new Task('1', 80),
				new Task('2', 35),
				new Task('3', 40)]
		}, {
			name: '6',
			tasks: [
				new Task('2', 75),
				new Task('3', 100)]
		}, {
			name: '7',
			tasks: [
				new Task('1', 150)]
		}, {
			name: '8',
			tasks: [
				new Task('1', 50),
				new Task('2', 250)]
		}, {
			name: '9',
			tasks: [
				new Task('1', 75),
				new Task('2', 25),
				new Task('3', 10)]
		}, {
			name: '10',
			tasks: [
				new Task('1', 20),
				new Task('3', 10)]
		}];
  }

  runProcess(queues) {
  	let counterDone = 0;
		this.processor.beginTime = performance.now();

		let intervalTasks = setInterval(() => {
			if (!this.processor.isBusy) {
				this._carriage(queues[counterDone], this.processor);
				counterDone++;
			}

			if (counterDone === queues.length) {
				this.processor.finishTime = performance.now();
				clearInterval(intervalTasks);
				Observer.emit(Variables.tasksIsDone, queues, this.processor);
			}
		}, 0);
	}


	_carriage(queue, process) {
		queue.beginTime = performance.now();
		queue.delayTime = queue.beginTime - process.beginTime;

		for (let task of queue.tasks) {
			task.beginTime = performance.now();
			process.makingFIFO(task);
			task.done = true;
		}

		queue.finishTime = performance.now();
		queue.totalTime = queue.finishTime - queue.beginTime;
		queue.isDone = true;
		Observer.emit(Variables.isFreeTaskFIFO, queue.tasks);
  }

}
