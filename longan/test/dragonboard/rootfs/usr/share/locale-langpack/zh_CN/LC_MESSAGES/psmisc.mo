��    L      |  e   �      p  `   q  b   �  p   5  k   �  #        6     S     j  )   �  	   �  3   �     �  �   	      �	  ,   �	  $   �	     
      
     <
     \
  #   {
  !   �
     �
     �
  %   �
          3     N     e     t     �     �     �  �   �  &   �     �     �     �  �     d   �     S  $   j  u   �  C     =   I     �  &   �  +   �     �  )        .  (   H  �   q  .   -  F   \  "   �  -   �     �  
          2   2  $   e  ,   �  '   �  '   �            +   "     N     c     w          �     �     �  �  �  p   �  b   �  �   X  w   �  )   S     }     �     �  1   �     �  ?        A  �   _     �  ,     $   A     f     }      �     �  "   �      �       $   3  $   X     }  &   �     �     �     �     �          0  �   N  $   !     F     V     j  �   |  _        z  $   �  �   �  G   `  @   �     �  #     )   &     P  )   m     �  .   �  �   �  -   �   *   �   "   �   <   !  %   I!     o!     |!  /   �!  &   �!  2   �!  "   "  ,   <"     i"     p"  /   �"     �"     �"  	   �"  	   �"     �"     �"     �"     0   K         1          G   2                  J       )   #          7   .   =              9   &                           E               F   "   +          5       '      ;   4   ?       	       8   
         A   :         !   <   /                         @         I   B   6                     -      %   ,       L      C   D   (            *      >   H   $   3      -                     reset options

  udp/tcp names: [local_port][,[rmt_host][,[rmt_port]]]

   -4,--ipv4             search IPv4 sockets only
  -6,--ipv6             search IPv6 sockets only
   -Z,--context REGEXP kill only process(es) having context
                      (must precede other arguments)
   PID    start at this PID; default is 1 (init)
  USER   show only trees rooted at processes of this user

 %*s USER        PID ACCESS COMMAND
 %s is empty (not mounted ?)
 %s: Invalid option %s
 %s: no process found
 %s: unknown signal; %s -l lists signals.
 (unknown) /proc is not mounted, cannot stat /proc/self/stat.
 Bad regular expression: %s
 CPU Times
  This Process    (user system guest blkio): %6.2f %6.2f %6.2f %6.2f
  Child processes (user system guest):       %6.2f %6.2f %6.2f
 Can't get terminal capabilities
 Cannot allocate memory for matched proc: %s
 Cannot find socket's device number.
 Cannot find user %s
 Cannot open /proc directory: %s
 Cannot open /proc/net/unix: %s
 Cannot open a network socket.
 Cannot open protocol file "%s": %s
 Cannot resolve local port %s: %s
 Cannot stat %s: %s
 Cannot stat file %s: %s
 Copyright (C) 2007 Trent Waddington

 Could not kill process %d: %s
 Error attaching to pid %i
 Invalid namespace name Invalid option Invalid time format Kill %s(%s%d) ? (y/N)  Kill process %d ? (y/N)  Killed %s(%s%d) with signal %d
 Memory
  Vsize:       %-10s
  RSS:         %-10s 		 RSS Limit: %s
  Code Start:  %#-10lx		 Code Stop:  %#-10lx
  Stack Start: %#-10lx
  Stack Pointer (ESP): %#10lx	 Inst Pointer (EIP): %#10lx
 Namespace option requires an argument. No process specification given No processes found.
 No such user name: %s
 PSmisc comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it under
the terms of the GNU General Public License.
For more information about these matters, see the files named COPYING.
 Page Faults
  This Process    (minor major): %8lu  %8lu
  Child Processes (minor major): %8lu  %8lu
 Press return to close
 Process with pid %d does not exist.
 Process, Group and Session IDs
  Process ID: %d		  Parent ID: %d
    Group ID: %d		 Session ID: %d
  T Group ID: %d

 Process: %-14s		State: %c (%s)
  CPU#:  %-3d		TTY: %s	Threads: %ld
 Scheduling
  Policy: %s
  Nice:   %ld 		 RT Priority: %ld %s
 Signal %s(%s%d) ? (y/N)  Specified filename %s does not exist.
 Specified filename %s is not a mountpoint.
 TERM is not set
 Unable to open stat file for pid %d (%s)
 Unknown local port AF %d
 Usage: killall [OPTION]... [--] NAME...
 Usage: prtstat [options] PID ...
       prtstat -V
Print information about a process
    -r,--raw       Raw display of information
    -V,--version   Display version information and exit
 You can only use files with mountpoint options You cannot search for only IPv4 and only IPv6 sockets at the same time You must provide at least one PID. all option cannot be used with silent option. asprintf in print_stat failed.
 disk sleep fuser (PSmisc) %s
 killall: %s lacks process entries (not mounted ?)
 killall: Bad regular expression: %s
 killall: Cannot get UID from process status
 killall: Maximum number of names is %d
 killall: skipping partial match %s(%d)
 paging peekfd (PSmisc) %s
 procfs file for %s namespace not available
 prtstat (PSmisc) %s
 pstree (PSmisc) %s
 running sleeping traced unknown zombie Project-Id-Version: psmisc 22.21-pre2
Report-Msgid-Bugs-To: csmall@enc.com.au
PO-Revision-Date: 2017-06-12 17:15+0000
Last-Translator: Mingye Wang <arthur2e5@aosc.xyz>
Language-Team: Chinese (simplified) <i18n-zh@googlegroups.com>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 13:23+0000
X-Generator: Launchpad (build 18719)
Language: zh_CN
X-Bugs: Report translation errors to the Language-Team address.
   -                     重置选项

  udp/tcp 名字格式: [本地端口][,[远程主机][,[远程端口]]]

   -4,--ipv4             仅查找 IPv4 套接字
  -6,--ipv6             仅查找 IPv6 套接字
   -Z,--context 正则表达式 仅杀死含有指定上下文的进程
                          (必须在其他参数前使用)
   进程号 从 "进程号" 开始，缺省是1(init)
  用户   仅显示从该 "用户" 的进程开始的进程树

 %*s 用户     进程号 权限   命令
 %s 是空的 (未挂载?)
 %s：无效的选项 %s
 %s: 未找到进程
 %s: 未知的信号; %s -l 显示信号列表。
 (未知) /proc 未被挂载，无法取得 /proc/self/stat 的状态。
 错误的正则表达式: %s
 CPU 时间
  此进程 (用户 系统 访客 区块输出入)：%6.2f %6.2f %6.2f %6.2f
  子进程 (用户 系统 访客)：           %6.2f %6.2f %6.2f
 无法获取终端功能
 无法为已找到的进程分配内存: %s
 无法找到 socket 的设备号。
 无法找到用户 %s
 无法打开 /proc 目录: %s
 无法打开 /proc/net/unix: %s
 无法打开网络端口。
 无法打开协议文件 "%s": %s
 无法解析本地端口 %s: %s
 无法分析 %s: %s
 无法获取 %s 的文件状态: %s
 Copyright © 2007 Trent Waddington

 无法杀死进程 %d: %s
 绑定到 %i 号进程时发生错误
 无效的命名空间名 无效的选项 无效的时间格式 杀死 %s(%s%d) ? (y/N)  杀死进程 %d ? (y/N)  %s(%s%d) 被信号 %d 杀死
 内存
  Vsize：      %-10s
  RSS：        %-10s		 RSS 限制：%s
  程序开始：   %#-10lx		 程序停止： %#-10lx
  堆栈开始：   %#-10lx
  堆栈指针 (ESP)：%#10lx	 指令指针 (EIP)：%#10lx
 命名空间选项需要一个参数 未指定进程 未发现进程。
 无用户名: %s
 PSmisc 不提供任何保证。
该程序为自由软件，欢迎你在 GNU 通用公共许可证 (GPL) 下重新发布。
详情可参阅 COPYING 文件。
 分页错误
  此进程 (次要 主要)：%8lu  %8lu
  子进程 (次要 主要)：%8lu  %8lu
 按回车以关闭
 具有 pid %d 的进程不存在。
 进程、群组和运行阶段识别码
    进程识别号：%d		     上层识别号：%d
    群组识别号：%d		 运行阶段识别码：%d
  T 群组识别号：%d

 进程：  %-14s		状态：%c (%s)
  CPU#: %-3d		TTY：%s	线程：%ld
 调度
  策略：%s
  友好：%ld 		 实时优先级：%ld %s
 信号 %s(%s%d) ? (y/N)  指定的文件名 %s 不存在。
 指定的文件名 %s 并非挂载点。
 未设置 TERM 环境变量
 无法打开 pid %d (%s) 的 stat 文件
 未知本地端口 AF %d
 用法: killall [选项]... [--] 进程名...
 用法：prtstat [选项] PID…
      prtstat -V
打印进程的相关信息
    -r,--raw       显示信息的原始样式
    -V,--version   显示版本信息然后离开
 你只能使用有挂载点选项的文件。 你无法同时搜索 IPv4 和 IPv6 端口 您必须提供至少一个 PID。 所有选项都不能与不输出信息选项同时使用。 print_stat 中的 asprintf 失败。
 磁盘暂停 fuser (PSmisc) %s
 killall: %s 不含进程信息项 (未挂载?)
 killall: 不当的正则表达式: %s
 killall: 无法从进程状态获取用户ID(UID)
 killall: 最大名字数量是 %d
 killall: 跳过部分符合的部分 %s(%d)
 分页 peekfd (PSmisc) %s
 适用 %s 命名空间的 proc 文件不可用
 prtstat (PSmisc) %s
 pstree (PSmisc) %s
 运行中 暂停中 追踪 未知 僵尸 