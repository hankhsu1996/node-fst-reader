# Node FST Reader

[![Build and Test](https://github.com/hankhsu1996/node-fst-reader/actions/workflows/build-and-test.yml/badge.svg?event=push)](https://github.com/hankhsu1996/node-fst-reader/actions/workflows/build-and-test.yml)

A Node.js library for reading FST (Fast Signal Trace) waveform files.

This package provides bindings for `libfst`, the library used by [GTKWave](https://github.com/gtkwave/gtkwave), allowing you to read and extract signal values from FST files within your Node.js applications.

## API

```typescript
// Open the FST file at the given path
const reader: FstReader = new FstReader(fstFilePath);

// Get the start time of the FST file
const startTime: number = reader.getStartTime();

// Get the end time of the FST file
const endTime: number = reader.getEndTime();

// Retrieve the scope ID for a specific scope name
const scopeId: number = reader.getScopeId("my_scope");

// Retrieve the signal ID within a given scope
const signalId: number = reader.getSignalId("my_signal", scopeId);

// Retrieve the signal ID by specifying the full path, starting from the root
const signalIdFromRoot: number = reader.getSignalId("my_scope.my_signal");

// Get the signal value at a specific time using the signal ID
const valueAtTime100: string = reader.getSignalValueAtTime(signalId, 100);

// Automatic cleanup; no need to manually close the reader
```

## Notes

- This project is still under development. Please check the source code for the latest updates.
