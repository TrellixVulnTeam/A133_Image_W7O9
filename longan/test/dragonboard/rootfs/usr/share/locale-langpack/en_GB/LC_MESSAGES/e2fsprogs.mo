��    1      �  C   ,      8  .   9  *   h  �  �  �   +  �   �  #   I     m  +   �     �     �  0   �  ;   	  )   Y	     �	     �	  5   �	  3   �	  %   
     E
  O   a
  8   �
  #   �
  '     )   6     `  Y   y  =   �  9        K  ;   j     �     �     �     �           4     K  8   i  G   �  +   �       "   .  #   Q  %   u      �  3   �     �  "     �  0  -   �  )     �  6  �   �  �   Z  "   �       *   *     U     s  0   �  ;   �  *   �     "     A  8   R  6   �  $   �     �  N     8   Q  "   �  '   �  )   �     �  X     <   p  8   �     �  :        @     _     u     �      �     �     �  7     J   ;  *   �     �  #   �  #   �  %         8  3   Y     �  "   �           $   #       1          %              "   +   ,       '      (   !   *         	              &                          -              .   0                              
                           /                   )           
%s: ***** FILE SYSTEM ERRORS CORRECTED *****
 
%s: ***** FILE SYSTEM WAS MODIFIED *****
 
Bad option(s) specified: %s

Extended options are separated by commas, and may take an argument which
	is set off by an equals ('=') sign.

Valid extended options are:
	mmp_update_interval=<interval>
	num_backup_sb=<0|1|2>
	stride=<RAID per-disk data chunk in blocks>
	stripe-width=<RAID stride * data disks in blocks>
	offset=<offset to create the file system>
	resize=<resize maximum size in blocks>
	packed_meta_blocks=<0 to disable, 1 to enable>
	lazy_itable_init=<0 to disable, 1 to enable>
	lazy_journal_init=<0 to disable, 1 to enable>
	root_owner=<uid of root dir>:<gid of root dir>
	test_fs
	discard
	nodiscard
	quotatype=<quota type(s) to be enabled>

 
Resizing bigalloc file systems has not been fully tested.  Proceed at
your own risk!  Use the force option if you want to go ahead anyway.

 
Warning: offset specified without an explicit file system size.
Creating a file system with %llu blocks but this might
not be what you want.

  contains a file system with errors %s contains a %s file system
 %s contains a %s file system labelled '%s'
 %s: File system was modified.
 %s: e2fsck canceled.
 @d @i %i has @x marked uninitialized at @b %c.   @g descriptor %g marked uninitialized without feature set.
 @i %i has a extra size (%IS) which is @n
 Bad CRC detected in file system Can not continue. Cannot enable project feature; inode size too small.
 Cannot enable project quota; inode size too small.
 Cannot proceed with file system check Checking all file systems.
 Error in resizing the inode size.
Run e2undo to undo the file system changes. 
 Error initializing quota context in support library: %m
 Error writing file system info: %m
 Failed to optimize @x tree %p (%i): %m
 Failed to optimize directory %q (%d): %m
 File system is corrupted Filesystem at %s is mounted on %s, and on-line resizing is not supported on this system.
 Filesystem at %s is mounted on %s; on-line resizing required
 Kernel does not support resizing a file system this large Last @g @b @B uninitialized.   On-line resizing not supported with revision 0 filesystems
 On-line shrinking not supported Optimizing @x trees:  Optimizing directories:  Pass 1E: Optimizing @x trees
 Pass 3A: Optimizing directories
 Proceed anyway? (y,N)  Setting error behavior to %d
 The file system superblock doesn't match the undo file.
 This may result in very poor performance, (re)-partitioning suggested.
 While checking for on-line resizing support bad error behavior - %s bad error behavior in profile - %s while initializing ext2_qcow2_image while initializing journal superblock while initializing quota context while initializing quota context in support library while populating file system while trying to initialize program Project-Id-Version: e2fsprogs
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2018-03-16 11:25+0000
Last-Translator: Andi Chandler <Unknown>
Language-Team: English (United Kingdom) <en_GB@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=n != 1;
X-Launchpad-Export-Date: 2019-07-18 10:39+0000
X-Generator: Launchpad (build 19010)
 
%s: ***** FILESYSTEM ERRORS CORRECTED *****
 
%s: ***** FILESYSTEM WAS MODIFIED *****
 
Bad option(s) specified: %s

Extended options are separated by commas, and may take an argument which
	is set off by an equals ('=') sign.

Valid extended options are:
	mmp_update_interval=<interval>
	num_backup_sb=<0|1|2>
	stride=<RAID per-disk data chunk in blocks>
	stripe-width=<RAID stride * data disks in blocks>
	offset=<offset to create the filesystem>
	resize=<resize maximum size in blocks>
	packed_meta_blocks=<0 to disable, 1 to enable>
	lazy_itable_init=<0 to disable, 1 to enable>
	lazy_journal_init=<0 to disable, 1 to enable>
	root_owner=<uid of root dir>:<gid of root dir>
	test_fs
	discard
	nodiscard
	quotatype=<quota type(s) to be enabled>

 
Resizing bigalloc filesystems has not been fully tested.  Proceed at
your own risk!  Use the force option if you want to go ahead anyway.

 
Warning: offset specified without an explicit file system size.
Creating a filesystem with %llu blocks, but this might
not be what you want.

  contains a filesystem with errors %s contains a %s filesystem
 %s contains a %s filesystem labelled '%s'
 %s: Filesystem was modified.
 %s: e2fsck cancelled.
 @d @i %i has @x marked uninitialised at @b %c.   @g descriptor %g marked uninitialised without feature set.
 @i %i has an extra size (%IS) which is @n
 Bad CRC detected in filesystem Cannot continue. Cannot enable project feature; inode size is too small.
 Cannot enable project quota; inode size is too small.
 Cannot proceed with filesystem check Checking all filesystems.
 Error in resizing the inode size.
Run e2undo to undo the filesystem changes. 
 Error initialising quota context in support library: %m
 Error writing filesystem info: %m
 Failed to optimise @x tree %p (%i): %m
 Failed to optimise directory %q (%d): %m
 Filesystem is corrupted Filesystem at %s is mounted on %s, and online resizing is not supported on this system.
 Filesystem at %s is mounted on %s; online resizing required
 Kernel does not support resizing a filesystem this large Last @g @b @B uninitialised.   Online resizing not supported with revision 0 filesystems
 Online shrinking not supported Optimising @x trees:  Optimising directories:  Pass 1E: Optimising @x trees
 Pass 3A: Optimising directories
 Proceed anyway? (y/N)  Setting error behaviour to %d
 The filesystem superblock doesn't match the undo file.
 This may result in very poor performance: (re)-partitioning is suggested.
 While checking for online resizing support bad error behaviour - %s bad error behaviour in profile - %s while initialising ext2_qcow2_image while initialising journal superblock while initialising quota context while initialising quota context in support library while populating filesystem while trying to initialise program 