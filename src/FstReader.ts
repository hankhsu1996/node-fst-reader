const addon = require('../build/Release/fst_reader');

export class FstReader {
  private ctx: any;

  constructor(filePath: string) {
    this.ctx = addon.openFstFile(filePath);
  }

  close() {
    addon.closeFstFile(this.ctx);
  }

  getStartTime(): number {
    return addon.getStartTime(this.ctx);
  }
}
