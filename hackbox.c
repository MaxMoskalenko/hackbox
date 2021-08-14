#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int my_strcmp(const char *str1, const char *str2);

int ls(char *pathes[], int pathc, int is_long);
int ls_single(char *path, int is_long);
int cat(char *pathes[], int pathc);
int my_mkdir(char *pathes[], int pathc);
int cp(const char *src, const char *dst);
void help();

int main(int argc, char *argv[])
{

    if (my_strcmp(argv[1], "ls") == 1)
    {
        if (argc > 2 && my_strcmp(argv[2], "-l") == 1)
            ls(argv, argc, 1);
        else
            ls(argv, argc, 0);
    }
    else if (my_strcmp(argv[1], "cat") == 1)
    {
        int status = 0;
        if (argc == 2)
        {
            printf("You don`t specify file path\n");
            return 0;
        }

        cat(argv, argc);
    }
    else if (my_strcmp(argv[1], "mkdir") == 1)
    {
        if (argc == 2)
        {
            printf("Not enough parameters (new dir path is not specified)\n");
            return 0;
        }
        my_mkdir(argv, argc);
    }
    else if (my_strcmp(argv[1], "cp") == 1)
    {
        int status = 0;

        if (argc != 4)
        {
            printf("Not enough arguments to copy\n");
            return 0;
        }

        status = cp(argv[2], argv[3]);
        if (status == 1)
            printf("Cannot read source file\n");
        if (status == 2)
            printf("Unable to create destination file\n");
    }
    else
        help();

    return 0;
}

int my_strcmp(const char *str1, const char *str2)
{
    int i = 0;
    for (i = 0; str1[i] == str2[i] && str1[i] != 0 && str2[i] != 0; i++)
    {
    }

    return (str1[i] == 0 && str2[i] == 0) ? 1 : 0;
}

int ls(char *pathes[], int pathc, int is_long)
{
    for (int i = 2 + is_long; i < pathc; i++)
    {
        ls_single(pathes[i], is_long);
    }
    if (2 + is_long == pathc)
    {
        ls_single(".", is_long);
    }

    return 0;
}

int ls_single(char *path, int is_long)
{
    DIR *dirp = opendir(path);

    if (dirp == NULL)
    {
        printf("%s - no such directory\n", path);
        return 1;
    }

    struct dirent *ent;
    struct stat statbuf;

    printf("%s:\n", path);
    while ((ent = readdir(dirp)) != NULL)
    {
        if (is_long == 1)
        {
            if (stat(ent->d_name, &statbuf) == -1)
                continue;

            printf(" %3o ", statbuf.st_mode & 0777);
            printf(" %4d ", statbuf.st_nlink);

            struct passwd *pws;
            pws = getpwuid(statbuf.st_uid);

            printf(" %s ", pws->pw_name);

            struct group *gr;
            gr = getgrgid(statbuf.st_gid);

            printf(" %s ", gr->gr_name);

            printf(" %8lld ", statbuf.st_size);

            char date[100];
            strftime(date, 100, "%d %b %R", localtime(&statbuf.st_ctime));

            printf(" %s ", date);

            printf(" %s\n", ent->d_name);
        }
        else
        {
            printf("%s  ", ent->d_name);
        }
    }
    printf("\n\n");
    (void)closedir(dirp);
    return 0;
}

int cat(char *pathes[], int pathc)
{
    for (int i = 2; i < pathc; i++)
    {
        FILE *fp = fopen(pathes[i], "r");
        if (fp == NULL)
        {
            printf("Error. No such directory: %s\n", pathes[i]);
            continue;
        }

        char ch;
        while ((ch = fgetc(fp)) != EOF)
            putchar(ch);

        printf("\n");
        fclose(fp);
    }
    return 0;
}

int my_mkdir(char *pathes[], int pathc)
{
    for (int i = 2; i < pathc; i++)
    {

        int status = 0;
        status = mkdir(pathes[i], 0755);

        if (status != 0)
            printf("Directory %s wasn`t created. Error with mkdir occured\n", pathes[i]);
    }
    return 0;
}

int cp(const char *src, const char *dst)
{
    FILE *f_src = fopen(src, "r");

    if (f_src == NULL)
        return 1;

    remove(dst);
    FILE *f_dst = fopen(dst, "w");

    if (f_dst == NULL)
        return 2;

    char ch;
    while ((ch = fgetc(f_src)) != EOF)
        fprintf(f_dst, "%c", ch);

    fclose(f_src);
    fclose(f_dst);
    return 0;
}

void help()
{
    printf("Hello, I`m HELP command and I will help you with hackbox tool\n\n");
    printf("There are all availiable commands:\n\n");
    printf("ls [path] - ls command shows all files and directories in a current directory, also you can enter path parameter to specify path of directory to show\n\n");
    printf("cat [path] - cat shows content of specified file from path\n\n");
    printf("mkdir [path] - mkdir creates directory by a specific path\n\n");
    printf("cp [source] [destination] - copies file from the source to destination location\n\n");
}