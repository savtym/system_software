//
// appController.js
//

import Observer from './common/observer.js';
import Variables from './common/variables.js'

import ModelController from './modelController.js';
import ViewController from './viewController.js';

export class AppController {

	constructor() {
    this.model = new ModelController();
    this.view = new ViewController(this.model);

		Observer.addListener(Variables.isFreeTaskFIFO, this.isFreeProcessFIFO.bind(this));
		Observer.addListener(Variables.tasksIsDone, this.tasksIsDone.bind(this));

		this.model.runProcess(this.model.queues);
  }

	isFreeProcessFIFO(task) {
		console.log(task);
	}

	tasksIsDone(queues, process) {
		console.log(queues)
		this.view.taskIsDone(queues, process);
	}
}
