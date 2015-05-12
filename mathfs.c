#define FUSE_USE_VERSION 30


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include "mathfs.h"
#include <fuse/fuse.h>

/*********************
GROUP MEMBERS : Zachary Scott (solo)
*********************/

// array of operations
struct op operations[] = 
{
	{add, "add", "adds two numbers \n", 3},
	{sub, "sub", "subtracts two numbers\n", 3},
	{mul, "mul", "multiplies\n", 3},
	{divv, "div", "divides two numbers\n", 3},
	{fib, "fib", "gives the first n numbers in the fibbonaci sequence (excluding zero) \n", 2},
	{factor, "factor", "gives the prime factors of the number given, excluding itself and 1. if the arg is prime, it will simply print itself\n", 2},
	{expp, "exp", "raises first arg to the power of the second arg\n", 3},
	{NULL,NULL,NULL,-1}
};

static int mathfs_getattr(const char *path, struct stat *stbuf)	// FUSE API
{
	int res = 0;
	char tokens[128];                          // token buffer for strtok
	char *path_parsed[1024];
	int i = 0;								// token index
	int op_index = 0; 							// index in operation table

	memset(stbuf, 0, sizeof(struct stat));
	int size = strlen(path) + 1;	
	memcpy(tokens, path, size);
	
	path_parsed[i] = strtok(tokens, "/");

	if(path_parsed[i] == NULL)       // root directory , links to all functions (9)
	{ 
		path_parsed[i] = "/";
		stbuf->st_mode = S_IFDIR | 0775;
		stbuf->st_nlink = 9;
		return 0;
	}

	while(path_parsed[i] != NULL) // iterate through the full path, tokenizing via '/'
	{	
		i++;
		path_parsed[i] = strtok(NULL, "/");
	}
	
	
	
	while(operations[op_index].path_name != 0) 
	{
		char* pathname = operations[op_index].path_name; 		// name of function
		char* arg0 = path_parsed[0];  								// assign all the arg tokens we got 
		char* arg1 = path_parsed[1];
		char* arg2 = path_parsed[2];
		char* arg3 = path_parsed[3];
		int args_given = i;

		if(strcmp(arg0, pathname) == 0)  // correct path name
		{
			if(i == 1)
			{
				stbuf->st_mode = S_IFDIR | 0775;
				stbuf->st_nlink = 3;
				return 0;
			}
			else 
			{
				if(strcmp(arg1, "doc") == 0) // we want to see the documentation
				{
					stbuf->st_mode = S_IFREG | 0664;
					stbuf->st_nlink = 3;
					stbuf->st_size = strlen(operations[op_index].document);
					return 0;
				}
				else if(operations[op_index].args == args_given) // correct number of args!
				{
					stbuf->st_mode = S_IFREG | 0664;
					stbuf->st_nlink = 3;
					stbuf->st_size = strlen(operations[op_index].f(path_parsed[i-2],path_parsed[i-1]));
					return 0;
				}
				else if(i > operations[op_index].args)  // too many args :(
				{
					return -ENOENT;
				}
				else 
				{                                          // just a directory listing
					stbuf->st_mode = S_IFDIR | 0775;
					stbuf->st_nlink = 3;
					return 0;
				}
			}
		}
		op_index++;
	}
	
	if(operations[op_index].args == -1)
	{
		res = -ENOENT;
	}
	return res;
}

static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, // FUSE API
			 off_t offset, struct fuse_file_info *fi)	
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "add", NULL, 0);
		filler(buf, "sub", NULL, 0);
		filler(buf, "mul", NULL, 0);
		filler(buf, "div", NULL, 0);
		filler(buf, "exp", NULL, 0);
		filler(buf, "fib", NULL, 0);
		filler(buf, "factor", NULL, 0);
	}
	else if (strcmp(path, "/add") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/sub") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/mul") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/div") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/fib") == 0)
	{

		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/exp") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else if (strcmp(path, "/factor") == 0)
	{
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}
	else
	{
		return -ENOENT;
	}
	return 0;
}



static int mathfs_open(const char *path, struct fuse_file_info *fi)	
{
	int i = 0;
	int op_index = 0; 									// index in operation table
	char tokens[128];      										 // token buffer for strtok
	char *path_parsed[1024];
	int size = strlen(path) + 1;	
	memcpy(tokens, path, size);
	
	
	path_parsed[i] = strtok(tokens, "/");


	if(path_parsed[i] == NULL) // root directory
	{
		path_parsed[i] = "/";
		i++;
	}
	while(path_parsed[i] != NULL) //iterate to the end of the path
	{
		i++;
		path_parsed[i] = strtok(NULL, "/");
	}
	
	char* arg0 = path_parsed[0]; // first arg
	char* arg1 = path_parsed[1];
	
	int args_given = i;
	
	while(operations[op_index].path_name != 0) 	// loop through operations until we reach the end of the operations table (containing 0,0,0,0)
	{
		char* pathname = operations[op_index].path_name; 		// name of function

		if(strcmp(arg0, pathname) == 0) 	// check if we referenced an operation correctly
		{
			if(strcmp(arg1, "doc") == 0) 	// we want the doc
			 {
				if ((fi->flags & 3) != O_RDONLY) 	// not read only
				{
					return -EACCES;
				}
			}
			else if(operations[op_index].args == args_given)	//correct number of args
			{
				if ((fi->flags & 3) != O_RDONLY) // not read only
				{
					return -EACCES;
				}
			}
			break;       // if we got our pathname right initially, no need to loop further
		}
		op_index++;
	}
	
	if(operations[op_index].args == -1) // we got to the end of the operations array with no match -- return error
	{
		return -ENOENT;
	}

	return 0;
}

static int mathfs_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)	
{
	
	char tokens[128];
	char *path_parsed[1024];
	int i = 0;
	int op_index = 0; // index in operation table
	size_t len;
	(void) fi;

	int size_2 = strlen(path) + 1;	
	memcpy(tokens, path, size_2);
	
	path_parsed[i] = strtok(tokens, "/");

	if(path_parsed[i] == NULL)  // root directory, nothing after /
	{
		path_parsed[i] = "/";
		i++;
	}
	while(path_parsed[i] != NULL)  // tokenize the path we were given
	{
		i++;
		path_parsed[i] = strtok(NULL, "/");
	}
	
	

	int args_given = i;
	char* arg0 = path_parsed[0]; // first arg
	char* arg1 = path_parsed[1];
	
	while(operations[op_index].path_name != 0) 
	{
		char* pathname = operations[op_index].path_name; 		// name of function
		if(strcmp(arg0, pathname) == 0) 					// correct operation name
		{
			if(strcmp(path_parsed[1], "doc") == 0)  // we want to read the documentation
			{
				len = strlen(operations[op_index].document);
				if (offset < len) 
				{
					if (offset + size > len) // as per the hello_world example
					{
						size = len - offset;
					}
					memcpy(buf, operations[op_index].document + offset, size);
				} 
				else
				{
					size = 0;
				}	
				return size;
			}
			else if(operations[op_index].args == args_given) // dont want doc -- right number of args
			{
				len = strlen(operations[op_index].f(path_parsed[args_given-2], path_parsed[args_given-1])); // give the function the previous two arguments
				if (offset < len) 
				{
					if (offset + size > len) // as per the hello_world example
					{
						size = len - offset;
					}
					memcpy(buf, operations[op_index].f(path_parsed[args_given-2], path_parsed[args_given-1]) + offset, size);
				} 
				else
				{
					size = 0;
				}
				return size;
			}
			break;
		}
		op_index++; // move to next operation index
	}
	if(operations[op_index].args == -1) // reached the end of the operations table :(
	{
		return -ENOENT;
	}	
	return 0;
}




static struct fuse_operations mathfs_oper = {
    .getattr = mathfs_getattr,
    .readdir = mathfs_readdir,
    .open = mathfs_open,
    .read = mathfs_read,
};



int main(int argc, char **argv)
{
    return fuse_main(argc, argv, &mathfs_oper, NULL);
}
