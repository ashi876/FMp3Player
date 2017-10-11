/*************************************************************************
	> 文件名: FMp3Play v1.1
	> 简介: win下的调用mci函数的mp3播放工具．可以在命令行下执行.
	> 功能：可以单次和循环播放mp3和wav.未来将加入指定次数播放功能．
			这些功能将用参数形式在命令行下实现．
	> 时间：20171011
	
	> 使用方法：
		1.将快捷方式放在右键<发送到>菜单，直接把mp3发送到本工具上
		2.命令行下：fmp xxx.mp3
		3.单曲循环：fmp r xxx.mp3
		4.播放全曲：fmp l
		5.关闭命令：fmp s
		
	>使用mgw_w64编译,各版mingw在win下编译百分之99有效，参考命令如下:
		生成发行版：gcc -o3 -s fmp.c -lwinmm -mwindows -o fmp.exe
		命令行测试版：gcc -Wall fmp.c -lwinmm -o fmp.exe
		
	注：命令4为播放当前目录下不含子文件夹的所有mp3	
	注：放入windows文件夹全局使用更方便
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
/************************************************************************/
	//帮助窗口
	if(__argc<2) 
	{
		int fmp_help();
		fmp_help();
	}
	
/************************************************************************/
	//命令fmp s关设备杀自身进程
	else if(__argc==2&&(strcmp(__argv[1],"s")==0))
	{
		int fmp_stop();
		fmp_stop();
	}
	
/************************************************************************/
	//命令fmp l播放当前目录mp3
	else if(__argc==2&&(strcmp(__argv[1],"l")==0))
	{
		int listfmp();
		listfmp();
		return 0;
	}

/************************************************************************/
	//命令fmp r单曲循环
	else if(__argc==3&&(strcmp(__argv[1],"r")==0)) 
	{  
		int fmp_singlerepeat();
		fmp_singlerepeat();
		return 0;
	}

/************************************************************************/
	//判断只有一个参数或文件名时单次播放
	else if(__argc==2) 
	{  
		int fmp_singleplay();
		fmp_singleplay();
		return 0;
	}	
	return 0;
}

/***************  以下为子函数实现具体功能部分  *************************/
//发送字符串到 mci 并回显可能发生的错误编号和发送出的字符串
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
//命令[fmp l]函数实现		
int listfmp()
{	
	char cmdBuff[MAX_PATH + 64];
	int i,j;
	char listname[31][100], *ext[1];//定义显示例表的数组长度,和一个存放后缀名的指针数组
	ext[1]="*.mp3";//可以用下一句指定后缀名
	//scanf("%s",&*ext[1]);
	
    WIN32_FIND_DATA p;
    HANDLE h=FindFirstFile(ext[1],&p);//见文尾注释,本例中在当前目录搜索mp3文件
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
	for(i=1;i<j;i++){
		sendCommand("Close All");
		sprintf(cmdBuff,"Open \"%s\" Type MPEGVideo Alias theMP3",listname[i]);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//等同于上条命令
	}
	//printf("记录一%s 记录二%s\n",listname[1],listname[3]);//验证数组内容用的
	//system("pause");
    return 0;
}

/************************************************************************/
//命令[fmp]帮助窗口函数实现
int fmp_help()
{
	//printf("帮助窗口%s %s %s %d ",__argv[0],__argv[1],__argv[2],__argc);		
	sprintf(msg,"Fmediaplay-%d位版\n命令格式:\n\t[fmp] [参数] [\"路径\\文件名.mp3\"]\n", sizeof(int*)*8);
	MessageBox(NULL,msg,"FMediaPlayV1.0　千城真人",MB_OK);
	//getch();
	return 1;
}

/************************************************************************/
//命令[fmp s]函数实现
int fmp_stop()
{
	printf("执行关闭命令：%s %s\n",__argv[0],__argv[1]);	
	char cmd[100];//容纳cmd的字符串变量。
	sendCommand("Close All");
	sleep(1);
	sprintf(cmd, "taskkill /f /t /im %s.exe",__argv[0]);//写入cmd命令
	//printf("%s",cmd);
	system(cmd);
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
		sendCommand("Close All");
		sprintf(cmdBuff,"Open %s Type MPEGVideo Alias theMP3",shortBuffer);
		sendCommand(cmdBuff);
		sendCommand("Play theMP3 Wait");
		//mciSendString("play theMP3 Wait",NULL,0,0);//等同于上条命令
		}
	//printf("单次\n %s %s %s %d",__argv[0],__argv[1],__argv[2],__argc);
	return 0;
}

/************************************************************************/
//命令[fmp xxx.mp3]函数实现
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
	//printf("单次\n %s %s %s %d",__argv[0],__argv[1],__argv[2],__argc);
	return 0;
}