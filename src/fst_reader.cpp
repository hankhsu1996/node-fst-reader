#include "FstFile.hpp"
#include <napi.h>

using namespace Napi;

Value OpenFstFile(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsString()) {
    throw TypeError::New(env, "Expected a string argument for file path");
  }

  std::string filePath = info[0].As<String>().Utf8Value();
  auto fstFile = new FstFile(filePath);

  return External<FstFile>::New(env, fstFile,
                                [](Env, FstFile *file) { delete file; });
}

Value GetStartTime(const CallbackInfo &info) {
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to FstFile");
  }

  auto *fstFile = info[0].As<External<FstFile>>().Data();
  return Number::New(info.Env(), fstFile->getStartTime());
}

Value GetEndTime(const CallbackInfo &info) {
  if (info.Length() < 1 || !info[0].IsExternal()) {
    throw TypeError::New(info.Env(), "Expected an external pointer to FstFile");
  }

  auto *fstFile = info[0].As<External<FstFile>>().Data();
  return Number::New(info.Env(), fstFile->getEndTime());
}

Value GetScopeId(const CallbackInfo &info) {
  if (info.Length() < 2 || !info[0].IsExternal() || !info[1].IsString()) {
    throw TypeError::New(info.Env(), "Expected FstFile pointer and scope path");
  }

  auto *fstFile = info[0].As<External<FstFile>>().Data();
  std::string scopePath = info[1].As<String>().Utf8Value();
  return Number::New(info.Env(), fstFile->getScopeId(scopePath));
}

Value GetSignalId(const CallbackInfo &info) {
  if (info.Length() < 2 || !info[0].IsExternal() || !info[1].IsString()) {
    throw TypeError::New(info.Env(),
                         "Expected FstFile pointer and signal path");
  }

  auto *fstFile = info[0].As<External<FstFile>>().Data();
  std::string signalPath = info[1].As<String>().Utf8Value();
  scopeId startingScope = info.Length() > 2 && info[2].IsNumber()
                              ? info[2].As<Number>().Uint32Value()
                              : 0;
  return Number::New(info.Env(),
                     fstFile->getSignalId(signalPath, startingScope));
}

Value GetSignalValueAtTime(const CallbackInfo &info) {
  Env env = info.Env();

  if (info.Length() < 4 || !info[0].IsExternal() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsBuffer()) {
    throw TypeError::New(env, "Expected FstFile pointer, time (number), signal "
                              "handle (number), and buffer (Buffer)");
  }

  auto *fstFile = info[0].As<External<FstFile>>().Data();
  uint64_t time = info[1].As<Number>().Int64Value();
  fstHandle handle = info[2].As<Number>().Int32Value();
  Buffer<char> buffer = info[3].As<Buffer<char>>();

  char *buf = buffer.Data();
  std::string result = fstFile->getSignalValueAtTime(time, handle, buf);

  return String::New(env, result);
}

Object Init(Env env, Object exports) {
  exports.Set("openFstFile", Function::New(env, OpenFstFile));
  exports.Set("getStartTime", Function::New(env, GetStartTime));
  exports.Set("getEndTime", Function::New(env, GetEndTime));
  exports.Set("getScopeId", Function::New(env, GetScopeId));
  exports.Set("getSignalId", Function::New(env, GetSignalId));
  exports.Set("getSignalValueAtTime", Function::New(env, GetSignalValueAtTime));
  return exports;
}

NODE_API_MODULE(fst_reader, Init)
