��          ,      <       P   B  Q   �  �  B  A	                     Parse option arguments.
    
    Getopts is used by shell procedures to parse positional parameters
    as options.
    
    OPTSTRING contains the option letters to be recognized; if a letter
    is followed by a colon, the option is expected to have an argument,
    which should be separated from it by white space.
    
    Each time it is invoked, getopts will place the next option in the
    shell variable $name, initializing name if it does not exist, and
    the index of the next argument to be processed into the shell
    variable OPTIND.  OPTIND is initialized to 1 each time the shell or
    a shell script is invoked.  When an option requires an argument,
    getopts places that argument into the shell variable OPTARG.
    
    getopts reports errors in one of two ways.  If the first character
    of OPTSTRING is a colon, getopts uses silent error reporting.  In
    this mode, no error messages are printed.  If an invalid option is
    seen, getopts places the option character found into OPTARG.  If a
    required argument is not found, getopts places a ':' into NAME and
    sets OPTARG to the option character found.  If getopts is not in
    silent mode, and an invalid option is seen, getopts places '?' into
    NAME and unsets OPTARG.  If a required argument is not found, a '?'
    is placed in NAME, OPTARG is unset, and a diagnostic message is
    printed.
    
    If the shell variable OPTERR has the value 0, getopts disables the
    printing of error messages, even if the first character of
    OPTSTRING is not a colon.  OPTERR has the value 1 by default.
    
    Getopts normally parses the positional parameters ($0 - $9), but if
    more arguments are given, they are parsed instead.
    
    Exit Status:
    Returns success if an option is found; fails if the end of options is
    encountered or an error occurs. Project-Id-Version: bash
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2015-05-20 12:23+0000
Last-Translator: Joel Addison <jaddi27@gmail.com>
Language-Team: English (Australia) <en_AU@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=n != 1;
X-Launchpad-Export-Date: 2019-07-18 10:32+0000
X-Generator: Launchpad (build 19010)
 Parse option arguments.
    
    Getopts is used by shell procedures to parse positional parameters
    as options.
    
    OPTSTRING contains the option letters to be recognised; if a letter
    is followed by a colon, the option is expected to have an argument,
    which should be separated from it by white space.
    
    Each time it is invoked, getopts will place the next option in the
    shell variable $name, initializing name if it does not exist, and
    the index of the next argument to be processed into the shell
    variable OPTIND.  OPTIND is initialized to 1 each time the shell or
    a shell script is invoked.  When an option requires an argument,
    getopts places that argument into the shell variable OPTARG.
    
    getopts reports errors in one of two ways.  If the first character
    of OPTSTRING is a colon, getopts uses silent error reporting.  In
    this mode, no error messages are printed.  If an invalid option is
    seen, getopts places the option character found into OPTARG.  If a
    required argument is not found, getopts places a ':' into NAME and
    sets OPTARG to the option character found.  If getopts is not in
    silent mode, and an invalid option is seen, getopts places '?' into
    NAME and unsets OPTARG.  If a required argument is not found, a '?'
    is placed in NAME, OPTARG is unset, and a diagnostic message is
    printed.
    
    If the shell variable OPTERR has the value 0, getopts disables the
    printing of error messages, even if the first character of
    OPTSTRING is not a colon.  OPTERR has the value 1 by default.
    
    Getopts normally parses the positional parameters ($0 - $9), but if
    more arguments are given, they are parsed instead.
    
    Exit Status:
    Returns success if an option is found; fails if the end of options is
    encountered or an error occurs. 