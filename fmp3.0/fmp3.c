/*************************************************************************
	> 文件名: FMediaPlay v3.0
	> 作者：千城真人
	> 简介: win下的调用bass音频库,命令行下执行.
	> 功能：可以单次和循环播放mp3
	> 初版时间：20180121
	
	> 使用方法：
		1.将快捷方式放在右键<发送到>菜单，直接把mp3发送到本工具上
		2.单曲：fmp xxx.mp3
		3.单曲循环：fmp r xxx.mp3
		4.指定序号到最后一首:fmp p number
		5.指定序号单曲循环:fmp pr number
		6.显示当前目录歌曲列表：fmp ls
		7.播放全曲：fmp l
		8.关闭命令：fmp s
		
	>使用mgw_w64编译,各版mingw在win下编译百分之99有效，参考命令如下:
	gcc -Wall -os -s -mwindows fmp3.c -lwinmm -lbass -o fmp3.exe	
	注：命令4为播放当前目录下不含子文件夹的所有mp3	
	注：放入windows文件夹全局使用更方便
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

	//命令[fmp]帮助窗口
	if(__argc<2) {
		fmp_help();
	}
	
	else if(__argc==2){
		//命令[fmp s]关设备杀自身进程
		if(strcmp(__argv[1],"s")==0){
			fmp_stop();
			return 0;
		}
		//命令[fmp l]当前目录全曲播放，30首上限
		else if(strcmp(__argv[1],"l")==0){
			fmp_list();
			return 0;
		}
		//命令[fmp ls]当前目录mp3编号例表
		else if(strcmp(__argv[1],"ls")==0){
			fmp_lsmp3();
			return 0;
		}
		//命令[fmp xxx.mp3]单次播放 
		fmp_singleplay();
		play_Dir(__argv[1]);
		return 0;
	}


	else if(__argc==3) {  
		//命令[fmp r xxx.mp3]单曲循环
		if(strcmp(__argv[1],"r")==0){
			fmp_singlerepeat();
			return 0;
		}
		//命令[fmp p number]指定序号播放到末曲
		else if(strcmp(__argv[1],"p")==0){
			fmp_point();
			return 0;
		}
		//命令[fmp pr number]指定序号单曲循环
		else if(strcmp(__argv[1],"pr")==0){
			fmp_pointrepeat();
			return 0;
		}
		
		//鼠标右键框选播放
		while(--__argc)fmp_play(*++__argv);
		return 0;
	}
	
	//鼠标右键框选播放
	while(--__argc)fmp_play(*++__argv);
	
	return 0;
}

/***************  以下为子函数实现具体功能部分  *************************/

/************************************************************************/
//命令[fmp]帮助窗口函数实现
int fmp_help()
{	
	sprintf(msg,"Fmediaplay-%I64d位版v3.0\n命令格式:\n单曲：fmp xxx.mp3\n单曲循环：fmp r xxx.mp3\n指定序号到最后一首:fmp p number\n指定序号循环播放:fmp pr number\n显示当前目录歌曲列表：fmp ls\n播放全曲：fmp l\n关闭命令：fmp s", sizeof(int*)*8);
	MessageBox(NULL,msg,"FMediaPlay 千城真人",MB_OK);
	return 1;
}
/************************************************************************/
//命令[fmp s]函数实现
int fmp_stop()
{
	printf("执行关闭命令：%s %s\n",__argv[0],__argv[1]);	
	char cmd[100];//容纳cmd的字符串变量。

	BASS_Free();
	BASS_PluginFree(0);
	sleep(1);
	sprintf(cmd, "taskkill /f /t /im %s.exe",__argv[0]);
	WinExec(cmd,SW_HIDE);
	return 0;
}

/************************************************************************/
//命令[fmp xxx.mp3]函数实现
int fmp_singleplay()
{
	//printf("单次播放%s",__argv[1]);	
	fmp_play(__argv[1]);
	return 0;
}

/************************************************************************/
//命令[fmp l]函数实现		
int fmp_list()
{	

	int i,j;
	char ext[50]="*.mp3";//存放后缀名
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//当前目录搜索mp3文件

	strcpy(listname[1],p.cFileName);//第一次写入数组
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]赋值过了，listname[0]就让它空着吧
	while(FindNextFile(h,&p)){
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30为显示和写入listname[]的mp3歌曲数量
	}
	j=i;
	for(i=1;i<j;i++){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//命令[fmp r xxx.mp3]函数实现
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
//命令[fmp p number]函数实现	
int fmp_point()
{	

	int i,j;
	char ext[50]="*.mp3";//存放后缀名

    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//当前目录搜索mp3文件
    //puts(p.cFileName);//去掉注释，帮你理解此时输出了什么
	strcpy(listname[1],p.cFileName);//第一次写入数组
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]赋值过了，listname[0]就让它空着吧
	while(FindNextFile(h,&p)){
		//puts(p.cFileName);//去掉注释，帮你理解此时输出了什么
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30为显示和写入listname[]的mp3歌曲数量
	}
	
	
	j=i;
	i=atoi(__argv[2]);
	sprintf(msg,"点击确定将从当前歌曲播放到最后一首:\n\n当前歌曲%d %s\n...\n最后一首%d %s",i,listname[i],j-1,listname[j-1]);
	//MessageBox(NULL,msg,"fmp",MB_OKCANCEL);
	if(MessageBox(NULL,msg,"fmp",MB_OKCANCEL)==IDCANCEL)  
	return 0; 
	printf("播放的歌曲序号: %d\n", i);

	for(;i<j;i++){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//命令[fmp pr number]函数实现指定序号单曲循环	
int fmp_pointrepeat()
{	

	int i,j;
	char ext[50]="*.mp3";//存放后缀名
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext,&p);//当前目录搜索mp3文件
	strcpy(listname[1],p.cFileName);//第一次写入数组
	printf("%d %s\n",1,listname[1]);
	
	i=2;//listname[1]赋值过了，listname[0]就让它空着吧
	while(FindNextFile(h,&p)){
		strcpy(listname[i],p.cFileName);
		printf("%d %s\n",i,listname[i]);
		i++;
		if(i>30){
			printf("warning! show 30 songs best!");
			break;
		}// i>30为显示和写入listname[]的mp3歌曲数量
	}
	
	
	j=i;
	i=atoi(__argv[2]);
	sprintf(msg,"点击确定循环播放[当前歌曲］:\n\n当前歌曲%d %s",i,listname[i]);
	//函数原形MessageBox(NULL,msg,"fmp",MB_OKCANCEL);
	if(MessageBox(NULL,msg,"fmp",MB_OKCANCEL)==IDCANCEL)  
	return 0; 
	printf("播放的歌曲序号: %d\n", i);

	for(;i<j;){
		fmp_play(listname[i]);
	}
    return 0;
}

/************************************************************************/
//命令[fmp ls]当前目录mp3编号列表
int fmp_lsmp3(int argc,char *argv[])
{	
	int i;
	char ext[50]="*.mp3";
	
	AllocConsole();//这两条命令建立新控制台．在-mwindws参数下printf的方法之一
	freopen("CONOUT$", "w", stdout);
	
		WIN32_FIND_DATA p;
		HANDLE h=FindFirstFile(ext,&p);
		printf("【%d】\t%s\n",1,p.cFileName);
		i=2;
		while(FindNextFile(h,&p)){
		printf("【%d】\t%s\n",i,p.cFileName);
		i++;
		}
		getch();//system("pause")	
    return 0;
}

/************************************************************************/
//播放目录内所有支持的格式
int play_Dir(char* p)
{
        DIR *dir = NULL; // 目录结构
        struct dirent *ent = NULL; // 目录下的文件名或者目录的结构
        
		dir = opendir(p);  // 读取目录路径
        while (NULL != (ent = readdir(dir))) { // 
			char temppath[500];
			sprintf(temppath,"%s\\%s",p,ent->d_name);
			printf("%s\n",temppath);
			fmp_play(temppath);
		}
	return 0;
}


/************************************************************************/
//播放子函数调用bass音频库
int plugin()
{
	//读取插件;
	DIR *dir = NULL; // 目录结构
	struct dirent *ent = NULL; // 目录下的文件名或者目录的结构	
	char temppath[260],pluginpath[260];//自身路径名和插件目录变量
		
	GetModuleFileName(NULL,temppath,260);//获取自身绝对路径，去掉文件名
	char *ext=strrchr(temppath,'\\');
	if (ext)*ext='\0';    

    sprintf(pluginpath,"%s\\bassplugin\\",temppath);//写入插件路径
	
	dir = opendir(pluginpath);  //打开插件目录并读取      
	while (NULL != (ent = readdir(dir))) {
		if (!strcmp(ent->d_name,".")||!strcmp(ent->d_name,".."))continue;
		sprintf(temppath,"%s%s",pluginpath,ent->d_name);//写入插件路径
		BASS_PluginLoad(temppath,0);
	}
	closedir(dir);

	return 0;
}
		
int fmp_play(char *soundfile)
{
	int TestSound;
	//初始化(启用设备-1表示默认,采样率,效果参数,窗口句柄,0表示使用默认数据初始化DirectSound)
	BASS_Init(-1, 44100, 0, 0, 0);
	plugin();

	//是否从内存中加载,文件路径,起始位置,终止位置,是否循环播放
	TestSound = BASS_StreamCreateFile(FALSE, soundfile, 0, 0, 0); 
 
	BASS_ChannelPlay(TestSound,true);  
	while (BASS_ChannelIsActive(TestSound));
	
	BASS_Free();
	BASS_PluginFree(0);
	return 0;
}


