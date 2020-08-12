#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct user
{
    int userid;
    char username[25];
    char password[20];
    int account_num;
    float balance;
    bool is_deleted;
}user;

int main(void)
{
    int fd=open("user_details.bin",O_CREAT|O_RDWR,0744);
    int option=1;
    user new_user;
    while(option)
    {
	    printf("Enter userid :");
	    scanf("%d",&new_user.userid);
	    printf("Enter username:");
	    scanf(" %[^\n]",new_user.username);
	    printf("Enter password :");
	    scanf(" %[^\n]",new_user.password);
        printf("Enter account number :");
	    scanf("%d",&new_user.account_num);
        srand(time(NULL));
        float scale = rand()/(float) RAND_MAX;
        new_user.balance = scale*(50000);
        new_user.is_deleted = false;
        // new_user.balance = rand()%50000;
        // printf("Enter balance :");
	    // scanf("%f",&new_user.balance);
	    write(fd,&new_user,sizeof(user));
	    printf("Enter 0 to exit and any other number to continue : ");
	    scanf("%d",&option);
    }
    close(fd);
    // int fd1 = open("user_details.bin",O_RDONLY);
    // user old_user;
    // while(read(fd1,&old_user,sizeof(old_user)))
    // {
    //     printf("%d\n",old_user.userid);
    //     printf("%s\n",old_user.password);
    // }
    // close(fd1);
    // read(fd1,&old_user,sizeof(old_user));
    // printf("%d\n",old_user.userid);
    // printf("%s\n",old_user.password);
    // printf("%.2f\n",old_user.balance);
    // if (!strcmp(new_user.password,old_user.password)) printf("password correct\n");
    // if (new_user.userid==old_user.userid) printf("correct\n");
}