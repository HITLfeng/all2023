#！/bin/bash
source /opt/rh/devtoolset-8/enable

case "$1" in
    clean)
        # 清空目录
        echo "clearing..."
        # rm main 2>/dev/null
        rm -rf ./output/bin
        rm -rf ./output/lib
        echo "clearing done!"
        ;;
    *)
        # 执行其他操作
        source ./innerbuild.sh
        ;;
esac

# cp ./utils/include/* ./output/include/testinclude/

