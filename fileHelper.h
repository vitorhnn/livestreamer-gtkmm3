#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>

namespace fileHelper {

std::string duckTapegetenv(const std::string &variable);

void checkForOurDirectories();

std::string getConfigFilePath(const std::string &fileName);

std::string getDataFilePath(const std::string &fileName);

}




#endif
