#include <napi.h>
#include "fstapi.h"

using namespace Napi;

// Function to wrap fstReaderOpen
Value OpenFstFile(const CallbackInfo& info) {
  Env env = info.Env();

  // Ensure the argument is a string
  if (info.Length() < 1 || !info[0].IsString()) {
    throw TypeError::New(env, "Expected a string argument for file path");
  }

  std::string filePath = info[0].As<String>().Utf8Value();
  void* ctx = fstReaderOpen(filePath.c_str());

  // Check if the file was opened successfully
  if (!ctx) {
    throw Error::New(env, "Failed to open FST file: Invalid path or file");
  }

  return External<void>::New(env, ctx);
}

// Function to wrap fstReaderClose
void CloseFstFile(const CallbackInfo& info) {
  // Ensure the argument is an External pointer
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void* ctx = info[0].As<External<void>>().Data();
  fstReaderClose(ctx);
}

// Function to wrap fstReaderGetStartTime
Value GetStartTime(const CallbackInfo& info) {
  // Ensure the argument is an External pointer
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void* ctx = info[0].As<External<void>>().Data();
  uint64_t startTime = fstReaderGetStartTime(ctx);
  return Number::New(info.Env(), startTime);
}

// Function to wrap fstReaderGetEndTime
Value GetEndTime(const CallbackInfo& info) {
  // Ensure the argument is an External pointer
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void* ctx = info[0].As<External<void>>().Data();
  uint64_t endTime = fstReaderGetEndTime(ctx);
  return Number::New(info.Env(), endTime);
}

// Initialize and export the wrapped functions
Object Init(Env env, Object exports) {
  exports.Set("openFstFile", Function::New(env, OpenFstFile));
  exports.Set("closeFstFile", Function::New(env, CloseFstFile));
  exports.Set("getStartTime", Function::New(env, GetStartTime));
  exports.Set("getEndTime", Function::New(env, GetEndTime));
  return exports;
}

NODE_API_MODULE(fst_reader, Init)
