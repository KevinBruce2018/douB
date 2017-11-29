#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#define USER_FILE "user.dat"
user *first_user = NULL;
void add_user()
{
	char try_again = 'Y';
	user *p = NULL;
	user *new_user = (user*)malloc(sizeof(user));
	
	while(try_again == 'Y' || try_again == 'y')
	{
		memset(new_user,0,sizeof(user));
		new_user->next = NULL;

		printf(">�����û���Ϣ...\n");
		input_user(&(new_user->ui));

		p = find_user(new_user->ui.username);
		if (p == NULL)
		{
			p = get_last_user();

			p->next = new_user;
			break;
		}
		printf(">�û�[%s]�Ѵ��ڡ����������룿��y or n):",new_user->ui.username);

		getchar();
			try_again = getchar();
			if (try_again != 'y' && try_again != 'y')
			{
				free(new_user);
			}
	}
}



	

void search_user()
{
	char input_char = 'Y';
	char username[MAX_USERNAME] = {0};
	user *p = NULL;
	while(input_char == 'Y' || input_char == 'y')
	{
		printf(">�����û���Ϣ...\n");
	    printf(">�������û�������󳤶�Ϊ %d):",MAX_USERNAME);
	    scanf("%s",username);
	    p = find_user(username);
	    if (p == NULL)
		{
		printf(">δ�ҵ��û�:%s����Ϣ�����������𣿣�yor n)",username);
		getchar();
		input_char = getchar();
		continue;
		}
	    show_user(&(p->ui));
	    printf(">���ҳɹ�����d/D��ɾ�����û�����u/U�����¸��û���Ϣ��������������!");
		getchar();
	    input_char = getchar();
	    if(input_char == 'd' || input_char == 'D')
		{
		delete_user(p);
		}
	    else if (input_char == 'U' || input_char == 'u')
		{
		update_user(p);
		}
	    printf(">search another?(y or n):");
	    getchar();
	    input_char = getchar();
	}
}


void delete_user(user *p)
{
	char input_char = 'N';
	user *previous = NULL;
	printf(">ȷ��Ҫɾ���û�[%s]��(y or n):",p->ui.username);
	getchar();
	input_char = getchar();
	if (input_char == 'Y' || input_char =='y')
	{
		previous = get_previous_user(p);
		previous->next = p->next;
		free(p);
		p = NULL;
	}
}


void update_user(user *p)
{
	char input = 'y';
	user *exist_p = NULL;
	user_info *new_p = (user_info*)malloc(sizeof(user_info));
	while(input == 'y' || input == 'Y')
	{
		memset(new_p,0,sizeof(user_info));
		input_user(new_p);
		exist_p = find_user(new_p->username);
		if(exist_p != NULL && exist_p != p)
		{
			printf(">�û�[%s]�Ѵ��ڣ���ѡ�������û�����\n",exist_p->ui.username);
			printf(">����������(y or n):");
			getchar();
			input = getchar();
		}
		else
		{strcpy(p->ui.username,new_p->username);
		strcpy(p->ui.password, new_p->password);
		p->ui.user_type = new_p->user_type;
		break;
		}
	}
	free(new_p);
}


void save_users()
{
	save_users_to_file();
	printf(">����ɹ��������������...");
	getchar();
	getchar();
}


void save_users_to_file()
{
	FILE *fp = fopen(USER_FILE,"wb");
	user *p = first_user;
	while(p != NULL)
	{
		fwrite(&(p->ui),sizeof(user_info),1,fp);
		fseek(fp,0,SEEK_END);
		p = p->next;
	}
	fclose(fp);
}






void init_user()
{
	FILE *fp=NULL;
	user_info default_admin;
	strcpy(default_admin.username,"admin");
	strcpy(default_admin.password,"123");
	default_admin.user_type=ADMIN;
	fp = fopen(USER_FILE,"r");
	if(fp==NULL)
	{
		fp=fopen(USER_FILE,"wb");
		fwrite(&default_admin,sizeof(user_info),1,fp);
	}
	fclose(fp);
}



void load_users()
{
	user *u=NULL;
	user *last=NULL;
	FILE *fp=NULL;
	int count=0;
	u=(user*)malloc(sizeof(user));
	memset(u,0,sizeof(user));
	u->next=NULL;
	fp=fopen(USER_FILE,"rb");
	fseek(fp,0,SEEK_SET);
	while(fread(&(u->ui),sizeof(user_info),1,fp)==1)
	{
		if(first_user==NULL)
		{
			first_user=u;
		}
		else
		{
			last=get_last_user();
			last->next=u;
		}
		count++;
		fseek(fp,count *sizeof(user_info),SEEK_SET);
		u=(user*)malloc(sizeof(user));
		memset(u,0,sizeof(user));
		u->next=NULL;
	}
	free(u);
	u=NULL;
	fclose(fp);
}



USER_TYPE login()
{
	char username[MAX_USERNAME]={0};
	char password[MAX_PASSWORD]={0};
	char try_again='Y';
	user *p=NULL;
	while(try_again='y'||try_again=='Y')
	{
		printf("�������û���:");
		scanf("%s",username);
		printf("����������:");
		scanf("%s",password);
		p=find_user(username);
		if(p==NULL)
		{
			printf("�û����������");
		}
		else if(strcmp(p->ui.password,password) !=0)
		{
			printf("�����������");
		}
		else
		{
			return p->ui.user_type;
		}
		printf(">����������?(y or n):");
		getchar();
		try_again=getchar();
	}
	exit(0);
}



void clear_users()
{
	user *p=NULL;
	while(first_user!=NULL)
	{
		if(first_user->next!=NULL)
		{
			p=first_user;
			first_user=first_user->next;
			free(p);
			p=NULL;
		}
		else
		{
			free(first_user);
			first_user=NULL;
		}
	}
}

user *get_last_user()
{
	user *p=first_user;
	while((p!=NULL)&&(p->next!=NULL))
	{
		p=p->next;
	}
	return p;
}



user *get_previous_user(user *p)
{
	user *previous=first_user;
	while(previous!=NULL)
	{
		if(previous->next==p)
		{
			break;
		}
		previous=previous->next;
	}
	return previous;
}


void show_user(user_info *info)
{
	printf("\n");
	printf("�û���:%s\n",info->username);
	printf("����:%S\n",info->password);
	printf("�û�����:%S\n",info->user_type==ADMIN?"admin":"user");
	printf("\n");
}



void input_user(user_info *info)
{
	printf(">�������û���(��󳤶�Ϊ%d):",MAX_USERNAME);
	scanf("%s",info->username);
	printf(">����������(��󳤶�Ϊ%d):",MAX_PASSWORD);
	scanf("%s",info->password);
	printf(">�������û�����(%d�ǹ���Ա��%d����ͨ�û�)",ADMIN,USER);
	scanf("%d",&(info->user_type));
}



user *find_user(char *name)
{
	user *p=first_user;
	int is_found=0;
	while(p!=NULL)
	{
		if(strcmp(p->ui.username,name)==0)
		{
			is_found=1;
			break;
		}
		p=p->next;
	}
	if(is_found)
	{
		return p;
	}
	return NULL;
}

