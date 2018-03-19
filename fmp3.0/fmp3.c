/*************************************************************************
	> �ļ���: FMediaPlay v3.0
	> ���ߣ�ǧ������
	> ���: win�µĵ���bass��Ƶ��,��������ִ��.
	> ���ܣ����Ե��κ�ѭ������mp3
	> ����ʱ�䣺20180121
	
	> ʹ�÷�����
		1.����ݷ�ʽ�����Ҽ�<���͵�>�˵���ֱ�Ӱ�mp3���͵���������
		2.������fmp xxx.mp3
		3.����ѭ����fmp r xxx.mp3
		4.ָ����ŵ����һ��:fmp p number
		5.ָ����ŵ���ѭ��:fmp pr number
		6.��ʾ��ǰĿ¼�����б�fmp ls
		7.����ȫ����fmp l
		8.�ر����fmp s
		
	>ʹ��mgw_w64����,����mingw��win�±���ٷ�֮99��Ч���ο���������:
	gcc -Wall -os -s -mwindows fmp3.c -lwinmm -lbass -o fmp3.exe	
	ע������4Ϊ���ŵ�ǰĿ¼�²������ļ��е�����mp3	
	ע������windows�ļ���ȫ��ʹ�ø�����
*************************************************************************/
#include "bass.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
/************************************************************************/
char msg[256];
extern int __argc;
extern char ** __argv;

char shortBuffer[MAX_PATH];
char cmdBuff[MAX_PATH + 64];
char listname[31][100];
char utf8name[100];

int fmp_play(char* mpfile);
char *localeToUTF8(char *src);
/************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine, 
					int nCmdShow )
{
	int fmp_help();
	int fmp_stop();
	int fmp_list();
	int fmp_singlerepeat();
	int fmp_singleplay();
	int fmp_point();
	int fmp_pointrepeat();
	int fmp_lsmp3();
	int play_Dir();

	//����[fmp]��������
	if(__argc<2) {
		fmp_help();
	}
	
	else if(__argc==2){
		//����[fmp s]���豸ɱ�������
		if(strcmp(__argv[1],"s")==0){
			fmp_stop();
			return 0;
		}
		//����[fmp l]��ǰĿ¼ȫ�����ţ�30������
		else if(strcmp(__argv[1],"l")==0){
			fmp_list();
			return 0;
		}
		//����[fmp ls]��ǰĿ¼mp3�������
		else if(strcmp(__argv[1],"ls")==0){
			fmp_lsmp3();
			return 0;
		}
		//����[fmp xxx.mp3]���β��� 
		fmp_singleplay();
		play_Dir(__argv[1]);
		return 0;
	}


	else if(__argc==3) {  
		//����[fmp r xxx.mp3]����ѭ��
		if(strcmp(__argv[1],"r")==0){
			fmp_singlerepeat();
			return 0;
		}
		//����[fmp p number]ָ����Ų��ŵ�ĩ��
		else if(strcmp(__argv[1],"p")==0){
			fmp_point();
			return 0;
		}
		//����[fmp pr number]ָ����ŵ���ѭ��
		else if(strcmp(__argv[1],"pr")==0){
			fmp_pointrepeat();
			return 0;
		}
		
		//����Ҽ���ѡ����
		while(--__argc)fmp_play(*++__argv);
		return 0;
	}
	
	//����Ҽ���ѡ����
	while(--__argc)fmp_play(*++__argv);
	
	return 0;
}

/***************  ����Ϊ�Ӻ���ʵ�־��幦�ܲ���  *************************/

/************************************************************************/
//����[fmp]�������ں���ʵ��
int fmp_help()
{	
	sprintf(msg,"Fmediaplay-%I64dλ��v3.0\n�����ʽ:\n������fmp xxx.mp3\n����ѭ����fmp r xxx.mp3\nָ����ŵ����һ��:fmp p number\nָ�����ѭ������:fmp pr number\n��ʾ��ǰĿ¼�����б�fmp ls\n����ȫ����fmp l\n�ر����fmp s", sizeof(int*)*8);
	MessageBox(NULL,msg,"FMediaPlay ǧ������",MB_OK);
	return 1;
}
/************************************************************************/
//����[fmp s]����ʵ��
int fmp_stop()
{
	printf("ִ�йر����%s %s\n",__argv[0],__argv[1]);	
	char cmd[100];//����cmd���ַ���������

	BASS_Free();
	BASS_PluginFree(0);
	sleep(1);
	sprintf(cmd, "taskkill /f /t /im %s.exe",__argv[0]);
	WinExec(cmd,SW_HIDE);
	return 0;
}

/************************************************************************/
//����[fmp xxx.mp3]����ʵ��
int fmp_singleplay()
{
	//printf("���β���%s",__argv[1]);	
	fmp_play(__argv[1]);
	return 0;
}

/************************************************************************/
//����[fmp l]����ʵ��		
int fmp_list()
{	

	int i,j;
	char ext[50]="*.mp3";//��ź�׺��
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//��ǰĿ¼����mp3�ļ�

	strcpy(listname[1],p.cFileName);//��һ��д������
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]��ֵ���ˣ�listname[0]���������Ű�
	while(FindNextFile(h,&p)){
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30Ϊ��ʾ��д��listname[]��mp3��������
	}
	j=i;
	for(i=1;i<j;i++){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//����[fmp r xxx.mp3]����ʵ��
int fmp_singlerepeat()
{  
		GetShortPathName(__argv[2],shortBuffer,sizeof(shortBuffer));
		if(!*shortBuffer) {
			sprintf(msg,"Cannot shorten filename \"%s\"\n",__argv[2]);
			MessageBox(NULL,msg,"Fmediaplay",MB_OK);
			return 1;
		}  
		
		for(int i=1;i<99;i++){
			fmp_play(shortBuffer);
		}
	return 0;
}

/************************************************************************/
//����[fmp p number]����ʵ��	
int fmp_point()
{	

	int i,j;
	char ext[50]="*.mp3";//��ź�׺��

    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//��ǰĿ¼����mp3�ļ�
    //puts(p.cFileName);//ȥ��ע�ͣ���������ʱ�����ʲô
	strcpy(listname[1],p.cFileName);//��һ��д������
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]��ֵ���ˣ�listname[0]���������Ű�
	while(FindNextFile(h,&p)){
		//puts(p.cFileName);//ȥ��ע�ͣ���������ʱ�����ʲô
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30Ϊ��ʾ��д��listname[]��mp3��������
	}
	
	
	j=i;
	i=atoi(__argv[2]);
	sprintf(msg,"���ȷ�����ӵ�ǰ�������ŵ����һ��:\n\n��ǰ����%d %s\n...\n���һ��%d %s",i,listname[i],j-1,listname[j-1]);
	//MessageBox(NULL,msg,"fmp",MB_OKCANCEL);
	if(MessageBox(NULL,msg,"fmp",MB_OKCANCEL)==IDCANCEL)  
	return 0; 
	printf("���ŵĸ������: %d\n", i);

	for(;i<j;i++){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//����[fmp pr number]����ʵ��ָ����ŵ���ѭ��	
int fmp_pointrepeat()
{	

	int i,j;
	char ext[50]="*.mp3";//��ź�׺��
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//��ǰĿ¼����mp3�ļ�
	strcpy(listname[1],p.cFileName);//��һ��д������
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]��ֵ���ˣ�listname[0]���������Ű�
	while(FindNextFile(h,&p)){
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30Ϊ��ʾ��д��listname[]��mp3��������
	}
	
	
	j=i;
	i=atoi(__argv[2]);
	sprintf(msg,"���ȷ��ѭ������[��ǰ������:\n\n��ǰ����%d %s",i,listname[i]);
	//����ԭ��MessageBox(NULL,msg,"fmp",MB_OKCANCEL);
	if(MessageBox(NULL,msg,"fmp",MB_OKCANCEL)==IDCANCEL)  
	return 0; 
	printf("���ŵĸ������: %d\n", i);

	for(;i<j;){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//����[fmp ls]��ǰĿ¼mp3����б�
int fmp_lsmp3(int argc,char *argv[])
{	
	int i;
	char ext[50]="*.mp3";
	
	AllocConsole();//������������¿���̨����-mwindws������printf�ķ���֮һ
	freopen("CONOUT$", "w", stdout);
	
		WIN32_FIND_DATA p;
		HANDLE h=FindFirstFile(ext,&p);
		printf("��%d��\t%s\n",1,p.cFileName);
		i=2;
		while(FindNextFile(h,&p)){
		printf("��%d��\t%s\n",i,p.cFileName);
		i++;
		}
		getch();//system("pause")	
    return 0;
}

/************************************************************************/
//����Ŀ¼������֧�ֵĸ�ʽ
int play_Dir(char* p)
{
        DIR *dir = NULL; // Ŀ¼�ṹ
        struct dirent *ent = NULL; // Ŀ¼�µ��ļ�������Ŀ¼�Ľṹ
        
		dir = opendir(p);  // ��ȡĿ¼·��
        while (NULL != (ent = readdir(dir))) { // 
			char temppath[500];
			sprintf(temppath,"%s\\%s",p,ent->d_name);
			printf("%s\n",temppath);
			fmp_play(temppath);
		}
	return 0;
}


/************************************************************************/
//�����Ӻ�������bass��Ƶ��
int plugin()
{
	//��ȡ���;
	DIR *dir = NULL; // Ŀ¼�ṹ
	struct dirent *ent = NULL; // Ŀ¼�µ��ļ�������Ŀ¼�Ľṹ	
	char temppath[260],pluginpath[260];//����·�����Ͳ��Ŀ¼����
		
	GetModuleFileName(NULL,temppath,260);//��ȡ�������·����ȥ���ļ���
	char *ext=strrchr(temppath,'\\');
	if (ext)*ext='\0';    

    sprintf(pluginpath,"%s\\bassplugin\\",temppath);//д����·��
	
	dir = opendir(pluginpath);  //�򿪲��Ŀ¼����ȡ      
	while (NULL != (ent = readdir(dir))) {
		if (!strcmp(ent->d_name,".")||!strcmp(ent->d_name,".."))continue;
		sprintf(temppath,"%s%s",pluginpath,ent->d_name);//д����·��
		BASS_PluginLoad(temppath,0);
	}
	closedir(dir);

	return 0;
}
		
int fmp_play(char *soundfile)
{
	int TestSound;
	//��ʼ��(�����豸-1��ʾĬ��,������,Ч������,���ھ��,0��ʾʹ��Ĭ�����ݳ�ʼ��DirectSound)
	BASS_Init(-1, 44100, 0, 0, 0);
	plugin();

	//�Ƿ���ڴ��м���,�ļ�·��,��ʼλ��,��ֹλ��,�Ƿ�ѭ������
	TestSound = BASS_StreamCreateFile(FALSE, soundfile, 0, 0, 0); 
 
	BASS_ChannelPlay(TestSound,true);  
	while (BASS_ChannelIsActive(TestSound));
	
	BASS_Free();
	BASS_PluginFree(0);
	return 0;
}


