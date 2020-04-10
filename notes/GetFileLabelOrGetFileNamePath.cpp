// GetFileLabel.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <io.h>
#include <stdio.h>
using namespace std;

void getFiles(string path, vector<string>& files)
{
     //�ļ����  
    long   hFile = 0;
   //�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
    struct _finddata_t fileinfo;
    string p;//�ַ��������·��
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
    {
        do
         {
            //�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
             if ((fileinfo.attrib &  _A_SUBDIR))
             {
                 //�ļ���������"."&&�ļ���������".."
                 //.��ʾ��ǰĿ¼
                 //..��ʾ��ǰĿ¼�ĸ�Ŀ¼
                 //�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
                 if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                     getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
             }
             //�������,�����б�  
             else
             {
				 string str_name(fileinfo.name);// = filename.substr(filename.find_last_of('.') + 1);//��ȡ�ļ���׺
				 string suffixStr = str_name.substr(str_name.find_last_of('.') + 1);//��ȡ�ļ���׺


				 if (suffixStr.compare("jpg")==0 || suffixStr.compare("png") == 0)
				 {
					 files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				 }
                 
             }
        } while (_findnext(hFile, &fileinfo) == 0);
        //_findclose������������
       _findclose(hFile);
     }
}
int getClassName(string src_path, vector<string> &results)
{
	int nums = 0;
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
	struct _finddata_t fileinfo;
	string p;//�ַ��������·��
	//
	if ((hFile = _findfirst(p.assign(src_path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
	{
		do
		{
			//�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//�ļ���������"."&&�ļ���������".."
				//.��ʾ��ǰĿ¼
				//..��ʾ��ǰĿ¼�ĸ�Ŀ¼
				//�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					results.push_back(fileinfo.name);
					nums++;
				}
				//getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose������������
		_findclose(hFile);

	}
	return nums;

}

int main()
{
	string src_path = "E:\\Dataset\\top secret data\\snake_gz_2";
	char dst_path[] = "labels.txt";
	string str_temp;
	vector<string> class_folder_names;
	vector<string> class_names;
	int class_num = getClassName(src_path, class_folder_names);
	if (class_num == 0) printf("No class num\n"); 
	FILE* pFile = fopen(dst_path, "w");
	
	for (int i = 0; i < class_num; i++)
	{
		vector<string> paths;
		int first_pos = class_folder_names[i].find_first_of("_");
		int sec_pos = class_folder_names[i].find_last_of("_");
		string class_name = class_folder_names[i].substr(first_pos + 1, sec_pos-3);
		getFiles(src_path + "\\" + class_folder_names[i], paths);
		for (int j = 0; j < paths.size(); j++)
		{
			fprintf(pFile, "%s	%s\n", paths[j].c_str(), class_name.c_str());
		}
	}
	system("pause");
    return 0;
}

