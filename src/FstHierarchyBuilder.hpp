#pragma once

#include "fstapi.h"
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using scopeId = uint32_t;
using signalId = fstHandle;

class ScopeNode {
public:
  scopeId id;
  std::string name;
  std::vector<scopeId> childScopes;
  std::vector<signalId> childSignals;

  ScopeNode() = default;
  ScopeNode(scopeId id, const std::string &name) : id(id), name(name) {}
};

class SignalNode {
public:
  scopeId parentScopeId;
  std::string name;
  signalId id;

  SignalNode() = default;
  SignalNode(scopeId parentScopeId, const std::string &name, signalId id)
      : parentScopeId(parentScopeId), name(name), id(id) {}
};

class FstHierarchyBuilder {
public:
  FstHierarchyBuilder(void *ctx) : ctx_(ctx), currentScopeId_(1) {
    scopes_.emplace(0, ScopeNode(0, "root"));
  }

  bool buildHierarchy() {
    std::vector<scopeId> scopeStack;
    scopeStack.push_back(0);

    fstReaderIterateHierRewind(ctx_);
    struct fstHier *hier;
    while ((hier = fstReaderIterateHier(ctx_)) != nullptr) {
      switch (hier->htyp) {
      case FST_HT_SCOPE: {
        std::string name(hier->u.scope.name, hier->u.scope.name_length);
        addScope(name, scopeStack.back());
        scopeStack.push_back(currentScopeId_ - 1);
        break;
      }
      case FST_HT_UPSCOPE: {
        if (!scopeStack.empty()) {
          scopeStack.pop_back();
        }
        break;
      }
      case FST_HT_VAR: {
        std::string name(hier->u.var.name, hier->u.var.name_length);
        addSignal(scopeStack.back(), name,
                  hier->u.var.handle); // Add signal to current scope
        break;
      }
      default:
        break;
      }
    }
    return true;
  }

  scopeId getScopeId(const std::string &path) const {
    auto pathParts = splitString(path, '.');
    const ScopeNode *currentNode = &scopes_.at(0);

    for (const auto &part : pathParts) {
      bool found = false;
      for (scopeId childScopeId : currentNode->childScopes) {
        if (scopes_.at(childScopeId).name == part) {
          currentNode = &scopes_.at(childScopeId);
          found = true;
          break;
        }
      }
      if (!found)
        return 0;
    }
    return currentNode->id;
  }

  signalId getSignalId(const std::string &path,
                       scopeId startingScope = 0) const {
    auto pathParts = splitString(path, '.');
    const ScopeNode *currentNode = &scopes_.at(startingScope);
    for (size_t i = 0; i < pathParts.size() - 1; ++i) {
      bool found = false;
      for (scopeId childScopeId : currentNode->childScopes) {
        if (scopes_.at(childScopeId).name == pathParts[i]) {
          currentNode = &scopes_.at(childScopeId);
          found = true;
          break;
        }
      }
      if (!found)
        return 0;
    }
    const auto &signalName = pathParts.back();
    for (signalId sigId : currentNode->childSignals) {
      const auto &signal = signals_.at(sigId);
      if (signal.name == signalName) {
        return signal.id;
      }
    }
    return 0;
  }

  std::unordered_map<scopeId, ScopeNode> getAllScopes() const {
    return scopes_;
  }

  std::unordered_map<signalId, SignalNode> getAllSignals() const {
    return signals_;
  }

  std::vector<scopeId> getChildScopes(scopeId id) const {
    return scopes_.at(id).childScopes;
  }

  std::vector<signalId> getChildSignals(scopeId id) const {
    return scopes_.at(id).childSignals;
  }

  std::string getScopeName(scopeId id) const { return scopes_.at(id).name; }

  std::string getSignalName(signalId id) const { return signals_.at(id).name; }

  scopeId getSignalParentScope(signalId id) const {
    return signals_.at(id).parentScopeId;
  }

private:
  void *ctx_;
  std::unordered_map<scopeId, ScopeNode> scopes_;
  std::unordered_map<signalId, SignalNode> signals_;
  std::unordered_map<signalId, scopeId> signalToScopeMap_;
  scopeId currentScopeId_;

  void addScope(const std::string &name, scopeId parentScopeId) {
    scopes_.emplace(currentScopeId_, ScopeNode(currentScopeId_, name));
    scopes_.at(parentScopeId).childScopes.push_back(currentScopeId_);
    currentScopeId_++;
  }

  void addSignal(scopeId parentScopeId, const std::string &name, signalId id) {
    signals_[id] = SignalNode(parentScopeId, name, id);
    scopes_.at(parentScopeId).childSignals.push_back(id);
    signalToScopeMap_[id] = parentScopeId; // Map signal ID to its parent scope
  }

  std::vector<std::string> splitString(const std::string &str,
                                       char delimiter) const {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
      tokens.push_back(token);
    }
    return tokens;
  }
};
