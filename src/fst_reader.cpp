#include "fstapi.h"
#include <napi.h>

using namespace Napi;

// Function to wrap fstReaderOpen
Value OpenFstFile(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsString()) {
    throw TypeError::New(env, "Expected a string argument for file path");
  }

  std::string filePath = info[0].As<String>().Utf8Value();
  void *ctx = fstReaderOpen(filePath.c_str());

  if (!ctx) {
    throw Error::New(env, "Failed to open FST file: Invalid path or file");
  }

  return External<void>::New(env, ctx);
}

// Function to wrap fstReaderClose
void CloseFstFile(const CallbackInfo &info) {
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void *ctx = info[0].As<External<void>>().Data();
  fstReaderClose(ctx);
}

// Function to wrap fstReaderGetStartTime
Value GetStartTime(const CallbackInfo &info) {
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void *ctx = info[0].As<External<void>>().Data();
  uint64_t startTime = fstReaderGetStartTime(ctx);
  return Number::New(info.Env(), startTime);
}

// Function to wrap fstReaderGetEndTime
Value GetEndTime(const CallbackInfo &info) {
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to context");
  }

  void *ctx = info[0].As<External<void>>().Data();
  uint64_t endTime = fstReaderGetEndTime(ctx);
  return Number::New(info.Env(), endTime);
}

// Function to get signal handle by name
fstHandle getSignalHandle(void *ctx, const std::string &signalName) {
  struct fstHier *hier = fstReaderIterateHier(ctx);

  while (hier) {
    if (hier->htyp == FST_HT_VAR) {
      auto const &var = hier->u.var;

      if (signalName == var.name) {
        return var.handle;
      }
    }

    hier = fstReaderIterateHier(ctx);
  }

  return 0;
}

// Function to wrap `getSignalHandle`
Napi::Value GetSignalHandle(const CallbackInfo &info) {
  Env env = info.Env();

  // Ensure the arguments are valid (context pointer and signal name string)
  if (info.Length() < 2 || !info[0].IsExternal() || !info[1].IsString()) {
    throw TypeError::New(
        env,
        "Expected an external pointer to context and a signal name string");
  }

  void *ctx = info[0].As<External<void>>().Data();
  std::string signalName = info[1].As<String>().Utf8Value();

  fstHandle handle = getSignalHandle(ctx, signalName);

  return Number::New(env, handle);
}

// Wrapper for `fstReaderGetValueFromHandleAtTime`
Value GetSignalValueAtTime(const CallbackInfo &info) {
  if (info.Length() < 4 || !info[0].IsExternal() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsBuffer()) {
    throw TypeError::New(info.Env(),
                         "Expected context pointer, time (number), signal "
                         "handle (number), and buffer (Buffer)");
  }

  void *ctx = info[0].As<External<void>>().Data();
  uint64_t time = info[1].As<Number>().Int64Value();
  fstHandle handle = info[2].As<Number>().Int32Value();
  char *buf = (char *)info[3].As<Buffer<char>>().Data();

  // Call the underlying libfst function
  char *result = fstReaderGetValueFromHandleAtTime(ctx, time, handle, buf);

  // Return the buffer content as a string
  return String::New(info.Env(), result);
}

// Initialize and export the wrapped functions
Object Init(Env env, Object exports) {
  exports.Set("openFstFile", Function::New(env, OpenFstFile));
  exports.Set("closeFstFile", Function::New(env, CloseFstFile));
  exports.Set("getStartTime", Function::New(env, GetStartTime));
  exports.Set("getEndTime", Function::New(env, GetEndTime));
  exports.Set("getSignalHandle", Function::New(env, GetSignalHandle));
  exports.Set("getSignalValueAtTime", Function::New(env, GetSignalValueAtTime));
  return exports;
}

NODE_API_MODULE(fst_reader, Init)
