#include <string>

namespace Filesystem {
	bool deleteDirectory(std::string path);
	bool deleteFile(const std::string& path);
	bool doesFileExist(const std::string& path);
}
