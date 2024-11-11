# Node FST Reader

A Node.js library for reading FST (Fast Signal Trace) waveform files.

This package provides bindings for `libfst`, the library used by [GTKWave](https://github.com/gtkwave/gtkwave), allowing you to read and extract signal values from FST files within your Node.js applications.

## API

```typescript
// Open the FST file at the given path
const reader = new FstReader(fstFilePath);

// Get the start time of the FST file
const startTime = reader.getStartTime();

// Get the end time of the FST file
const endTime = reader.getEndTime();

// Retrieve the scope ID for a specific scope name
const scopeId = reader.getScopeId("my_scope");

// Retrieve the signal ID within a given scope
const signalId = reader.getSignalId("my_signal", scopeId);

// Get the signal value at a specific time using the signal ID
const valueAtTime100 = reader.getSignalValueAtTime(signalId, 100);

// Automatic cleanup; no need to manually close the reader
```

## Notes

- This project is still under development. Please check the source code for the latest updates.
