# C++/C常用函数

1. 循环遍历读取指定路径文件名


   ```C++
   #include <cstdlib>
   #include <io.h>
   
   void getFiles(std::string path, std::vector<std::string>& files)
   {
   	//文件句柄  
   	long long   hFile = 0;
   	//文件信息，声明一个存储文件信息的结构体  
   	struct _finddata_t fileinfo;
   	std::string p;//字符串，存放路径
   	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
   	{
   		do
   		{
   			//如果是目录,迭代之（即文件夹内还有文件夹）  
   			if ((fileinfo.attrib &  _A_SUBDIR))
   			{
   				//文件名不等于"."&&文件名不等于".."
   				//.表示当前目录
   				//..表示当前目录的父目录
   				//判断时，两者都要忽略，不然就无限递归跳不出去了！
   				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
   					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
   			}
   			//如果不是,加入列表  
   			else
   			{
                   // = filename.substr(filename.find_last_of('.') + 1);//获取文件后缀
   				std::string str_name(fileinfo.name);
                 //获取文件后缀
   				std::string suffixStr = str_name.substr(str_name.find_last_of('.') + 1);
   				if (suffixStr.compare("jpg") == 0 || suffixStr.compare("png") == 0){
   					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
   				}
   			}
   		} while (_findnext(hFile, &fileinfo) == 0);
   		//_findclose函数结束查找
   		_findclose(hFile);
   	}
   }
   ```

2. 讲读取的字符串根据标识符切割

   > ````c++
   > void split(string s, char splitchar, vector<string>& vec)
   > {
   > 	if (vec.size() > 0)//保证vec是空的
   > 		vec.clear();
   > 	int length = s.length();
   > 	int start = 0;
   > 	string topush;
   > 	for (int i = 0; i < length; i++)
   > 	{
   > 		if (s[i] == splitchar && i == 0)//第一个就遇到分割符
   > 		{
   > 			start += 1;
   > 		}
   > 		else if (s[i] == splitchar)
   > 		{
   > 			topush = s.substr(start, i - start);
   > 			if (topush.length() > 0)
   > 				vec.push_back(topush);
   > 			start = i + 1;
   > 		}
   > 		else if (i == length - 1)//到达尾部
   > 		{
   > 			topush = s.substr(start, i + 1 - start);
   > 			if (topush.length() > 0)
   > 				vec.push_back(topush);
   > 		}
   > 	}
   > }  
   > ````

3. 12