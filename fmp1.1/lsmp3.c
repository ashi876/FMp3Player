/*************************************************************************
	> 文件名: lsm
	> 作者：千城真人
	> 简介: fmp的伴侣.当前目录下的mp3顺序排列并编号的小工具
	> 编译命令:gcc -Wall -o3 -s lsm.c -o lsm
*************************************************************************/#include<stdio.h>
#include<windows.h>
 
int main(int argc,char *argv[])
{	
	int i;
	char ext[50]="*.mp3";//char ext[50]="*";
	
	/*if(argc==1) {
		printf("command type:%s .ext",argv[0]);
		return 0;
	}*/
	
	//else{
		//strcat(ext,argv[1]);//以上注释掉的语句用于指定搜索到的文件名
		//printf("argv[1]=%s\next=%s\n",argv[1],ext);//验证想法
		WIN32_FIND_DATA p;
		HANDLE h=FindFirstFile(ext,&p);
		printf("【%d】\t%s\n",1,p.cFileName);
		i=2;
		while(FindNextFile(h,&p)){
		printf("【%d】\t%s\n",i,p.cFileName);
		i++;
		}
		return 0;
	//}
	
    return 0;
}


//>	lpFileNameString，欲搜索的文件名。参数可如下
//>	c:\Windows\*.*//在c:\Windows目录中查找所有文件
//>	c:\Windows\System32\*.dll//在c:\Windows\System32目录中查找所有dll文件
//>	c:\Windows\System.ini；在c:\Windows目录中查找System.ini文件
//>	c:\Windows\a???.*//在c:\Windows目录中查找所有以a开头的文件名长度.为4个字符的文件
//>	Test.dat//在当前目录查找Test.dat文件
//>	*.*//在当前目录查找所有文件
