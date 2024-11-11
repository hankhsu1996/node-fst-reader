const addon = require("../build/Release/fst_reader");

export class FstReader {
  private ctx: any;

  constructor(filePath: string) {
    this.ctx = addon.openFstFile(filePath);
  }

  getStartTime(): number {
    return addon.getStartTime(this.ctx);
  }

  getEndTime(): number {
    return addon.getEndTime(this.ctx);
  }

  getScopeId(scopeName: string): number {
    return addon.getScopeId(this.ctx, scopeName);
  }

  getSignalId(signalName: string, startScopeId = 0): number {
    return addon.getSignalId(this.ctx, signalName, startScopeId);
  }

  getSignalValueAtTime(signalId: number, time: number): string {
    const buffer = Buffer.alloc(256);
    addon.getSignalValueAtTime(this.ctx, time, signalId, buffer);

    const nullByteIndex = buffer.indexOf(0);
    const length = nullByteIndex === -1 ? 64 : nullByteIndex;

    return buffer.toString("utf8", 0, length).trim();
  }
}
