const addon = require("../build/Release/fst_reader");

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

  getEndTime(): number {
    return addon.getEndTime(this.ctx);
  }

  getSignalHandle(signalName: string): number {
    return addon.getSignalHandle(this.ctx, signalName);
  }

  getSignalValueAtTime(signalHandle: number, time: number): string {
    const buffer = Buffer.alloc(256);
    addon.getSignalValueAtTime(this.ctx, time, signalHandle, buffer);

    const nullByteIndex = buffer.indexOf(0);
    const length = nullByteIndex === -1 ? 64 : nullByteIndex;

    return buffer.toString("utf8", 0, length).trim();
  }
}
