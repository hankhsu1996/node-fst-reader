import { FstReader } from "../src/FstReader";
import * as path from "path";

describe("FstReader", () => {
  const fstFilePath = path.resolve(__dirname, "../test/data/hdl-example.fst");

  test("should open FST file and get start time", () => {
    const reader = new FstReader(fstFilePath);
    const startTime = reader.getStartTime();
    expect(typeof startTime).toBe("number");
    expect(startTime).toBeGreaterThanOrEqual(0);
    reader.close();
  });

  test("should open FST file and get end time", () => {
    const reader = new FstReader(fstFilePath);
    const endTime = reader.getEndTime();
    expect(typeof endTime).toBe("number");
    expect(endTime).toBeGreaterThanOrEqual(0);
    reader.close();
  });

  test("should close FST file without error", () => {
    const reader = new FstReader(fstFilePath);
    expect(() => reader.close()).not.toThrow();
  });

  test("should open FST file and get signal handle", () => {
    const reader = new FstReader(fstFilePath);
    const signalHandle = reader.getSignalHandle("reset");
    expect(typeof signalHandle).toBe("number");
    expect(signalHandle).toBeGreaterThan(0);
    reader.close();
  });

  test("should open FST file and get signal value at specific times", () => {
    const reader = new FstReader(fstFilePath);
    const signalHandle = reader.getSignalHandle("reset");

    expect(signalHandle).toBeGreaterThan(0);

    expect(reader.getSignalValueAtTime(signalHandle, 100)).toBe("0");
    expect(reader.getSignalValueAtTime(signalHandle, 200)).toBe("1");
    expect(reader.getSignalValueAtTime(signalHandle, 300)).toBe("0");
    expect(reader.getSignalValueAtTime(signalHandle, 400)).toBe("0");

    reader.close();
  });
});
