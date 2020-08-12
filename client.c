#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<stdbool.h>

typedef struct admin
{
    int adminid;
    char username[25];
    char password[20];
}admin;

typedef struct user
{
    int userid;
    char username[25];
    char password[20];
    int account_num;
    float balance;
    bool is_deleted;
}user;

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

void admin_operation(int sd);

void user_operation(int sd,int type);

int admin_login(int sd)
{
    admin new_admin;
    int status=0;
    write(1,"Adminid : ",sizeof("Adminid : "));
    scanf("%d",&new_admin.adminid);
    write(1,"password : ",sizeof("password : "));
    scanf("%s",new_admin.password);
    write(sd,&new_admin.adminid,sizeof(new_admin.adminid));
    write(sd,&new_admin.password,sizeof(new_admin.password));
    read(sd,&status,sizeof(int));
    return status;
}

int user_login(int sd)
{
    user new_user;
    int status;
    write(1,"Userid : ",sizeof("Userid : "));
    scanf("%d",&new_user.userid);
    write(1,"password : ",sizeof("password : "));
    scanf("%s",new_user.password);
    write(sd,&new_user.userid,sizeof(new_user.userid));
    write(sd,&new_user.password,sizeof(new_user.password));
    read(sd,&status,sizeof(int));
    return status;
}

int jointuser_login(int sd)
{
    joint_user newjoint_user;
    int status;
    write(1,"Join user id : ",sizeof("Join user id : "));
    scanf("%d",&newjoint_user.userid);
    write(1,"password : ",sizeof("password : "));
    scanf("%s",newjoint_user.password);
    write(sd,&newjoint_user.userid,sizeof(newjoint_user.userid));
    write(sd,&newjoint_user.password,sizeof(newjoint_user.password));
    read(sd,&status,sizeof(int));
    return status;
}

void login(int sd,int arr[])
{
    int type,status;
    write(1,"Login Menu\nEnter corresponding number to login\n",sizeof("Login Menu\nEnter corresponding number to login\n"));
    write(1,"1 : Admin\n",sizeof("1 : Admin\n"));
    write(1,"2 : Normal user\n",sizeof("2 : Normal user\n"));
    write(1,"3 : Joint User\n",sizeof("3 : Joint user\n"));
    scanf("%d",&type);
    switch (type)
    {
    case 1 : write(sd,&type,sizeof(type));
        status = admin_login(sd);
        break;
    case 2 : write(sd,&type,sizeof(type));
        status = user_login(sd);
        break;
    case 3 : write(sd,&type,sizeof(type));
        status = jointuser_login(sd);
        break;
    default: write(1,"Invalid input\n",sizeof("Invalid input\n"));
        status = 2;
        break;
    }
    if(status==1) write(1,"Logged in succeessfully\n",sizeof("Logged in succeessfully\n"));
    else write(1,"Login unccussefull\n",sizeof("Login unccussefull\n"));
    arr[0] = type;
    arr[1] = status;
}

void add_new_user(int sd)
{
    int status=0;
    user new_user,new_user1;
    write(1,"Enter username : ",sizeof("Enter username : "));
	scanf(" %[^\n]",new_user.username);
    write(1,"Enter password : ",sizeof("Enter password : "));
	scanf(" %[^\n]",new_user.password);
    write(sd,&new_user,sizeof(user));
    read(sd,&new_user1,sizeof(user));
    read(sd,&status,sizeof(int));
    if(status)
    {
        write(1,"Your id is ",sizeof("Your id is "));
        printf("%d\n",new_user1.userid);
    }
    else write(1,"Unable to add new user\n",sizeof("Unable to add new user\n"));
    admin_operation(sd);
}

void add_new_joint_user(int sd)
{
    int status=0;
    joint_user new_user,new_user1;
    write(1,"Enter username\n",sizeof("Enter username\n"));
	scanf(" %[^\n]",new_user.username1);
    write(1,"Enter other username\n",sizeof("Enter other username\n"));
	scanf(" %[^\n]",new_user.username2);
    write(1,"Enter password\n",sizeof("Enter password\n"));
	scanf(" %[^\n]",new_user.password);
    write(sd,&new_user,sizeof(user));
    read(sd,&new_user1,sizeof(user));
    read(sd,&status,sizeof(int));
    if(status)
    {
        write(1,"Your id is ",sizeof("Your id is "));
        printf("%d\n",new_user1.userid);
    }
    else write(1,"Unable to add new user\n",sizeof("Unable to add new user\n"));
    admin_operation(sd);
}

void delete_normal_user(int sd)
{
    int status=0,id;
    write(1,"Enter userid of the user that you want to delete\n",sizeof("Enter userid of the user that you want to delete\n"));
    scanf("%d",&id);
    write(sd,&id,sizeof(int));
    read(sd,&status,sizeof(int));
    if(status) write(1,"Deleted successfully\n",sizeof("Deleted successfully\n"));
    else write(1,"Cannot delete\n",sizeof("Cannot delete\n"));
    admin_operation(sd);
}

void delete_joint_user(int sd)
{
    int status=0,id;
    write(1,"Enter joint userid of the user that you want to delete\n",sizeof("Enter joint userid of the user that you want to delete\n"));
    scanf("%d",&id);
    write(sd,&id,sizeof(int));
    read(sd,&status,sizeof(int));
    if(status) write(1,"Deleted successfully\n",sizeof("Deleted successfully\n"));
    else write(1,"Cannot delete\n",sizeof("Cannot delete\n"));
    admin_operation(sd);
}

void search_normal_user(int sd)
{
    int id,status=0;
    user new_user;
    write(1,"Enter userid\n",sizeof("Enter userid\n"));
    scanf("%d",&id);
    write(sd,&id,sizeof(id));
    read(sd,&status,sizeof(int));
    read(sd,&new_user,sizeof(user));
    if(status)
    {
        printf("User ID = %d\n",new_user.userid);
		printf("username = %s\n",new_user.username);
		printf("account number =%d\n",new_user.account_num);
		printf("Balance = %f\n",new_user.balance);
    }
    else write(1,"No user found\n",sizeof("No user found\n"));
    admin_operation(sd);
}

void search_joint_user(int sd)
{
    int id,status=0;
    joint_user new_user;
    printf("Enter userid\n");
    scanf("%d",&id);
    write(sd,&id,sizeof(id));
    read(sd,&status,sizeof(int));
    read(sd,&new_user,sizeof(user));
    if(status)
    {
        printf("User ID = %d\n",new_user.userid);
		printf("username = %s\n",new_user.username1);
        printf("other username = %s\n",new_user.username1);
		printf("account number =%d\n",new_user.account_num);
		printf("Balance = %f\n",new_user.balance);
    }
    else write(1,"No user found\n",sizeof("No user found\n"));
    admin_operation(sd);
}

void modify_user_details(int sd)
{
    user new_user,other_user;
    int status=0;
	write(1,"Enter userid\n",sizeof("Enter userid\n"));
	scanf("%d",&new_user.userid);
	write(1,"Enter new username\n",sizeof("Enter new username\n"));
	scanf(" %[^\n]",new_user.username);
	write(1,"Enter new password\n",sizeof("Enter new password\n"));
	scanf(" %[^\n]",new_user.password);
    write(sd,&new_user,sizeof(new_user));
    read(sd,&status,sizeof(status));
    if(status) write(1,"Modified Successfully\n",sizeof("Modified Successfully\n"));
    else write(1,"Cannot modify.Recheck userid\n",sizeof("Cannot modify.Recheck userid\n"));
    admin_operation(sd);
}

void modify_joint_user_details(int sd)
{
    int status = 0;
    joint_user new_user;
	write(1,"Enter userid\n",sizeof("Enter userid\n"));
	scanf("%d",&new_user.userid);
	write(1,"Enter new username\n",sizeof("Enter new username\n"));
	scanf(" %[^\n]",new_user.username1);
	write(1,"Enter another new username\n",sizeof("Enter another new username\n"));
	scanf(" %[^\n]",new_user.username2);
	write(1,"Enter new password\n",sizeof("Enter new password\n"));
	scanf(" %[^\n]",new_user.password);
    write(sd,&new_user,sizeof(new_user));
    read(sd,&status,sizeof(status));
    if(status) write(1,"Modified Successfully\n",sizeof("Modified Successfully\n"));
    else write(1,"Cannot modify.Recheck userid\n",sizeof("Cannot modify.Recheck userid\n"));
    admin_operation(sd);
}

void change_admin_password(int sd)
{
    char old_pass[20],new_pass[20];
    int status=0;
    write(1,"Enter old password\n",sizeof("Enter old password\n"));
    scanf(" %[^\n]",old_pass);
    write(1,"Enter new password\n",sizeof("Enter new password\n"));
    scanf(" %[^\n]",new_pass);
    write(sd,&old_pass,sizeof(old_pass));
    write(sd,&new_pass,sizeof(new_pass));
    read(sd,&status,sizeof(int));
    if(status) write(1,"Modified Successfully\n",sizeof("Modified Successfully\n"));
    else write(1,"Cannot modify.Recheck adminid or password\n",sizeof("Cannot modify.Recheck adminid or password\n"));
    admin_operation(sd);
}

void Exit(int sd)
{
    write(1,"Server disconnected\n",sizeof("Server disconnected\n"));
    close(sd);
    exit(0);
}

void admin_operation(int sd)
{
    int operation_type,next=0;
    write(1,"Enter number corresponding to your operation\n",sizeof("Enter number corresponding to your operation\n"));
    write(1,"1 : Add new normal user\n",sizeof("1 : Add new normal user\n"));
    write(1,"2 : Add new joint user\n",sizeof("2 : Add new joint user\n"));
    write(1,"3 : Delete normal user\n",sizeof("3 : Delete normal user\n"));
    write(1,"4 : Delete joint user\n",sizeof("4 : Delete joint user\n"));
    write(1,"5 : Search normal user\n",sizeof("5 : Search normal user\n"));
    write(1,"6 : Search joint user\n",sizeof("6 : Search joint user\n"));
    write(1,"7 : Modify normal user details\n",sizeof("7 : Modify normal user details\n"));
    write(1,"8 : Modify normal joint user details\n",sizeof("8 : Modify normal joint user details\n"));
    write(1,"9 : change admin password\n",sizeof("9 : change admin password\n"));    
    write(1,"10 : Exit\n",sizeof("6 : Exit\n"));
    scanf("%d",&operation_type);

    switch (operation_type)
    {
        case 1 : write(sd,&operation_type,sizeof(operation_type));
            add_new_user(sd);
            break;
        case 2 : write(sd,&operation_type,sizeof(operation_type));
            add_new_joint_user(sd);
            break;
        case 3 : write(sd,&operation_type,sizeof(operation_type));
            delete_normal_user(sd);
            break;
        case 4 : write(sd,&operation_type,sizeof(operation_type));
            delete_joint_user(sd);
            break;
        case 5 : write(sd,&operation_type,sizeof(operation_type));
            search_normal_user(sd);
            break;
        case 6 : write(sd,&operation_type,sizeof(operation_type));
            search_joint_user(sd);
            break;
        case 7 : write(sd,&operation_type,sizeof(operation_type));
            modify_user_details(sd);
            break;
        case 8 : write(sd,&operation_type,sizeof(operation_type));
            modify_joint_user_details(sd);
            break;
        case 9 : write(sd,&operation_type,sizeof(operation_type));
            change_admin_password(sd);
            break;
        case 10 : write(sd,&operation_type,sizeof(operation_type));
            Exit(sd);
            break;
        default: operation_type = 10;
            write(1,"Invalid operation\n",sizeof("Invalid operation\n"));
            write(sd,&operation_type,sizeof(operation_type));
            Exit(sd);
            break;
    }
}

void user_deposit(int sd,int type)
{
    float amount,balance;
    int status=0;
    write(1,"Enter amount to deposit\n",sizeof("Enter amount to deposit\n"));
    scanf("%f",&amount);
    write(sd,&amount,sizeof(float));
    read(sd,&balance,sizeof(balance));
    read(sd,&status,sizeof(int));
    if(status==1) 
    {
        write(1,"Transaction successful\n",sizeof("Transaction successful\n"));
        printf("New balance : %f\n",balance);
    }
    else write(1,"Unsuccessful\n",sizeof("Unsuccessful\n"));
    user_operation(sd,type);
}

void user_withdraw(int sd,int type)
{
    float amount,balance;
    int status=0;
    write(1,"Enter withdraw amount\n",sizeof("Enter withdraw amount\n"));
    scanf("%f",&amount);
    write(sd,&amount,sizeof(float));
    read(sd,&balance,sizeof(balance));
    read(sd,&status,sizeof(int));
    if(status==1) 
    {
        write(1,"Transaction successful\n",sizeof("Transaction successful\n"));
        printf("New balance : %f\n",balance);
    }
    else write(1,"Unsuccessful\n",sizeof("Unsuccessful\n"));
    user_operation(sd,type);
}

void balance_enquiry(int sd,int type)
{
    float balance;
    int status; 
    read(sd,&status,sizeof(status));
    read(sd,&balance,sizeof(balance));
    if(status==1) printf("New balance : %f\n",balance);
    else write(1,"No user found\n",sizeof("No user found\n")); 
    user_operation(sd,type);   
}

void change_user_password(int sd,int type)
{
    char old_pass[20],new_pass[20];
    int status=0;
    write(1,"Enter old password\n",sizeof("Enter old password\n"));
    scanf(" %[^\n]",old_pass);
    write(1,"Enter new password\n",sizeof("Enter new password\n"));
    scanf(" %[^\n]",new_pass);
    write(sd,&old_pass,sizeof(old_pass));
    write(sd,&new_pass,sizeof(new_pass));
    read(sd,&status,sizeof(int));
    if(status) write(1,"Modified Successfully\n",sizeof("Modified Successfully\n"));
    else write(1,"Cannot modify.Recheck adminid or password\n",sizeof("Cannot modify.Recheck adminid or password\n"));
    user_operation(sd,type);
}

void view_user_details(int sd,int type)
{
    int status=0;
    read(sd,&status,sizeof(status));
    if(status)
    {
        if(type==2)
        {
		    user new_user;
		    read(sd,&new_user,sizeof(new_user));
            printf("User ID = %d\n",new_user.userid);
		    printf("username = %s\n",new_user.username);
		    printf("account number =%d\n",new_user.account_num);
		    printf("Balance = %f\n",new_user.balance);	    
        }
	    else if(type==3)
        {
		    joint_user new_user;
		    read(sd,&new_user,sizeof(new_user));
		    printf("User ID = %d\n",new_user.userid);
		    printf("username = %s\n",new_user.username1);
		    printf("username = %s\n",new_user.username2);
		    printf("account number =%d\n",new_user.account_num);
		    printf("Balance = %f\n",new_user.balance);	    
        }
    }
	else write(1,"Cannot find user\n",sizeof("Cannot find user\\n"));
    user_operation(sd,type);
}

void user_operation(int sd,int type)
{
    int operation_type,next=0;
    write(1,"Enter number corresponding to your operation\n",sizeof("Enter number corresponding to your operation\n"));
    write(1,"1 : Deposit\n",sizeof("1 : Deposit\n"));
    write(1,"2 : Withdraw\n",sizeof("2 : Withdraw\n"));
    write(1,"3 : Balance Enquiry\n",sizeof("3 : Balance Enquiry\n"));
    write(1,"4 : Password Change\n",sizeof("4 : Password Change\n"));
    write(1,"5 : View detils\n",sizeof("5 : View detils\n"));
    write(1,"6 : Exit\n",sizeof("6 : Exit\n"));
    scanf("%d",&operation_type);
    switch (operation_type)
    {
        case 1 : write(sd,&operation_type,sizeof(operation_type));
            user_deposit(sd,type);
            break;
        case 2 : write(sd,&operation_type,sizeof(operation_type));
            user_withdraw(sd,type);
            break;
        case 3 : write(sd,&operation_type,sizeof(operation_type));
            balance_enquiry(sd,type);
            break;
        case 4 : write(sd,&operation_type,sizeof(operation_type));
            change_user_password(sd,type);
            break;
        case 5 : write(sd,&operation_type,sizeof(operation_type));
            view_user_details(sd,type);
            break;
        case 6 : write(sd,&operation_type,sizeof(operation_type));
            Exit(sd);
            break;
    
        default: operation_type = 6;
            write(1,"Invalid operation\n",sizeof("Invalid operation\n"));
            write(sd,&operation_type,sizeof(operation_type));
            Exit(sd);
            break;
    }  
}

int main()
{
    int sd,type,status=0;
    struct sockaddr_in serv;
    sd = socket(AF_INET,SOCK_STREAM,0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5555);
    connect(sd,(struct sockaddr *)&serv,sizeof(serv));
    int arr[2];
    login(sd,arr);
    type = arr[0];
    status = arr[1];
    if((type==1) && (status==1))
    {
        admin_operation(sd);
    }
    if((type==2 || type ==3) && (status==1))
    {
        user_operation(sd,type);
    }
    return 0;
}