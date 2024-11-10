# Node FST Reader

A Node.js library for reading FST (Fast Signal Trace) waveform files.

This package provides bindings for `libfst`, the library used by [GTKWave](https://github.com/gtkwave/gtkwave), allowing you to read and extract signal values from FST files within your Node.js applications.

## API

| Method                                                             | Description                                   |
| ------------------------------------------------------------------ | --------------------------------------------- |
| `constructor(filePath: string)`                                    | Opens the FST file at the given path.         |
| `close()`                                                          | Closes the opened FST file.                   |
| `getStartTime(): number`                                           | Returns the start time of the FST file.       |
| `getEndTime(): number`                                             | Returns the end time of the FST file.         |
| `getSignalHandle(signalName: string): number`                      | Returns the handle for the given signal name. |
| `getSignalValueAtTime(signalHandle: number, time: number): string` | Returns the signal value at a specific time.  |

## Notes

- This project is still under development. Please check the source code for the latest updates.
