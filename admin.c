#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

typedef struct admin
{
    int adminid;
    char username[25];
    char password[20];
}admin;


int main(void)
{
    int fd=open("admin_details.bin",O_CREAT|O_RDWR,0744);
    int option=1;
    admin ad;
    while(option)
    {
	    printf("Enter adminid :");
	    scanf("%d",&ad.adminid);
	    printf("Enter username:");
	    scanf(" %[^\n]",ad.username);
	    printf("Enter password :");
	    scanf(" %[^\n]",ad.password);
	    write(fd,&ad,sizeof(admin));
	    printf("Enter 0 to exit and any other number to continue : ");
	    scanf("%d",&option);
    }
    close(fd);
    // int fd1 = open("admin_details.bin",O_RDONLY);
    // admin old_user;
    // while(read(fd1,&old_user,sizeof(old_user)))
    // {
    //     printf("%d\n",old_user.adminid);
    //     printf("%s\n",old_user.password);
    // }
    // close(fd1);
    // read(fd1,&ad1,sizeof(ad1));
    // close(fd1);
    // printf("%d\n",ad1.adminid);
    // printf("%s\n",ad1.password);
    // if (!strcmp(ad.password,ad1.password)) printf("password correct\n");
    // if (ad1.adminid==ad.adminid) printf("correct\n");
}





