/*************************************************************************
	> �ļ���: FMp3Play v2.0
	> ���ߣ�ǧ������
	> ���: win�µĵ���SDL2_mixer��mpg123�ⲥ��mp3,��������ִ��.
	> ���ܣ����Ե��κ�ѭ������mp3
	> ����ʱ�䣺20171012
	
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
	gcc fmp2.c -Os -s -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lwinmm -mwindows -o fmp2.exe
	gcc fmp2.c -Wall -IC:\MinGW\include\SDL2 -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lwinmm  -o fmp2.exe	
	ע������4Ϊ���ŵ�ǰĿ¼�²������ļ��е�����mp3	
	ע������windows�ļ���ȫ��ʹ�ø�����
*************************************************************************/
#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>

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
		return 0;
	}
	
	return 0;
}

/***************  ����Ϊ�Ӻ���ʵ�־��幦�ܲ���  *************************/

/************************************************************************/
//����[fmp]�������ں���ʵ��
int fmp_help()
{	
	sprintf(msg,"Fmediaplay-%I64dλ��v2.0\n�����ʽ:\n������fmp xxx.mp3\n����ѭ����fmp r xxx.mp3\nָ����ŵ����һ��:fmp p number\nָ�����ѭ������:fmp pr number\n��ʾ��ǰĿ¼�����б�fmp ls\n����ȫ����fmp l\n�ر����fmp s", sizeof(int*)*8);
	MessageBox(NULL,msg,"FMediaPlay ǧ������",MB_OK);
	return 1;
}
/************************************************************************/
//����[fmp s]����ʵ��
int fmp_stop()
{
	printf("ִ�йر����%s %s\n",__argv[0],__argv[1]);	
	char cmd[100];//����cmd���ַ���������

	Mix_CloseAudio();
	SDL_Quit();
	sleep(1);
	sprintf(cmd, "taskkill /f /t /im %s.exe",__argv[0]);
	WinExec(cmd,SW_HIDE);
	return 0;
}

/************************************************************************/
//����[fmp xxx.mp3]����ʵ��
int fmp_singleplay()
{
	printf("���β���");	
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
//�����Ӻ�������SDL2_mixer��mpg123��Դ��Ƶ��
int fmp_play(char* mpfile)
{
	// ��ʼ��.
	SDL_Init(SDL_INIT_AUDIO);
	
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 )	;	

	Mix_Music *music = Mix_LoadMUS(localeToUTF8(mpfile));//gbkתutf8��SDL2ʶ��
	Mix_PlayMusic(music,1);//ֵΪ-1ѭ����1Ϊ����
		
	while (Mix_PlayingMusic());	//ֻҪ������û��ȡ��Ͳ�ֹͣ,���while��ˮƽ����ѧ����

	Mix_FreeMusic(music);
	Mix_CloseAudio();
	SDL_Quit();	
	return 0;
}

/************************************************************************/
//char�ַ�����gbkתutf8
char *localeToUTF8(char *src)
{
    static char *buf = NULL;
    if(buf){
        free(buf);
        buf = NULL;
    }
    wchar_t *unicode_buf;
    int nRetLen = MultiByteToWideChar(CP_ACP,0,src,-1,NULL,0);
    unicode_buf = (wchar_t*)malloc((nRetLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP,0,src,-1,unicode_buf,nRetLen);
    nRetLen = WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,NULL,0,NULL,NULL);
    buf = (char*)malloc(nRetLen+1);
    WideCharToMultiByte(CP_UTF8,0,unicode_buf,-1,buf,nRetLen,NULL,NULL);
    free(unicode_buf);
    return buf;
}