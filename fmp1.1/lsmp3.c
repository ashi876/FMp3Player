/*************************************************************************
	> �ļ���: lsm
	> ���ߣ�ǧ������
	> ���: fmp�İ���.��ǰĿ¼�µ�mp3˳�����в���ŵ�С����
	> ��������:gcc -Wall -o3 -s lsm.c -o lsm
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
		//strcat(ext,argv[1]);//����ע�͵����������ָ�����������ļ���
		//printf("argv[1]=%s\next=%s\n",argv[1],ext);//��֤�뷨
		WIN32_FIND_DATA p;
		HANDLE h=FindFirstFile(ext,&p);
		printf("��%d��\t%s\n",1,p.cFileName);
		i=2;
		while(FindNextFile(h,&p)){
		printf("��%d��\t%s\n",i,p.cFileName);
		i++;
		}
		return 0;
	//}
	
    return 0;
}


//>	lpFileNameString�����������ļ���������������
//>	c:\Windows\*.*//��c:\WindowsĿ¼�в��������ļ�
//>	c:\Windows\System32\*.dll//��c:\Windows\System32Ŀ¼�в�������dll�ļ�
//>	c:\Windows\System.ini����c:\WindowsĿ¼�в���System.ini�ļ�
//>	c:\Windows\a???.*//��c:\WindowsĿ¼�в���������a��ͷ���ļ�������.Ϊ4���ַ����ļ�
//>	Test.dat//�ڵ�ǰĿ¼����Test.dat�ļ�
//>	*.*//�ڵ�ǰĿ¼���������ļ�
