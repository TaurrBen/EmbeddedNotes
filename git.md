# git 使用说明

## 一、概念

工作区、暂存区、本地仓库、远程仓库、远程分支、当前本地分支

## 二、常用命令

### 配置

```sh
全局配置
git config --global user.name '你的名字'
git config --global user.email '你的邮箱'
git config --global --list
git config --unset --global 要删除的配置项
当前仓库配置
git config --local user.name '你的名字'
git config --local user.email '你的邮箱'
git config --local --list
git config --unset --local 要删除的配置项
```

### 1.新项目初始化及第一次提交

```sh
git init
git add .
git commit -m "first commit"
git remote add origin https://github.com/liyinchigithub/Git-commands.git
git push origin master
```

### 2.克隆文件

```sh
git clone [远程仓库地址]
```

### 3.拉取文件

```sh
git pull origin(或远程仓库地址)
git pull origin [master]

git fetch 拉取但不合并到本地
git fetch --all
git fetch --tags
git reset --hard origin/master
git pull 与git fetch区别：

远端跟踪分支不同
git fetch能够直接更改远端跟踪分支。
git pull无法直接对远程跟踪分支操作，我们必须先切回本地分支然后创建一个新的commit提交。
拉取不同
git fetch会将数据拉取到本地仓库，它并不会自动合并或修改当前的工作。
git pull是从远程获取最新版本并merge到本地，会自动合并或修改当前的工作。
commitID不同
使用git fetch更新代码，本地的库中master的commitID不变，还是等于1。
使用git pull更新代码，本地的库中master的commitID发生改变，变成了2。
```

### 4.添加/删除文件到暂存区

```sh
git add [dir]
git rm [dir]
dir为指定目录、包含子目录
dir为.表示所有文件
```
### 5.提交暂存区改动到本地仓库

```sh
git commit -m ["first commit"]
```

### 6.分支

```sh
添加分支
git branch [新建的分支名]
列出所有分支
git branch
列出所有分支，包括远程分支
git branch -a

删除分支需要先切换到其他分支
切换分支
git checkout [分支名]
删除本地分支
git branch -d [本地分支名]
删除远程分支
git push origin --delete [远程分支名称]
```

### 7.将本地仓库推送到远程仓库

```sh
将本地的master分支推送到origin主机的master分支
git push origin [master]
强行推送
git push --force origin [master]
指定分支
git push [远程主机名] [本地分支名]:[远程分支名]

远程项目仓库对提交上来的分支代码还有做merge操作，才可以把分支代码合并到master中
将分支A合并到当前分支中并commit
git merge [分支]
将分支A合并入分支B并commit
git merge [分支A] [分支B]
```