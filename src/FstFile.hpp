#pragma once

#include "FstHierarchyBuilder.hpp"
#include "fstapi.h"
#include <memory>
#include <string>

class FstFile {
public:
  explicit FstFile(const std::string &filePath) {
    ctx_ = fstReaderOpen(filePath.c_str());
    if (!ctx_) {
      throw std::runtime_error("Failed to open FST file: Invalid path or file");
    }
    hierarchyBuilder_ = std::make_unique<FstHierarchyBuilder>(ctx_);
    hierarchyBuilder_->buildHierarchy();
  }

  ~FstFile() {
    if (ctx_) {
      fstReaderClose(ctx_);
    }
  }

  uint64_t getStartTime() const { return fstReaderGetStartTime(ctx_); }

  uint64_t getEndTime() const { return fstReaderGetEndTime(ctx_); }

  scopeId getScopeId(const std::string &path) const {
    return hierarchyBuilder_->getScopeId(path);
  }

  signalId getSignalId(const std::string &path,
                       scopeId startingScope = 0) const {
    return hierarchyBuilder_->getSignalId(path, startingScope);
  }

  std::string getSignalValueAtTime(uint64_t time, signalId signal,
                                   char *buffer) const {
    char *result =
        fstReaderGetValueFromHandleAtTime(ctx_, time, signal, buffer);
    if (result == nullptr) {
      throw std::runtime_error("Failed to retrieve signal value.");
    }
    return std::string(result);
  }

  void *getCtx() const { return ctx_; }

private:
  void *ctx_;
  std::unique_ptr<FstHierarchyBuilder> hierarchyBuilder_;
};
