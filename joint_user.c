#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct joint_user
{
    int userid;
    char username1[25];
    char username2[25];
    char password[20];
    int account_num;
    float balance;
    bool is_deleted;
}joint_user;

int main(void)
{
    int fd=open("joint_user_details.bin",O_CREAT|O_RDWR,0744);
    int option=1;
    joint_user new_user;
    while(option)
    {
	    printf("Enter userid :");
	    scanf("%d",&new_user.userid);
	    printf("Enter username:");
	    scanf(" %[^\n]",new_user.username1);
	    printf("Enter other username:");
	    scanf(" %[^\n]",new_user.username2);
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
	    write(fd,&new_user,sizeof(joint_user));
	    printf("Enter 0 to exit and any other number to continue : ");
	    scanf("%d",&option);
    }
    close(fd);
    // int fd1 = open("joint_user_details.bin",O_RDONLY);
    // joint_user old_user;
    // while(read(fd1,&old_user,sizeof(joint_user)))
    // {
    //     printf("%d\n",old_user.userid);
    //     printf("%s\n",old_user.password);
    // }
    // // read(fd1,&old_user,sizeof(old_user));
    // close(fd1);
    // printf("%d\n",old_user.userid);
    // printf("%s\n",old_user.password);
    // printf("%.2f\n",old_user.balance);
    // if (!strcmp(new_user.password,old_user.password)) printf("password correct\n");
    // if (new_user.userid==old_user.userid) printf("correct\n");
}