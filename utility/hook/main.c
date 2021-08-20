#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char buf[] = "hello world!";
    FILE *fp = fopen("out.txt", "w");
    fwrite(buf, strlen(buf), 1, fp);
    fclose(fp);

    fp = fopen("out.txt", "r");
    fread(buf, sizeof(buf), 1, fp);
    fclose(fp);

    return 0;
}