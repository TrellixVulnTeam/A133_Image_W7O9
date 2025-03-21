��    K      t  e   �      `  $   a  &   �  )   �      �  #   �  -     *   J  +   u  =   �  )   �  ;   	  <   E      �  :   �     �     �      	     0	  $   L	     q	     �	  (   �	     �	     �	     
      
     :
     N
  '   g
  &   �
  "   �
  $   �
     �
  $        B      ]  #   ~  )   �  ,   �  0   �  "   *     M  /   g     �      �  !   �      �  #        >     Q  ?   i  4   �  -   �  4     4   A  $   v  &   �  ,   �     �  "        *  ;   C  *        �     �  %   �       �       �  �   �  5   t     �     �     �  �  �      k  (   �  )   �     �  #   �  ,   !  )   N  %   x  7   �  %   �  7   �  /   4  "   d  .   �     �     �     �                =     W  #   r     �     �     �     �     �       *     *   H  #   s      �      �  #   �     �          -  %   E  (   k  ,   �     �     �  *   �     %     ;  %   X     ~     �     �     �  ?   �  3     +   Q  3   }  3   �  +   �  *     5   <  "   r      �     �  =   �           *      @   )   Q      {   �  �     ;$  �   I%  <   
&     G&     ]&     }&            	   <           =   K          -   )                  J                E   5   3      #   6      $   @   (   4      A   2           "           B   ;          *            &   
   I   F           1   !   ?       0      :             H   G       +                  9   ,           >   C                       '   7                      D       %   8   /      .       %s: %s doesn't exist, skipping call
 %s: %s is encrypted on real device %s
 %s: CD-ROM auto-eject command failed: %s
 %s: CD-ROM eject command failed
 %s: CD-ROM eject command succeeded
 %s: CD-ROM load from slot command failed: %s
 %s: CD-ROM select disc command failed: %s
 %s: CD-ROM select speed command failed: %s
 %s: CD-ROM select speed command not supported by this kernel
 %s: CD-ROM tray close command failed: %s
 %s: CD-ROM tray close command not supported by this kernel
 %s: CD-ROM tray toggle command not supported by this kernel
 %s: FindDevice called too often
 %s: IDE/ATAPI CD-ROM changer not supported by this kernel
 %s: SCSI eject failed
 %s: SCSI eject succeeded
 %s: `%s' can be mounted at `%s'
 %s: `%s' is a link to `%s'
 %s: `%s' is a multipartition device
 %s: `%s' is mounted at `%s'
 %s: `%s' is not a mount point
 %s: `%s' is not a multipartition device
 %s: `%s' is not mounted
 %s: closing tray
 %s: could not allocate memory
 %s: default device: `%s'
 %s: device is `%s'
 %s: device name is `%s'
 %s: disabling auto-eject mode for `%s'
 %s: enabling auto-eject mode for `%s'
 %s: error while allocating string
 %s: error while finding CD-ROM name
 %s: error while reading speed
 %s: exiting due to -n/--noop option
 %s: expanded name is `%s'
 %s: floppy eject command failed
 %s: floppy eject command succeeded
 %s: invalid argument to --auto/-a option
 %s: invalid argument to --cdspeed/-x option
 %s: invalid argument to --changerslot/-c option
 %s: invalid argument to -i option
 %s: listing CD-ROM speed
 %s: maximum symbolic link depth exceeded: `%s'
 %s: selecting CD-ROM disc #%d
 %s: setting CD-ROM speed to %dX
 %s: setting CD-ROM speed to auto
 %s: tape offline command failed
 %s: tape offline command succeeded
 %s: toggling tray
 %s: too many arguments
 %s: tried to use `%s' as device name but it is no block device
 %s: trying to eject `%s' using CD-ROM eject command
 %s: trying to eject `%s' using SCSI commands
 %s: trying to eject `%s' using floppy eject command
 %s: trying to eject `%s' using tape offline command
 %s: unable to eject, last error: %s
 %s: unable to exec umount of `%s': %s
 %s: unable to find or open device for: `%s'
 %s: unable to fork: %s
 %s: unable to open /etc/fstab: %s
 %s: unable to open `%s'
 %s: unable to read the speed from /proc/sys/dev/cdrom/info
 %s: unmount of `%s' did not exit normally
 %s: unmount of `%s' failed
 %s: unmounting `%s'
 %s: unmounting device `%s' from `%s'
 %s: using default device `%s'
 Eject version %s by Jeff Tranter (tranter@pobox.com)
Usage:
  eject -h				-- display command usage and exit
  eject -V				-- display program version and exit
  eject [-vnrsfqpm] [<name>]		-- eject device
  eject [-vn] -d			-- display default device
  eject [-vn] -a on|off|1|0 [<name>]	-- turn auto-eject feature on or off
  eject [-vn] -c <slot> [<name>]	-- switch discs on a CD-ROM changer
  eject [-vn] -t [<name>]		-- close tray
  eject [-vn] -T [<name>]		-- toggle tray
  eject [-vn] -i on|off|1|0 [<name>]	-- toggle manual eject protection on/off
  eject [-vn] -x <speed> [<name>]	-- set CD-ROM max speed
  eject [-vn] -X [<name>]		-- list CD-ROM available speeds
Options:
  -v	-- enable verbose output
  -n	-- don't eject, just show device found
  -r	-- eject CD-ROM
  -s	-- eject SCSI device
  -f	-- eject floppy
  -q	-- eject tape
  -p	-- use /proc/mounts instead of /etc/mtab
  -m	-- do not unmount device even if it is mounted
 Long options:
  -h --help   -v --verbose      -d --default
  -a --auto   -c --changerslot  -t --trayclose  -x --cdspeed
  -r --cdrom  -s --scsi         -f --floppy     -X --listspeed     -q --tape
  -n --noop   -V --version
  -p --proc   -m --no-unmount   -T --traytoggle
 Parameter <name> can be a device file or a mount point.
If omitted, name defaults to `%s'.
By default tries -r, -s, -f, and -q in order until success.
 eject version %s by Jeff Tranter (tranter@pobox.com)
 unable to open %s: %s
 usage: volname [<device-file>]
 volname Project-Id-Version: eject
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2009-03-26 13:33+0000
Last-Translator: Feng Chao <chaofeng111@gmail.com>
Language-Team: Chinese (China) <zh_CN@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 12:50+0000
X-Generator: Launchpad (build 18719)
 %s: %s 不存在，跳过调用
 %s： %s 被加密在真实的设备 %s
 %s：光驱自动弹出命令失败：%s
 %s: 光驱弹出命令失败
 %s: 光驱弹出命令成功执行
 %s：从插槽载入光驱命令失败：%s
 %s：光驱选择光盘命令失败：%s
 %s:光驱速度选择命令失败:%s
 %s:光驱速度选择命令不被本版本内核支持
 %s:光驱托盘关闭命令失败:%s
 %s:光驱托盘关闭命令不被本版本内核支持
 %s: 此内核不支持CD-ROM托盘切换命令
 %s：FindDivice调用过于频繁
 %s:IDE/ATAPI光驱不被本版本内核支持
 %s: SCSI弹出失败
 %s: SCSI弹出成功
 %s: `%s'可以被装载到'%s'
 %s: '%s'与'%s'有连接
 %s:'%s'是一个多分区设备
 %s:'%s'被装载到 `%s'
 %s：`%s' 不是挂载点
 %s:'%s'不是一个多分区设备
 %s: '%s'未被装载
 %s: 关闭托盘
 %s：无法分配内存
 %s: 默认设备: `%s'
 %s: 设备为`%s'
 %s: 设备名字为: `%s'
 %s: 不能启用'%s'的自动弹出模式
 %s: 正在启用'%s'的自动弹出模式
 %s: 分配字符串时出现错误
 %s: 查找CD-ROM名称时出错
 %s: 读取速度信息时出错
 %s: 因为-n/--noop选项而退出
 %s: 扩展名为: `%s'
 %s: 软驱弹出失败
 %s: 软驱弹出成功
 %s：--auto/-a 选项的参数无效
 %s：--cdspeed/-x 选项的参数无效
 %s：--changerslot/-c 选项的参数无效
 %s: -i 选项的无效参数
 %s: 列出CD-ROM的速度
 %s：超出最大符号链接深度：`%s
 %s：选择光盘#%d
 %s:设置光驱速度为%dX
 %s:设置光驱速度为自动选择
 %s:离线磁带命令失败
 %s:离线磁带命令成功
 %s: 切换托盘
 %s：参数太多
 %s：尝试使用 `%s' 作为设备名，但它不是块设备
 %s: 正在通过光驱弹出命令尝试弹出'%s'
 %s: 正在尝试通过SCSI命令弹出'%s'
 %s: 正在通过软驱弹出命令尝试弹出'%s'
 %s: 正在通过离线磁带命令尝试弹出'%s'
 %s:不能够弹出，最后的错误为:%s
 %s：无法执行`%s'的umount命令：%s
 %s: 不能够找到或者打开设备原因为: `%s'
 %s:不能够执行派生指令:%s
 %s:不能够打开/etc/fstab:%s
 %s:不能够打开`%s'
 %s: 无法从 /proc/sys/dev/cdrom/info 中读取速度信息
 %s:卸载"%s"非正常退出
 %s:卸载 `%s'失败
 %s:卸载中:%s
 %s: 将设备 `%s'从`%s'中卸载出来
 %s: 使用默认设备: `%s'
 Eject 版本为 %s 由 Jeff Tranter (tranter@pobox.com)
用法:
  eject -h				-- 显示命令用法然后退出
  eject -V				-- 显示程序版本然后退出
  eject [-vnrsfqpm] [<name>]		-- 弹出驱动器
  eject [-vn] -d			-- 显示默认驱动器
  eject [-vn] -a on|off|1|0 [<name>]	-- 开/关自动弹出功能
  eject [-vn] -c <slot> [<name>]	-- 在 CD-ROM 中换盘
  eject [-vn] -t [<name>]		-- 关闭托盘
  eject [-vn] -T [<name>]		-- 锁定托盘
  eject [-vn] -i on|off|1|0 [<name>]	-- 开/关手动弹出
  eject [-vn] -x <speed> [<name>]	-- 将 CD-ROM 设为全速
  eject [-vn] -X [<name>]		-- 列出可用的 CD-ROM 速度
选项:
  -v	-- 显示更多输出
  -n	-- 不要弹出，仅仅显示找到的设备
  -r	-- 弹出 CD-ROM
  -s	-- 弹出 SCSI 设备
  -f	-- eject 软盘
  -q	-- eject 磁带
  -p	-- 使用 /proc/mounts 而不是 /etc/mtab 中的数据
  -m	-- 不要卸载设备即使其已被挂载
 长选项:
  -h --help   -v --verbose      -d --default
  -a --auto   -c --changerslot  -t --trayclose  -x --cdspeed
  -r --cdrom  -s --scsi         -f --floppy     -X --listspeed     -q --tape
  -n --noop   -V --version
  -p --proc   -m --no-unmount   -T --traytoggle
 参数<name>可以是一个设备的文件夹或者是一个指定的装载。
如果忽略<name>参数，名字默认为 '%s'。
系统默认会依次尝试-r,-s,-r和-q命令直到成功。
 eject 版本：%s 作者：Jeff Tranter (tranter@pobox.com)
 不能够打开%s:%s
 使用：卷名[<device-file>]
 卷名 