��    3      �  G   L      h  4   i  F   �  #   �     	     "     7  1   L  �   ~     0     >     U     o  *   �  �  �  +   �
     �
  #   �
  0         @      a  )   �  '   �     �     �  %        *     <  &   N  .   u     �     �     �  )   �  !     8   5  $   n     �     �  '   �     �  "     .   %     T  $   p  "   �     �  "   �  !   �          *  �  H       "   ;     ^     o     }     �  &   �  �   �     t     �  &   �      �  &   �  �    #   �       #   &  8   J     �     �  0   �  !   �            !   3     U     g  !   y  *   �     �     �     �  )   �     )  8   >      w     �     �  )   �     �       *   '  $   R     w  $   �     �     �     �               	   1   !       *       "            $                0                    +                 )   (   '       2             
   &                       3   .                                ,           -                 /   %                         #    	%'ju byte in file names
 	%'ju bytes in file names
 	%'ju byte used to store database
 	%'ju bytes used to store database
 	%'ju directory
 	%'ju directories
 	%'ju file
 	%'ju files
 
Report bugs to %s.
 --%s specified twice --%s would override earlier command-line argument Copyright (C) 2007 Red Hat, Inc. All rights reserved.
This software is distributed under the GPL v.2.

This program is provided with NO WARRANTY, to the extent permitted by law. Database %s:
 I/O error reading `%s' I/O error seeking in `%s' I/O error while writing to `%s' I/O error while writing to standard output Usage: updatedb [OPTION]...
Update a mlocate database.

  -f, --add-prunefs FS           omit also FS
  -n, --add-prunenames NAMES     omit also NAMES
  -e, --add-prunepaths PATHS     omit also PATHS
  -U, --database-root PATH       the subtree to store in database (default "/")
  -h, --help                     print this help
  -o, --output FILE              database to update (default
                                 `%s')
      --prune-bind-mounts FLAG   omit bind mounts (default "no")
      --prunefs FS               filesystems to omit from database
      --prunenames NAMES         directory names to omit from database
      --prunepaths PATHS         paths to omit from database
  -l, --require-visibility FLAG  check visibility before reporting files
                                 (default "yes")
  -v, --verbose                  print paths of files as they are found
  -V, --version                  print version information

The configuration defaults to values read from
`%s'.
 `%s' does not seem to be a mlocate database `%s' has unknown version %u `%s' has unknown visibility flag %u `%s' is locked (probably by an earlier updatedb) `=' expected after variable name can not change directory to `%s' can not change group of file `%s' to `%s' can not change permissions of file `%s' can not drop privileges can not find group `%s' can not get current working directory can not lock `%s' can not open `%s' can not open a temporary file for `%s' can not read two databases from standard input can not stat () `%s' configuration is too large error replacing `%s' file name length %zu in `%s' is too large file name length %zu is too large file system error: zero-length file name in directory %s invalid empty directory name in `%s' invalid regexp `%s': %s invalid value `%s' of --%s invalid value `%s' of PRUNE_BIND_MOUNTS missing closing `"' no pattern to search for specified non-option arguments are not allowed with --%s unexpected EOF reading `%s' unexpected data after variable value unexpected operand on command line unknown variable `%s' value in quotes expected after `=' variable `%s' was already defined variable name expected warning: Line number overflow Project-Id-Version: mlocate
Report-Msgid-Bugs-To: https://fedorahosted.org/mlocate/
PO-Revision-Date: 2013-06-20 21:09+0000
Last-Translator: Christopher Meng <i@cicku.me>
Language-Team: Chinese (China) <trans-zh_cn@lists.fedoraproject.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=1; plural=0;
X-Launchpad-Export-Date: 2018-07-12 14:13+0000
X-Generator: Launchpad (build 18719)
Language: zh_CN
 	%'ju 文件名中的字节数
 	%'ju 字节用于存储数据库
 	%'ju 文件夹
 	%'ju 文件
 
将 bug 报告给 %s.
 --%s 指定两次 --%s 将覆盖先前的命令行参数 Copyright (C) 2007 Red Hat, Inc. 保留所有权利。
本软件在 GPL v.2 下发布。

本程序不提供任何担保，在法律允许的范围内予以排除。 数据库 %s:
 I/O 读取错误 `%s' 在 `%s' 中搜索时发生 I/O 错误 写入 '%s' 时发生 I/O 错误 写入标准输出时发生 I/O 错误 用法: updatedb [OPTION]...
更新 mlocate 数据库。

  -f, --add-prunefs FS           也忽略 FS
  -n, --add-prunenames NAMES     也忽略 NAMES
  -e, --add-prunepaths PATHS     也忽略 PATHS
  -U, --database-root PATH       向数据库中存储的子树 (默认 "/")
  -h, --help                     显示这份帮助
  -o, --output FILE              要升级的数据库 (默认
                                 `%s')
      --prune-bind-mounts FLAG   忽略绑定装载 (默认 "no")
      --prunefs FS               从数据库中忽略的文件系统
      --prunenames NAMES         从数据库中忽略的目录名
      --prunepaths PATHS         从数据库中忽略的路径
  -l, --require-visibility FLAG  在报告文件之前检查可见性
                                 (默认 "yes")
  -v, --verbose                  当文件被找到时显示其路径
  -V, --version                  显示版本信息
配置默认为从文件`%s' 读取的值。
.
 `%s' 不是一个 mlocate 数据库 `%s' 具有未知的版本 %u `%s' 有未知的可视性标志 %u `%s' 被锁定 (可能是由于先前的数据库更新) 变量名后需要一个 `=' 无法改变目录 `%s' 无法将文件所在的组 `%s'  更改为 `%s' 无法改变文件 `%s' 的权限 无法释放特权 无法找到组 `%s' 无法得到当前的工作目录 无法锁定 `%s' 无法打开 `%s' 无法为 `%s' 打开临时文件 无法从标准输入读取两个数据库 无法执行 stat () `%s' 配置过大 替换错误 `%s' 文件名的长度 %zu 在`%s' 中 过大 文件名 %zu 过长 文件系统错误:在目录 %s 下文件名长度为零 `%s' 中有无效的空目录名 无效的 regexp `%s': %s %s 有无效的值 `%s' PRUNE_BIND_MOUNTS 后有无效的值 `%s' 缺少匹配的 '"' 没有指定任何搜索模式 非选项参数不允许 --%s 一起使用 读取 `%s' 时, 出现意外的 EOF 变量值后有意外的数据 命令行包含意外的操作对象 未知的变量 `%s' `=' 后需要引号内的值 变量 `%s' 已被定义 需要一个变量名 警告:行数溢出 