��    -      �  =   �      �     �  �   �     �     �     �     �     
  7        I     d     i     ~     �     �     �  ?   �  4     %   K  &   q     �     �     �  "   �  6   �  >   '     f     }  &   �  M   �  +     6   =     t  .   �     �     �     �  ,    	     -	     3	     M	     b	     g	  (   z	  1   �	  }  �	     S  �   o     /     L     U     i     �  1   �     �     �     �     �               )  '   6     ^     }  ,   �     �  	   �     �  !   �  9     (   V          �  $   �  D   �  +     )   8     b     o     �     �     �  '   �                     9     =     P  -   o        $      ,                                                       )       
         #      *                      !           &                %   	         "       -             +                     '   (       
<action> is one of:
 
<name> is the device to create under %s
<device> is the encrypted device
<key slot> is the LUKS key slot number to modify
<key file> optional key file for the new key for luksAddKey action
 %s: requires %s as arguments <device> <device> <key slot> <device> [<new key file>] <name> Align payload at <n> sector boundaries - for luksFormat Argument <action> missing. BITS Command successful.
 Create a readonly mapping Display brief usage Do not ask for confirmation Help options: How many sectors of the encrypted data to skip at the beginning How often the input of the passphrase can be retried Key size must be a multiple of 8 bits PBKDF2 iteration time for LUKS (in ms) Print package version SECTORS Show this help message Shows more detailed error messages The cipher used to encrypt the disk (see /proc/crypto) The hash used to create the encryption key from the passphrase The size of the device The size of the encryption key The start offset in the backend device This is the last keyslot. Device will become unusable after purging this key. This will overwrite data on %s irrevocably. Timeout for interactive passphrase prompt (in seconds) Unknown action. Verifies the passphrase by asking for it twice add key to LUKS device dump LUKS partition information formats a LUKS device memory allocation error in action_luksFormat msecs print UUID of LUKS device resize active device secs show device status tests <device> for LUKS partition header wipes key with number <key slot> from LUKS device Project-Id-Version: cryptsetup
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2009-09-29 07:47+0000
Last-Translator: Wylmer Wang <Unknown>
Language-Team: Simplified Chinese <zh_CN@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 13:31+0000
X-Generator: Launchpad (build 18719)
 
<action> 是其中之一:
 
<名称> 是这个设备创建在 %s 之下
<设备> 是这个加密的设备
<key slot> 是 LUKS key slot 数字去设置
<key file> optional key file for the new key for luksAddKey action
 %s: requires %s 作为参数 <设备> <设备> <key slot> <设备> [<新密钥文件>] <名称> 为luksFormat排列有效负荷在<n>扇区边界 缺少<action>参数. 位数 命令成功.
 创建一个只读映射 显示简短用法 不询问确认 帮助选项 加密数据在开始跳过多少扇区 多久能重试再输入密钥 密码长度必须是8的倍数 用于LUKS的PBKDF2 重复时间 (毫秒内) 显示包版本 扇区数 显示该帮助信息 显示更多的详细错误信息 这个密码用于加密这个磁盘(参看 /proc/crypto) hash用于从密钥中创建加密密钥 设备尺寸 加密密码的长度 在后台设备中的开始偏移量 这是最后的 keyslot. 设备将不可用在清除这个密钥后. 这将不能取消地在 %s 上覆盖数据 交互密钥提示的时间限制(秒数) 未知参数 通过两次来校验密钥 添加密钥到LUKS设备 复制LUKS分区信息 格式化成一个LUKS设备 内存配置错误在 action_luksFormat 毫秒 显示LUKS设备的UUID 改变激活分区大小 秒 显示设备状态 为LUKS分区头检测<设备> 从LUKS 设备用数字<key slot>清除密钥 