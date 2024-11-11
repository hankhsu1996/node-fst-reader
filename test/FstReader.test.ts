import { FstReader } from "../src/FstReader";
import * as path from "path";

describe("FstReader", () => {
  const fstFilePath = path.resolve(__dirname, "../test/data/hdl-example.fst");

  test("should open FST file and get start time", () => {
    const reader = new FstReader(fstFilePath);
    const startTime = reader.getStartTime();
    expect(typeof startTime).toBe("number");
    expect(startTime).toBeGreaterThanOrEqual(0);
  });

  test("should open FST file and get end time", () => {
    const reader = new FstReader(fstFilePath);
    const endTime = reader.getEndTime();
    expect(typeof endTime).toBe("number");
    expect(endTime).toBeGreaterThanOrEqual(0);
  });

  test("should open FST file and get scope ID", () => {
    const reader = new FstReader(fstFilePath);
    const scopeId = reader.getScopeId("fejkon_fc_debug");
    expect(typeof scopeId).toBe("number");
    expect(scopeId).toBeGreaterThan(0);
  });

  test("should open FST file and get signal ID", () => {
    const reader = new FstReader(fstFilePath);
    const scopeId = reader.getScopeId("fejkon_fc_debug");
    const signalId = reader.getSignalId("reset", scopeId);
    expect(typeof signalId).toBe("number");
    expect(signalId).toBeGreaterThan(0);
  });

  test("should open FST file and get signal value at specific times", () => {
    const reader = new FstReader(fstFilePath);
    const signalId = reader.getSignalId("fejkon_fc_debug.reset");
    expect(signalId).toBeGreaterThan(0);
    expect(reader.getSignalValueAtTime(signalId, 100)).toBe("0");
    expect(reader.getSignalValueAtTime(signalId, 200)).toBe("1");
    expect(reader.getSignalValueAtTime(signalId, 300)).toBe("0");
    expect(reader.getSignalValueAtTime(signalId, 400)).toBe("0");
  });
});
