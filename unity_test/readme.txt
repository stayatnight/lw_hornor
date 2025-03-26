//前提在根目录下
1 新建build 目录
meson setup build
2 编译
meson compile -C build 
1 清理
meson compile -C build --clean

//单测简单示例运行的条件有
1 安装ninja 
2 安转meson
3 安装ruby


//一些单测强烈依赖硬件环境怎么办？
1 桩测试
2 模拟测试
3 