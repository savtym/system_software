//
// variables.js
//

const tasksIsDone = 'tasksIsDone';
const isFreeProcessFIFO = 'fifo-tasks';

export default class Variables {
	static get tasksIsDone() { return tasksIsDone; }
	static get isFreeTaskFIFO() { return isFreeProcessFIFO; }
}