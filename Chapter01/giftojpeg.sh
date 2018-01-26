#! /bin/sh

# unfortunately, my system doesn't have program named cjpeg
# 为每个GIF文件创建一个对应的JPEG文件，通过cjpeg脚本和参数扩展来实现
for image in *.gif
do
	cjpeg $image > ${image%%gif}jpg  # 参数扩展：从尾部开始删除与gif匹配最长的部分，即将文件名中和gif有关的部分删除改成jpg
done

exit 0
