#include "../src/FstFile.hpp"
#include <cassert>
#include <iostream>

int main() {
  const std::string filePath = "../data/hdl-example.fst";

  try {
    FstFile fstFile(filePath);

    uint64_t startTime = fstFile.getStartTime();
    uint64_t endTime = fstFile.getEndTime();

    std::cout << "Start Time: " << startTime << std::endl;
    assert(startTime == 0);

    std::cout << "End Time: " << endTime << std::endl;
    assert(endTime > startTime);

    scopeId scope = fstFile.getScopeId("fejkon_fc_debug");
    std::cout << "Scope ID: " << scope << std::endl;
    assert(scope != 0);

    fstHandle handle = fstFile.getSignalId("reset", scope);
    std::cout << "Signal Handle: " << handle << std::endl;
    assert(handle != 0);

  } catch (const std::exception &e) {
    std::cerr << "Test failed: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "All tests passed." << std::endl;
  return 0;
}
