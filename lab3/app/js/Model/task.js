//
// task.js
//

export default class Task {
  constructor(title, time, priority) {
    this.title = title;
    this.time = time;
    this.done = false;
    this.durationTime = 0;
  }
}
