#include "Filesystem.h"
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>

bool Filesystem::deleteDirectory(std::string path) {
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(path.c_str())) == NULL) {
                printf("Cannot open dir %s\n",path.c_str());
                return false;
        }

        while ((ent = readdir(dir)) != NULL) {
                std::string entry( ent->d_name );
                if(std::string(ent->d_name) == "." || std::string(ent->d_name) == "..") continue;
                struct stat st;
                stat(std::string(path+"/"+ent->d_name).c_str(),&st);
                if(st.st_mode & S_IFDIR) {
                        deleteDirectory(std::string(path+"/"+ent->d_name));
                } else {
			remove(std::string(path+"/"+ent->d_name).c_str());
		}
        }
	rmdir(path.c_str());
	return true;
}

bool Filesystem::deleteFile(const std::string& path) {
	struct stat s;
	bool ret = false;
	int rc = stat(path.c_str(),&s);
	if(rc == 0) {
		ret = (remove(path.c_str()) == 0);
	}
	return ret;
}

bool Filesystem::doesFileExist(const std::string& path)
{
	struct stat s;
	int rc = stat(path.c_str(),&s);
	return (rc == 0);
}
