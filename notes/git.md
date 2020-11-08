# git操作相关

1. 更改.gitignore使其生效

   ```
   git rm -r --cached . // 删除本地缓存
   git add . // 添加要提交的文件
   ```

2. 放弃修改

   1. - 未添加至暂存区

        `git checkout .`

      - 已添加到暂存区

        `git reset HEAD .`

        `git checkout .`

        `git reset --hard`(回到之前的提交)

   2. 给某此commit打上tag

      `git tag <tag name> 16098ee1cbbc8a1884e19c6681735e1792f9b577 `

   3. 提交后当前分支莫名其妙的出现在head，如何merge回原来的分支

   ![](C:\Users\charlesliu\Desktop\企业微信截图_20200513141454.png)

   完整命令为：

   ```
   git branch temp  #新建临时分支，这是commit的内容在临时分支中存在
   git checkout master   #回到所需分支
   git merge temp        #merge到主分支
   git branch -d "temp"  #删除临时分支
   ```

3. 将打的tag推到远端

   ```
   git push origin tagName   # 提交指定 tag
   git push origin --tags  #提交所有未提交的 tag
   ```

   

4. todo