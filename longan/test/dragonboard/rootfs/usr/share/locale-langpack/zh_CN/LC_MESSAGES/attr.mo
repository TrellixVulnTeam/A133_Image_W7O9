��            )         �     �  �  �  �  �  !      !   B  /   d  =   �  2   �  $        *  %   C  .   i  +   �  *   �  .   �     	     9	     U	     s	     �	  &   �	     �	     �	  @   �	  3   /
  R  c
     �     �     �       }        �    �     �     �       /   #  =   S  *   �  !   �     �     �  (         6     W  7   w     �     �     �     �       "   "     E     V  ?   e  4   �    �     �     	          4                                                                                     	                                      
                    %s %s
   -n, --name=name         get the named extended attribute value
  -d, --dump              get all extended attribute values
  -e, --encoding=...      encode values (as 'text', 'hex' or 'base64')
      --match=pattern     only get attributes with names matching pattern
      --only-values       print the bare values only
  -h, --no-dereference    do not dereference symbolic links
      --absolute-names    don't strip leading '/' in pathnames
  -R, --recursive         recurse into subdirectories
  -L, --logical           logical walk, follow symbolic links
  -P  --physical          physical walk, do not follow symbolic links
      --version           print version and exit
      --help              this help text
   -n, --name=name         set the value of the named extended attribute
  -x, --remove=name       remove the named extended attribute
  -v, --value=value       use value as the attribute value
  -h, --no-dereference    do not dereference symbolic links
      --restore=file      restore extended attributes
      --version           print version and exit
      --help              this help text
 %s %s -- get extended attributes
 %s %s -- set extended attributes
 %s: %s: No filename found in line %d, aborting
 %s: No filename found in line %d of standard input, aborting
 %s: Removing leading '/' from absolute path names
 %s: invalid regular expression "%s"
 -V only allowed with -s
 A filename to operate on is required
 At least one of -s, -g, -r, or -l is required
 Attribute "%s" had a %d byte value for %s:
 Attribute "%s" has a %d byte value for %s
 Attribute "%s" set to a %d byte value for %s:
 Could not get "%s" for %s
 Could not list "%s" for %s
 Could not remove "%s" for %s
 Could not set "%s" for %s
 No such attribute Only one of -s, -g, -r, or -l allowed
 Unrecognized option: %c
 Usage: %s %s
 Usage: %s %s
       %s %s
Try `%s --help' for more information.
 Usage: %s %s
Try `%s --help' for more information.
 Usage: %s [-LRSq] -s attrname [-V attrvalue] pathname  # set value
       %s [-LRSq] -g attrname pathname                 # get value
       %s [-LRSq] -r attrname pathname                 # remove attr
       %s [-LRq]  -l pathname                          # list attrs 
      -s reads a value from stdin and -g writes a value to stdout
 getting attribute %s of %s listing attributes of %s setting attribute %s for %s setting attributes for %s Project-Id-Version: attr
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2007-08-18 12:08+0000
Last-Translator: Tao Wei <weitao1979@gmail.com>
Language-Team: Chinese (China) <zh_CN@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 12:39+0000
X-Generator: Launchpad (build 18719)
        %s %s
   -n, --name=name 获取命名的扩展属性值
  -d, --dump 获取所有扩展属性值
  -d, --encoding=... 编码值(如'text', 'hex' 或'base64')
      --math=pattern 只获取与模板名匹配的属性
      --only-values 只打印相应值
  -h , --on-dereference 不解除软连接
      --absolute-names 不去除路径中的引导'/'
  -R, --recursive 包括所有子目录
  -L, --logical 沿着软连接逻辑漫步
  --P --physical 不沿着软件物理漫步
      --version 打印版本并退出
      --help 本帮助文本
   -n, --name=name 设置指定扩展属性的值
  -x, --remove=name 移除指定扩展属性
  -v, --value=value 用该值作为属性值
  -h, --no-dereference 不解除软连接
      --restore=file 恢复扩展属性
      --version 打印版本并退出
      --help 本帮助文本
 %s %s --获取扩展属性
 %s %s --设置扩展属性
 %s: %s:没有在%d 中找到文件名，中止
 %s: 在标准输入的第%d行没有发现文件名，中止
 %s：删去绝对路径名最前面的'/'
 %s:无效的正则表达式 "%s"
 -V 只能和 -s 一起使用
 需要文件名
 至少要 -s、-g、-r、-l中的一个
 属性"%s" 有 %d 字节到 %s:
 属性"%s"有 %d字节值于%s
 属性"%s" 设定到一个 %d 字节的数值对 %s：
 无法获取 "%s" 于 %s:
 无法列出 "%s"于%s
 无法移除 "%s" 于 %s:
 无法设定"%s"为 %s
 无此属性 只允许 -s,-g,-r,-l中的一个
 未知选项:%c
 用法: %s %s
 用法：%s %s
      %s %s
'%s --help以'获得更多信息。
 用法： %s %s
'%s --help'以获取更多信息。
 用法：%s [-LRSq] -s 属性名 [-V attrvalue] 路径名# 设定值
       %s [-LRSq] -g 属性名 路径名# 获取值
       %s [-LRSq] -r 属性名 路径名# 移除属性
       %s [-LRq] -l 路径名# 列出属性
      -s 从标准输入读取值 -g 写值到标准输出
 获取属性 %s 于 %s 列出 %s 的属性 设置属性 %s 于 %s 为 %s 设置属性 