# Java

## 常用函数

1. bitmap使用参考

[bitmap使用介绍](https://www.jianshu.com/p/a951ba9c85f5) 

2. bitmap做旋转镜像

   ```java
   Bitmap convert(Bitmap a, int width, int height) { 
   	int w = a.getWidth(); 
   	int h = a.getHeight(); 
   	Bitmap newb = Bitmap.createBitmap(ww, wh, Config.ARGB_8888);// 创建一个新的和SRC长度宽度一样的位图 
   	Canvas cv = new Canvas(newb); 
   	Matrix m = new Matrix(); 
   	m.postScale(1, -1);   //镜像垂直翻转 
   	m.postScale(-1, 1);   //镜像水平翻转 
   	m.postRotate(-90);  //旋转-90度 
   	Bitmap new2 = Bitmap.createBitmap(a, 0, 0, w, h, m, true); 
   	cv.drawBitmap(new2, new Rect(0, 0, new2.getWidth(), new2.getHeight()),new Rect(0, 0, ww, wh), null); 
   	return newb; 
   } 
   ```

   

3. 