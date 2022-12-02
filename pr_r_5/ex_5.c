#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

int list_file(const char* dirname) {
	    DIR* dir;
        struct dirent *item;
        struct stat file;
        if ( (dir = opendir(dirname)) != NULL) {
		printf("Reading files in: %s\n", dirname);
		item = readdir(dir);
		if (item == NULL) {
			perror("Error in readdir");
			return 1;
		   }
                while (item != NULL) {
			char path_to_current_file[256] = {0};
			strcat(path_to_current_file, dirname);
			strcat(path_to_current_file, "/");
			strcat(path_to_current_file, item->d_name);
			if (stat(path_to_current_file, &file) == -1) { 
                        	perror("Error during stat");
                                return 1;
                            }
                        if ( (item->d_type == DT_REG) && (file.st_size == 0)) {
                        	for (int i = 0; i < strlen(item->d_name); i++)
                                	if (item->d_name[i] == '~') 
                                        	if (remove(path_to_current_file) == 0) {
							printf("Deleted file: %s\n", item->d_name);
                                                	break;
                                                  } else {
                                                          perror("Error when delete");
                                                          return 1;
                                                  }
	                 }
		        else if ( (item->d_type == DT_DIR && strcmp(item->d_name, ".") != 0) && (strcmp(item->d_name, "..") != 0)) {
                int pid = fork();
                if (pid == 0) {
                    printf("\nCreating children proccess: %d\n", getpid());
                    printf("My parent: %d\n", getppid());
				    printf("Trying to open directory: %s\n", item->d_name);
			    	char path[256] = {0};
				    strcat(path, dirname);
				    strcat(path, "/");
				    strcat(path, item->d_name);
				    list_file(path);
                    } else if (pid < 0) {
                        perror("");
                        return 1;
                    }
                if (pid > 0)
                    printf("\nThis is parent proccess: %d\n", getpid());
                    wait(&pid);
                };
		        item = readdir(dir);
		        errno = 0; 
		        if ( (item == NULL) && (errno != 0)) {
				    perror("Error after readdir");
			    	return 1;
			    };
		      };
            }; 
             closedir(dir);
};


int main() {
	list_file(".");
	return 0;
}
