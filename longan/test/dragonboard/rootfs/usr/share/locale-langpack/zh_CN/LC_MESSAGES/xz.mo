��   >   0      Y        t     |  �      �  -   �     �  7   �  H   5  >   ~  9   �  �   �  �   y  �   �     �     �     �  !   	  !   *	  '   L	     t	     �	  *   �	  /   �	  %   
  /   5
  ,   e
  4   �
     �
     �
     �
            6  h   W     �     �  2   �     !  $   >  /   c     �      �  O   �  .     /   G     w  A   �     �     �       (     I   F  !   �  9   �     �  0     <   8  /   u  D   �  %   �          (     6  A   J  Q   �  *   �  �  	  *   �     �  N   �  L   4  =   �  4   �  �   �  �     �   
     �     �     �       %   ,  "   R     u  "   �  .   �  /   �  6     ,   K  &   x  ,   �     �     �     �           5  n   V     �     �  2   �     *  %   G  #   m     �     �  $   �     �  $        *  <   @     }     �     �     �  I   �  !   &  6   H       -   �  <   �  %     H   )  *   r     �     �     �  <   �  T     2   k     
   -       .                    9      (               3      #   )            8       6                                 >                  $   ,      '      !      /          "             0   	   +   4             %              1   &                          7      <                 5      =      *         :   2   ;      �  �  �  �  0                 �����  1                 ���� 
 Basic file format and compression options:
 
 Other options:
 
With no FILE, or when FILE is -, read standard input.
       --robot         use machine-parsable messages (useful for scripts)   -Q, --no-warn       make warnings not affect the exit status   -V, --version       display the version number and exit   -h, --help          display the short help (lists only the basic options)
  -H, --long-help     display this long help and exit   -h, --help          display this short help and exit
  -H, --long-help     display the long help (lists also the advanced options)   -k, --keep          keep (don't delete) input files
  -f, --force         force overwrite of output file and (de)compress links
  -c, --stdout        write to standard output and don't delete input files  Operation mode:
 %s home page: <%s>
 %s: Cannot remove: %s %s: Cannot set the file group: %s %s: Cannot set the file owner: %s %s: Cannot set the file permissions: %s %s: Closing the file failed: %s %s: Error reading filenames: %s %s: File already has `%s' suffix, skipping %s: File has setuid or setgid bit set, skipping %s: File has sticky bit set, skipping %s: File seems to have been moved, not removing %s: Filename has an unknown suffix, skipping %s: Input file has more than one hard link, skipping %s: Invalid option name %s: Invalid option value %s: Is a directory, skipping %s: Is a symbolic link, skipping %s: Not a regular file, skipping %s: Null character found when reading filenames; maybe you meant to use `--files0' instead of `--files'? %s: Read error: %s %s: Unexpected end of file %s: Unexpected end of input when reading filenames %s: Unknown file format type %s: Unsupported integrity check type %s: Value is not a non-negative decimal integer %s: Write error: %s Cannot establish signal handlers Cannot read data from standard input when reading filenames from standard input Compressed data cannot be read from a terminal Compressed data cannot be written to a terminal Compressed data is corrupt Compression and decompression with --robot are not supported yet. Empty filename, skipping File format not recognized Internal error (bug) LZMA1 cannot be used with the .xz format Mandatory arguments to long options are mandatory for short options too.
 Maximum number of filters is four Memory usage limit is too low for the given filter setup. Memory usage limit reached No integrity check; not verifying file integrity Only one file can be specified with `--files' or `--files0'. The .lzma format supports only the LZMA1 filter The exact options of the presets may vary between software versions. Try `%s --help' for more information. Unexpected end of input Unknown error Unsupported options Unsupported type of integrity check; not verifying file integrity Usage: %s [OPTION]... [FILE]...
Compress or decompress FILEs in the .xz format.

 Using a preset in raw mode is discouraged. Project-Id-Version: xz-utils
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2009-10-26 12:49+0000
Last-Translator: Chen Ming <Unknown>
Language-Team: Simplified Chinese <zh_CN@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=1; plural=0;
X-Launchpad-Export-Date: 2018-07-12 14:24+0000
X-Generator: Launchpad (build 18719)
 
 基本的文件格式和压缩选项：
 
 其它选项：
 
如果不给出 FILE，或者 FILE 是 “-”，就从标准输入读取。
       --robot         使用易于机器解析的消息 (方便脚本使用)   -Q, --no-warn       使警告不影响程序的退出状态   -V, --version       显示版本号，然后退出   -h, --help          显示较短的帮助 (只列出基本选项)
  -H, --long-help     显示现在这个较长的帮助，然后退出   -h, --help          显示现在这个较短的帮助，然后退出
  -H, --long-help     显示较长的帮助 (也列出高级选项)   -k, --keep          保留 (不删除) 输入文件
  -f, --force         强制覆盖输出文件并且 (解)压缩链接
  -c, --stdout        输出到标准输出并且不删除输入文件  操作模式：
 %s 的主页：<%s>
 %s：无法删除：%s %s：无法设定文件组：%s %s：无法设置文件所有者：%s %s：无法设置文件权限：%s %s：关闭文件失败：%s %s：读取文件名时错误：%s %s：文件的后缀已经是“%s”，跳过 %s：文件设置了setuid或setgid位，跳过 %s：文件设置了粘滞位（sticky bit），跳过 %s：发现文件已经移动，不会移除 %s：文件名的后缀未知，跳过 %s：输入文件有多个硬链接，跳过 %s：无效的选项名称 %s：无效的选项值 %s：是目录，跳过 %s：是符号链接，跳过 %s：不是常规文件，跳过 %s：读取文件名时遇到空字符；您可能想使用“--files0”选项而不是“--files”选项。 %s：读入错误：%s %s：意外的文件末尾 %s：读取文件名时遇到意外的结束标志 %s: 未知文件格式类型 %s: 不支持的完整性检查类型 %s：值不是一个非负的整数 %s：写入错误：%s 无法建立信号句柄 无法从标准输入读取文件名 不能从终端中读取数据 不能向终端中写入压缩数据 压缩数据已损坏 使用 --robot 选项的压缩和解压缩尚未被支持。 空文件名，跳过 文件格式无法识别 内部错误(bug) LZMA1 不能用于 .xz 格式 长选项必须使用的参数对于短选项时也是必需使用的。
 最大的过滤器数目是四个 给定的过滤器设置的内存使用限制过小。 达到内存使用限制 无完整性检查；未验证文件完整性 只能通过“--files”或“--files0”指定一个文件 .lzma 格式只支持 LZMA1 过滤器 这些预定义设置的具体取值根据版本可能会有所不同。 用“%s --help”来获得更多的信息 不可预料的输入结尾 未知错误 不支持的选项 不支持的完整性验证类型；未验证文件完整性 用法：%s [选项] ... [文件名] ...
压缩或解压缩 .xz 格式的文件。

 不建议在 raw 模式中使用预定义设置。 PRIu64 Value of the option `%s' must be in the range [%, %] 选项的“%s”的值必须在此范围内：[%, %] 