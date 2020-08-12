#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
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

void check_admin(int nsd,int id,char password[30])
{
    int status=0,fd;
    admin new_admin;
    fd = open("admin_details.bin",O_RDONLY);
    while(read(fd,&new_admin,sizeof(new_admin)))
    {
        if(!strcmp(new_admin.password,password) && (new_admin.adminid==id))
        {
            status = 1;
            write(1,"Logged in as admin\n",sizeof("Logged in as admin\n"));
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(int));
}

void check_user(int nsd,int id,char password[30])
{
    int status=0,fd;
    user new_user;
    fd = open("user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(user)))
    {
        if(!strcmp(new_user.password,password) && (new_user.userid==id) && (new_user.is_deleted==false))
        {
            status = 1;
            write(1,"Logged in as normal user\n",sizeof("Logged in as normal user\n"));
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(int));
}

void check_joint_user(int nsd,int id,char password[30])
{
int status=0,fd;
    joint_user new_joint_user;
    fd = open("joint_user_details.bin",O_RDONLY);
    while(read(fd,&new_joint_user,sizeof(joint_user)))
    {
        if(!strcmp(new_joint_user.password,password) && (new_joint_user.userid==id)  && (new_joint_user.is_deleted==false))
        {
            status = 1;
            write(1,"Logged in as joint user\n",sizeof("Logged in as joint user\n"));
            break;           
        }
    }
    close(fd);
    write(nsd,&status,sizeof(int));
}

void add_normal_user(int nsd)
{
    user previous__user,new_user;
    int status = 0,id=0,acc=32112;
    read(nsd,&new_user,sizeof(user));
    int fd = open("user_details.bin",O_RDWR);
    lseek(fd,-(sizeof(user)),SEEK_END);
    read(fd,&previous__user,sizeof(user));
    id = previous__user.userid;
    acc = previous__user.account_num;
    new_user.userid = id+1;
    new_user.account_num = acc+1;
    srand(time(NULL));
    float scale = rand()/(float) RAND_MAX;
    new_user.balance = scale*(50000);
    new_user.is_deleted = false;
    write(1,"Before critical section\n",sizeof("Before critical section\n"));
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = -sizeof(user);
    lock.l_len = sizeof(user);
    lock.l_pid = getpid();
    int lock_status = fcntl(fd,F_SETLKW,&lock);
    write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
    // sleep(10);
    write(fd,&new_user,sizeof(user));
    lock.l_type = F_UNLCK;
    lock_status = fcntl(fd,F_SETLKW,&lock);
    write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
    if(lock_status!=-1) status =1;
    close(fd);
    write(nsd,&new_user,sizeof(user));
    write(nsd,&status,sizeof(int));
}

void add_joint_user(int nsd)
{
    joint_user previous__user,new_user;
    int status = 0,id=0,acc = 23131;
    read(nsd,&new_user,sizeof(joint_user));
    int fd = open("joint_user_details.bin",O_RDWR);
    lseek(fd,-(sizeof(joint_user)),SEEK_END);
    read(fd,&previous__user,sizeof(joint_user));
    id = previous__user.userid;
    acc = previous__user.account_num;
    new_user.userid = id+1;
    new_user.account_num = acc+1;
    srand(time(NULL));
    float scale = rand()/(float) RAND_MAX;
    new_user.balance = scale*(50000);
    new_user.is_deleted = false;

    write(1,"Before critical section\n",sizeof("Before critical section\n"));
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = -sizeof(joint_user);
    lock.l_len = sizeof(joint_user);
    lock.l_pid = getpid();
    int lock_status = fcntl(fd,F_SETLKW,&lock);
    write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
    write(fd,&new_user,sizeof(joint_user));
    lock.l_type = F_UNLCK;
    lock_status = fcntl(fd,F_SETLKW,&lock);
    write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
    if(lock_status!=-1) status =1;
    close(fd);
    write(nsd,&new_user,sizeof(user));
    write(nsd,&status,sizeof(int));
}

void delete_normal_user(int nsd)
{
    int id,status=0;
    user new_user;
    read(nsd,&id,sizeof(int));
    int fd = open("user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            new_user.is_deleted = true;
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            write(fd,&new_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(int));
}

void delete_joint_user(int nsd)
{
    int id,status=0;
    joint_user new_user;
    read(nsd,&id,sizeof(int));
    int fd = open("joint_user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            new_user.is_deleted = true;
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            write(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(int));
}

void search_normal_user(int nsd)
{
    int id,status=0;
    user new_user;
    read(nsd,&id,sizeof(int));
    int fd = open("user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            read(fd,&new_user,sizeof(user));
            // sleep(10);
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    write(nsd,&new_user,sizeof(new_user));
}

void search_joint_user(int nsd)
{
    int id,status=0;
    joint_user new_user;
    read(nsd,&id,sizeof(int));
    int fd = open("joint_user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            read(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    write(nsd,&new_user,sizeof(new_user));
}

void modify_user_details(int nsd)
{
    int id,status=0;
    user new_user,old_user;
    read(nsd,&new_user,sizeof(user));
    int fd = open("user_details.bin",O_RDWR);
    // lseek(fd,(new_user.userid-1)*(sizeof(user)),SEEK_SET);
    while(read(fd,&old_user,sizeof(old_user)))
    {
        if((old_user.userid==new_user.userid) && (old_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            strcpy(old_user.username,new_user.username);
            strcpy(old_user.password,new_user.password);
            write(fd,&old_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
}

void modify_joint_user_details(int nsd)
{
    int id,status=0;
    joint_user new_user,old_user;
    read(nsd,&new_user,sizeof(joint_user));
    int fd = open("joint_user_details.bin",O_RDWR);
    while(read(fd,&old_user,sizeof(old_user)))
    {
        if((old_user.userid==new_user.userid) && (old_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            strcpy(old_user.username1,new_user.username1);
            strcpy(old_user.username2,new_user.username2);
            strcpy(old_user.password,new_user.password);
            write(fd,&old_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
}

void change_password(int nsd,int id)
{
    admin new_admin;
    int status=0;
    char password[20],new_pass[20];
    // read(nsd,&id,sizeof(int));
    read(nsd,&password,sizeof(password));
    read(nsd,&new_pass,sizeof(new_pass));
    int fd = open("admin_details.bin",O_RDWR);
    while(read(fd,&new_admin,sizeof(new_admin)))
    {
        if((new_admin.adminid==id) && !strcmp(new_admin.password,password))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(admin));
            lock.l_len = sizeof(admin);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(admin)),SEEK_CUR);
            strcpy(new_admin.password,new_pass);
            write(fd,&new_admin,sizeof(admin));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
}

void user_deposit(int nsd,int id)
{
    user new_user;
    float amount;
    int status=0;
    read(nsd,&amount,sizeof(float));
    int fd = open("user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            sleep(10);
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            new_user.balance = new_user.balance + amount;
            write(fd,&new_user,sizeof(user));
            getchar();
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&new_user.balance,sizeof(new_user.balance));
    write(nsd,&status,sizeof(int));
}

void user_withdraw(int nsd,int id)
{
    user new_user;
    float amount;
    int status=0;
    read(nsd,&amount,sizeof(float));
    int fd = open("user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            new_user.balance = new_user.balance - amount;
            if(new_user.balance<0) status = 0;
            else
            {
                write(fd,&new_user,sizeof(user));
                status = 1;
            }
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            break;
        }
    }
    close(fd);
    write(nsd,&new_user.balance,sizeof(new_user.balance));
    write(nsd,&status,sizeof(int));
}

void user_balance(int nsd,int id)
{
    user new_user;
    int status =0;
    int fd = open("user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            read(fd,&new_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    write(nsd,&new_user.balance,sizeof(new_user.balance));

}

void user_password(int nsd,int id)
{
    user new_user;
    int status=0;
    char password[20],new_pass[20];
    read(nsd,&password,sizeof(password));
    read(nsd,&new_pass,sizeof(new_pass));
    int fd = open("user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(user)))
    {
        if((new_user.userid==id) && !strcmp(new_user.password,password))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            strcpy(new_user.password,new_pass);
            write(fd,&new_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
}

void view_user_details(int nsd,int id)
{
    int status=0;
    user new_user;
    int fd = open("user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(user));
            lock.l_len = sizeof(user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(user)),SEEK_CUR);
            read(fd,&new_user,sizeof(user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    if(status) write(nsd,&new_user,sizeof(new_user));
}

void Exit(int nsd)
{
    write(1,"One client disconnected\n",sizeof("One client disconnected\n"));
    close(nsd);
    exit(0);
}

void joint_user_deposit(int nsd,int id)
{
    joint_user new_user;
    float amount;
    int status=0;
    read(nsd,&amount,sizeof(float));
    int fd = open("joint_user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            new_user.balance = new_user.balance + amount;
            write(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&new_user.balance,sizeof(new_user.balance));
    write(nsd,&status,sizeof(int));
}

void joint_user_withdraw(int nsd,int id)
{
    joint_user new_user;
    float amount;
    int status=0;
    read(nsd,&amount,sizeof(float));
    int fd = open("joint_user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if(new_user.is_deleted == false && new_user.userid==id)
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            sleep(10);
            // getchar();
            new_user.balance = new_user.balance - amount;
            if(new_user.balance<0) status = 0;
            else
            {
                write(fd,&new_user,sizeof(joint_user));
                status = 1;
            }
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            break;
        }
    }
    close(fd);
    write(nsd,&new_user.balance,sizeof(new_user.balance));
    write(nsd,&status,sizeof(int));
}

void joint_user_balance(int nsd,int id)
{
    joint_user new_user;
    int status =0;
    int fd = open("joint_user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            read(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    write(nsd,&new_user.balance,sizeof(new_user.balance));

}

void joint_user_password(int nsd,int id)
{
    joint_user new_user;
    int status=0;
    char password[20],new_pass[20];
    read(nsd,&password,sizeof(password));
    read(nsd,&new_pass,sizeof(new_pass));
    int fd = open("joint_user_details.bin",O_RDWR);
    while(read(fd,&new_user,sizeof(joint_user)))
    {
        if((new_user.userid==id) && !strcmp(new_user.password,password))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to write\n",sizeof("Locked record to write\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            strcpy(new_user.password,new_pass);
            write(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
}

void view_joint_user_details(int nsd,int id)
{
    int status=0;
    joint_user new_user;
    int fd = open("joint_user_details.bin",O_RDONLY);
    while(read(fd,&new_user,sizeof(new_user)))
    {
        if((new_user.userid==id) && (new_user.is_deleted==false))
        {
            write(1,"Before critical section\n",sizeof("Before critical section\n"));
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_CUR;
            lock.l_start = -(sizeof(joint_user));
            lock.l_len = sizeof(joint_user);
            lock.l_pid = getpid();
            int lock_status = fcntl(fd,F_SETLKW,&lock);
            write(1,"Locked record to read\n",sizeof("Locked record to read\n"));
            lseek(fd,-(sizeof(joint_user)),SEEK_CUR);
            read(fd,&new_user,sizeof(joint_user));
            lock.l_type = F_UNLCK;
            lock_status = fcntl(fd,F_SETLK,&lock);
            write(1,"Unlocked record\n",sizeof("Unlocked record\n"));
            status = 1;
            break;
        }
    }
    close(fd);
    write(nsd,&status,sizeof(status));
    if(status) write(nsd,&new_user,sizeof(new_user));
}

int main()
{
    int sd,nsd,user_type,id,check,login_num=0,admin_option,user_option,num;
    char password[20];
    
    struct sockaddr_in server_connection, client_connection;
    sd = socket(AF_INET,SOCK_STREAM,0);
    server_connection.sin_family = AF_INET;
    server_connection.sin_addr.s_addr = INADDR_ANY;
    server_connection.sin_port = htons(5555);
    bind(sd,(struct sockaddr*)&server_connection,sizeof(server_connection));
    listen(sd,10);
    write(1,"Waiting for clients\n",sizeof("Waiting for clients\n"));
    while(true)
    {   
        int len = sizeof(client_connection);
        nsd = accept(sd,(struct sockaddr *)&client_connection,&len);
        if(!fork())
        {
            write(1,"Entered server\n",sizeof("Entered server\n"));
            read(nsd,&user_type,sizeof(int));
            while(true)
            {
                if(user_type==1)
                {
                    admin new_admin;
                    read(nsd,&id,sizeof(new_admin.adminid));
                    read(nsd,&password,sizeof(new_admin.password));
                    check_admin(nsd,id,password);
                    break;
                }
                else if(user_type==2)
                {
                    user new_user;
                    read(nsd,&id,sizeof(new_user.userid));
                    read(nsd,&password,sizeof(new_user.password));
                    check_user(nsd,id,password);
                    break;
                }
                else if(user_type==3)
                {
                    joint_user new_joint_user;
                    read(nsd,&id,sizeof(new_joint_user.userid));
                    read(nsd,&password,sizeof(new_joint_user.password));
                    check_joint_user(nsd,id,password);
                    break;
                }
            }
            while(true)
            {
                if(user_type==1)
                {
                    read(nsd,&admin_option,sizeof(int));
                    if(admin_option==1) add_normal_user(nsd);
                    else if(admin_option==2) add_joint_user(nsd);
                    else if(admin_option==3) delete_normal_user(nsd);
                    else if(admin_option==4) delete_joint_user(nsd);
                    else if(admin_option==5) search_normal_user(nsd);
                    else if(admin_option==6) search_joint_user(nsd);
                    else if(admin_option==7) modify_user_details(nsd);
                    else if(admin_option==8) modify_joint_user_details(nsd);
                    else if(admin_option==9) change_password(nsd,id);
                    else if(admin_option==10) Exit(nsd);
                }
                else if(user_type==2)
                {
                    read(nsd,&user_option,sizeof(int));
                    if(user_option==1) user_deposit(nsd,id);
                    else if(user_option==2) user_withdraw(nsd,id);
                    else if(user_option==3) user_balance(nsd,id);
                    else if(user_option==4) user_password(nsd,id);
                    else if(user_option==5) view_user_details(nsd,id);
                    else if(user_option==6) Exit(nsd);
                }
                else if(user_type==3)
                {
                    read(nsd,&user_option,sizeof(int));
                    if(user_option==1) joint_user_deposit(nsd,id);
                    else if(user_option==2) joint_user_withdraw(nsd,id);
                    else if(user_option==3) joint_user_balance(nsd,id);
                    else if(user_option==4) joint_user_password(nsd,id);
                    else if(user_option==5) view_joint_user_details(nsd,id);
                    else if(user_option==6) Exit(nsd);
                }
            }
        }
        else close(nsd);
    }
    return 0;
}
