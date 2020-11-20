# 8ms-sstar
http://hmi.8ms.xyz

# 安装依赖
已验证系统为：ubuntu 16.04.7 64位系统

````sh
sudo apt-get install subversion build-essential libncurses5-dev zlib1g-dev gawk git ccache \
		gettext libssl-dev xsltproc libxml-parser-perl \
		gengetopt default-jre-headless ocaml-nox sharutils textinfo
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install zlib1g:i386 libstdc++6:i386 libc6:i386 libc6-dev-i386
````

# 安装toolchian

1. 解压缩toolchain

```
sudo tar toolchain/wt-gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf.tag.gz -xvf -C /opt/
```

2. 设置环境变量，修改 ~/.profile文件, 将下面这行添加到文件末尾

```
PATH="/opt/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin:$PATH"
```

手动生效环境变量
```
source ~/.profile
```

测试交叉工具链
```
arm-linux-gnueabihf-gcc --version
```

# 编译
make

库文件位于lib/liblvgl.so
可执行文件位于bin/demo

# 测试
将demo通过网口上传到板子（tftp或者scp），增加可执行权限，并运行。例如

```
cd /tmp
tftp -g 192.168.1.111 -r demo
chmod +x demo
./demo
```

如果板子没有liblvgl.so，可先手动上传。
```
cd /usr/lib/
tftp -g 192.168.1.111 -r liblvgl.so
```
