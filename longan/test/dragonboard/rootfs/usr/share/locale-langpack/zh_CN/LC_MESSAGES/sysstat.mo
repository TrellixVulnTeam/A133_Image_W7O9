��    B      ,  Y   <      �     �     �     �  %   �  &        .  %   G     m    �  -   �  ,   �  )   �       2   1     d     �  -   �  :   �  !   	  )   &	     P	  %   Y	  '   	     �	  $   �	  "   �	     
  .   
  -   I
  B   w
     �
  &   �
  "   �
  '        F  -   ^  6   �     �     �     �     �       !     '   >     f     {  %   �  8   �  +   �  {   !     �  +   �  #   �  .        :     R     _  $   h  1   �  J   �  ?   
  >   J     �     �     �  �  �     �     �     �     �      �     �  $        0  �   E        $   8  "   ]     �  &   �     �     �  3   �  1   '      Y  #   z     �  %   �     �     �  !     "   .     Q  2   f  2   �  :   �       $   "     G  1   g     �  &   �  ;   �          &  !   5     W     `  &   }     �     �     �  (   �  5     *   L  u   w     �  "     "   /  5   R      �     �     �  )   �  6   �  S   !  I   u  C   �                    "         )      5   -              2      *   4       /                    7   0          <       #                   %   .          >               $      B   3   :         @   =   9       !             1                   &       ;   (       ?   	      A          +       
              6           8       ,   '          		Filesystems statistics
 	-B	Paging statistics
 	-F [ MOUNT ]
 	-H	Hugepages utilization statistics
 	-S	Swap space utilization statistics
 	-W	Swapping statistics
 	-b	I/O and transfer rate statistics
 	-d	Block devices statistics
 	-m { <keyword> [,...] | ALL }
		Power management statistics
		Keywords are:
		CPU	CPU instantaneous clock frequency
		FAN	Fans speed
		FREQ	CPU average clock frequency
		IN	Voltage inputs
		TEMP	Devices temperature
		USB	USB devices plugged into the system
 	-q	Queue length and load average statistics
 	-r [ ALL ]
		Memory utilization statistics
 	-u [ ALL ]
		CPU utilization statistics
 	-v	Kernel tables statistics
 	-w	Task creation and system switching statistics
 	-y	TTY devices statistics
 	[Unknown activity format] 
CPU activity not found in file. Aborting...
 
File successfully converted to sysstat format version %s
 
Invalid data found. Aborting...
 -f and -o options are mutually exclusive
 Average: Cannot append data to that file (%s)
 Cannot convert the format of this file
 Cannot find disk data
 Cannot find the data collector (%s)
 Cannot handle so many processors!
 Cannot open %s: %s
 Cannot write data to system activity file: %s
 Cannot write system activity file header: %s
 Current sysstat version cannot read the format of this file (%#x)
 Data collector found: %s
 Data collector will be sought in PATH
 End of data collecting unexpected
 End of system activity file unexpected
 Endian format mismatch
 Error while reading system activity file: %s
 File created by sar/sadc from sysstat version %d.%d.%d File date: %s
 File time:  Genuine sa datafile: %s (%x)
 Host:  Inconsistent input data
 Invalid system activity file: %s
 Invalid type of persistent device name
 List of activities:
 Main options and reports:
 No tape drives with statistics found
 Not reading from a system activity file (use -f option)
 Number of CPU for last samples in file: %u
 Options are:
[ -C <comment> ] [ -D ] [ -F ] [ -L ] [ -V ]
[ -S { INT | DISK | IPV6 | POWER | SNMP | XDISK | ALL | XALL } ]
 Other devices not listed here Please check if data collecting is enabled
 Requested activities not available
 Requested activities not available in file %s
 Size of a long int: %d
 Statistics:  Summary: System activity data file: %s (%#x)
 Usage: %s [ options ] [ <interval> [ <count> ] ]
 Usage: %s [ options ] [ <interval> [ <count> ] ] [ <datafile> | -[0-9]+ ]
 Usage: %s [ options ] [ <interval> [ <count> ] ] [ <outfile> ]
 Using a wrong data collector from a different sysstat version
 no sysstat version %s
 yes Project-Id-Version: sysstat 11.5.1
Report-Msgid-Bugs-To: sysstat <at> orange.fr
PO-Revision-Date: 2018-01-02 10:39+0000
Last-Translator: Ji ZhengYu <Unknown>
Language-Team: Chinese (simplified) <i18n-zh@googlegroups.com>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 13:37+0000
X-Generator: Launchpad (build 18719)
Language: zh_CN
X-Bugs: Report translation errors to the Language-Team address.
 		文件系统统计信息
 	-B	分页状况
 	-F [ MOUNT ]
 	-H	交换空间利用率
 	-S	交换空间利用率信息
 	-W	交换信息
 	-b	I/O 和传输速率信息状况
 	-d	块设备状况
 	-m { <关键字> [,...] | ALL }
		电源管理统计信息
		关键字:
		CPU	CPU 频率
		FAN	风扇速度
\t\tFREQ\tCPU 平均时钟频率
		IN	输入电压
		TEMP	设备温度
\t\tUSB\t连接的USB 设备
 	-q	队列长度和平均负载
 	-r [ ALL ]
		内存利用率信息
 	-u [ ALL ]
		CPU 利用率信息
 	-v	内核表统计信息
 	-w	任务创建与系统转换信息
 	-y	TTY 设备信息
 	\t[未知的记录格式] 
未在文件中找到CPU 活动记录。退出...
 
文件已成功转化为sysstat 格式版本 %s
 
发现无效信息。退出...
 -f 和 -o 选项不能同时使用
 平均时间: 无法向文件 (%s) 中追加记录
 无法轮换此文件的格式
 无法找到磁盘数据
 无法找到数据收集器 (%s)
 处理器太多，无法处理！
 无法打开 %s: %s
 无法将数据写入系统运行记录文件: %s
 无法写系统运行记录文件开始部分: %s
 当前版本的 sysstat 无法读取此文件格式 (%#x)
 找到数据收集器: %s
 将在PATH 中查找数据收集器
 数据流结尾有未知错误
 系统运行记录文件的结尾有未知错误
 Endian 格式不匹配
 查看系统运行记录时出错: %s
 文件由 sysstat 版本 %d.%d.%d 中的 sar/sadc 创建。 文件日期: %s
 文件时间:  Genuine sa 数据文件: %s (%x)
 主机:  所取数据前后不一致
 无效的系统运行记录文件: %s
 固有设备名类型无效
 运行记录列表:
 主选项和报告：
 未找到带统计信息的磁带设备
 无法查看系统活动记录文件 (用 -f 选项)
 文件中最后一次取样的CPU 数: %u
 选项:
[ -C <注释> ] [ -D ] [ -F ] [ -L ] [ -V ]
[ -S { INT | DISK | IPV6 | POWER | SNMP | XDISK | ALL | XALL } ]
 未在此列出的其它设备 请检查是否允许数据收集
 所需的运行记录无法获得
 所需的运行记录在此文件 %s 中无法获得
 长整型数的字节大小: %d
 统计信息:  总计: 系统运行记录数据文件: %s (%#x)
 用法: %s [ 选项 ] [ <时间间隔> [ <次数> ] ]
 用法: %s [ 选项 ] [ <时间间隔> [ <次数> ] ] [ <数据文件> | -[0-9]+ ]
 用法: %s [ 选项 ] [ <时间间隔> [ <次数> ] ] [ <输出文件> ]
 正在使用来自不同版本 sysstat 的错误的数据收集器
 否 sysstat 版本 %s
 是 