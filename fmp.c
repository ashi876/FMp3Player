/*************************************************************************
	> �ļ���: FMp3Play v1.1
	> ���ߣ�ǧ������
	> ���: win�µĵ���mci������mp3���Ź��ߣ���������������ִ��.
	> ���ܣ����Ե��κ�ѭ������mp3
	> ʱ�䣺20171012
	
	> ʹ�÷�����
		1.����ݷ�ʽ�����Ҽ�<���͵�>�˵���ֱ�Ӱ�mp3���͵���������
		2.������fmp xxx.mp3
		3.����ѭ����fmp r xxx.mp3
		4.ָ����ŵ����һ��:fmp p number
		5.ָ����ŵ���ѭ��:fmp pr number
		6.����ȫ����fmp l
		7.�ر����fmp s
		
	>ʹ��mgw_w64����,����mingw��win�±���ٷ�֮99��Ч���ο���������:
		gcc -o3 -s fmp.c -lwinmm -mwindows -o fmp.exe
		
	ע������4Ϊ���ŵ�ǰĿ¼�²������ļ��е�����mp3	
	ע������windows�ļ���ȫ��ʹ�ø�����
*************************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>

/************************************************************************/
char msg[256];
extern int __argc;
extern char ** __argv;
void sendCommand(char *);

char shortBuffer[MAX_PATH];
char cmdBuff[MAX_PATH + 64];

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
//�����ַ����� mci �����Կ��ܷ����Ĵ����źͷ��ͳ����ַ���
void sendCommand(char *s)
{
	int i;
	i=mciSendString(s,NULL,0,0);
	
	if(i){
		sprintf(msg,"Error %d when sending %s\n",i,s);
		MessageBox(NULL,msg,"fmp",MB_OK);
	}
}

/************************************************************************/
//����[fmp]�������ں���ʵ��
int fmp_help()
{
	//printf("��������%s %s %s %d ",__argv[0],__argv[1],__argv[2],__argc);		
	sprintf(msg,"Fmediaplay-%dλ��\n�����ʽ:\n������fmp xxx.mp3\n����ѭ����fmp r xxx.mp3\nָ����ŵ����һ��:fmp p number\nָ�����ѭ������:fmp pr number\n����ȫ����fmp l\n�ر����fmp s", sizeof(int*)*8);
	MessageBox(NULL,msg,"FMediaPlay ǧ������",MB_OK);
	//getch();
	return 1;
}

/************************************************************************/
//����[fmp s]����ʵ��
int fmp_stop()
{
	printf("ִ�йر����%s %s\n",__argv[0],__argv[1]);	
	char cmd[100];//����cmd���ַ���������
	sendCommand("Close All");
	sleep(1);
	sprintf(cmd, "taskkill /f /t /im %s.exe",__argv[0]);//д��cmd����
	//printf("%s",cmd);
	system(cmd);
	return 0;
}

/************************************************************************/
//����[fmp xxx.mp3]����ʵ��
int fmp_singleplay()
{  
	GetShortPathName(__argv[1],shortBuffer,sizeof(shortBuffer));
	if(!*shortBuffer) {
		sprintf(msg,"Cannot shorten filename \"%s\"\n",__argv[1]);
		MessageBox(NULL,msg,"Fmediaplay",MB_OK);
		return 1;
	}  
	sendCommand("Close All");
	sprintf(cmdBuff,"Open %s Type MPEGVideo Alias theMP3",shortBuffer);
	sendCommand(cmdBuff);
	sendCommand("Play theMP3 Wait");
	//printf("����\n %s %s %s %d",__argv[0],__argv[1],__argv[2],__argc);
	return 0;
}

/************************************************************************/
//����[fmp l]����ʵ��		
int fmp_list()
{	

	int i,j;
	char listname[31][100], *ext[1];//������ʾ��������鳤��,��һ����ź�׺����ָ������
	ext[1]="*.mp3";//��������һ��ָ����׺��
	//scanf("%s",&*ext[1]);
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext[1],&p);//����βע��,�������ڵ�ǰĿ¼����mp3�ļ�
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
	for(i=1;i<j;i++){
		sendCommand("Close All");
		sprintf(cmdBuff,"Open \"%s\" Type MPEGVideo Alias theMP3",listname[i]);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//��ͬ����������
	}
	//printf("��¼һ%s ��¼��%s\n",listname[1],listname[3]);//��֤���������õ�
	//system("pause");
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
		sendCommand("Close All");
		sprintf(cmdBuff,"Open %s Type MPEGVideo Alias theMP3",shortBuffer);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//��ͬ����������
		}
	//printf("����\n %s %s %s %d",__argv[0],__argv[1],__argv[2],__argc);
	return 0;
}

/************************************************************************/
//����[fmp p number]����ʵ��	
int fmp_point()
{	

	int i,j;
	char listname[31][100], *ext[1];//������ʾ��������鳤��,��һ����ź�׺����ָ������
	ext[1]="*.mp3";//��������һ��ָ����׺��
	//scanf("%s",&*ext[1]);
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext[1],&p);//����βע��,�������ڵ�ǰĿ¼����mp3�ļ�
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
		sendCommand("Close All");
		sprintf(cmdBuff,"Open \"%s\" Type MPEGVideo Alias theMP3",listname[i]);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//��ͬ����������
	}
	//printf("��¼һ%s ��¼��%s\n",listname[1],listname[3]);//��֤���������õ�
	//system("pause");
    return 0;
}

/************************************************************************/
//����[fmp pr number]����ʵ��ָ����ŵ���ѭ��	
int fmp_pointrepeat()
{	

	int i,j;
	char listname[31][100], *ext[1];//������ʾ��������鳤��,��һ����ź�׺����ָ������
	ext[1]="*.mp3";//��������һ��ָ����׺��
	//scanf("%s",&*ext[1]);
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext[1],&p);//����βע��,�������ڵ�ǰĿ¼����mp3�ļ�
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
	sprintf(msg,"���ȷ��ѭ������[��ǰ������:\n\n��ǰ����%d %s",i,listname[i]);
	//MessageBox(NULL,msg,"fmp",MB_OKCANCEL);
	if(MessageBox(NULL,msg,"fmp",MB_OKCANCEL)==IDCANCEL)  
	return 0; 
	printf("���ŵĸ������: %d\n", i);

	for(;i<j;){
		sendCommand("Close All");
		sprintf(cmdBuff,"Open \"%s\" Type MPEGVideo Alias theMP3",listname[i]);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//��ͬ����������
	}
	//printf("��¼һ%s ��¼��%s\n",listname[1],listname[3]);//��֤���������õ�
	//system("pause");
    return 0;
}