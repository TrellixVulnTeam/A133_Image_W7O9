Þ    =        S   ì      8     9  c   Ð     4  a   Æ  K   (  ü   t  '  q     	     °	  *   ¿	  ë  ê	  -   Ö  +     ,   0  &   ]       .     )   Î  )   ø  -   "  I   P          ¨     Ç  "   Õ      ø  &     -   @  -   n          ¼  #   Ì  6   ð     '     A     \     d     y  &     O   ´  -        2     @  #   P  "   t  %     "   ½  %   à          &     ;     Z     s       5        ¼     Ð     ä     û         ,     ·  c   N     ²  a   D  K   ¦  D  ò  '  7     _     v  *     '  ¼  -   ä  +     ,   >  &   k       :   ­  )   è  5     9   H  I        Ì     Ú     ù  "         *  &   K  -   r  -         Î     î  #   þ  6   "     Y     s               «  2   ¿  O   ò  -   B     p     ~  #     "   ²  %   Õ  "   û  %         D      d      y            ±      ½   5   Ä      ú      !     "!     9!     Y!     9   #   *       
      <          1   ,          0              !               8       7                              =                      $                         +             6   %   	         /   .   &   -          4      :       (   ;          2   3       '                   )   "      5          --allow-unassigned   Toggle IDNA AllowUnassigned flag (default off)
      --usestd3asciirules  Toggle IDNA UseSTD3ASCIIRules flag (default off)
       --debug              Print debugging information
      --quiet              Silent operation
       --no-tld             Don't check string for TLD specific rules
                             Only for --idna-to-ascii and --idna-to-unicode
   -h, --help               Print help and exit
  -V, --version            Print version and exit
   -n, --nfkc               Normalize string according to Unicode v3.2 NFKC
   -p, --profile=STRING     Use specified stringprep profile instead
                             Valid stringprep profiles: `Nameprep',
                             `iSCSI', `Nodeprep', `Resourceprep', 
                             `trace', `SASLprep'
   -s, --stringprep         Prepare string according to nameprep profile
  -d, --punycode-decode    Decode Punycode
  -e, --punycode-encode    Encode Punycode
  -a, --idna-to-ascii      Convert to ACE according to IDNA (default mode)
  -u, --idna-to-unicode    Convert from ACE according to IDNA
 Cannot allocate memory Charset `%s'.
 Code points prohibited by top-level domain Command line interface to the internationalized domain name library.

All strings are expected to be encoded in the preferred charset used
by your locale.  Use `--debug' to find out what this charset is.  You
can override the charset used by setting environment variable CHARSET.

To process a string that starts with `-', for example `-foo', use `--'
to signal the end of parameters, as in `idn --quiet -a -- -foo'.

Mandatory arguments to long options are mandatory for short options too.
 Conflicting bidirectional properties in input Could not convert string in locale encoding Could not convert string in locale encoding. Error in stringprep profile definition Flag conflict with profile Forbidden leading or trailing minus sign (`-') Forbidden unassigned code points in input Input already contain ACE prefix (`xn--') Input does not start with ACE prefix (`xn--') Internationalized Domain Name (IDN) convert STRINGS, or standard input.

 Invalid input Malformed bidirectional string Missing input No top-level domain found in input Non-digit/letter/hyphen in input Output would be too large or too small Output would exceed the buffer space provided Prohibited bidirectional code points in input Prohibited code points in input Punycode failed String not idempotent under ToASCII String not idempotent under Unicode NFKC normalization String preparation failed String size limit exceeded Success System dlopen failed System iconv failed Try `%s --help' for more information.
 Type each input string on a line by itself, terminated by a newline character.
 Unicode normalization failed (internal error) Unknown error Unknown profile Usage: %s [OPTION]... [STRINGS]...
 could not convert from %s to UTF-8 could not convert from UCS-4 to UTF-8 could not convert from UTF-8 to %s could not convert from UTF-8 to UCS-4 could not do NFKC normalization idna_to_ascii_4z: %s idna_to_unicode_8z4z (TLD): %s idna_to_unicode_8z4z: %s input error malloc only one of -s, -e, -d, -a, -u or -n can be specified punycode_decode: %s punycode_encode: %s stringprep_profile: %s tld_check_4z (position %lu): %s tld_check_4z: %s Project-Id-Version: libidn 1.33
Report-Msgid-Bugs-To: bug-libidn@gnu.org
PO-Revision-Date: 2015-11-10 08:55+0000
Last-Translator: Launchpad Translations Administrators <Unknown>
Language-Team: none
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2019-07-18 10:56+0000
X-Generator: Launchpad (build 19010)
Language: en@boldquot
       --allow-unassigned   Toggle IDNA AllowUnassigned flag (default off)
      --usestd3asciirules  Toggle IDNA UseSTD3ASCIIRules flag (default off)
       --debug              Print debugging information
      --quiet              Silent operation
       --no-tld             Don't check string for TLD specific rules
                             Only for --idna-to-ascii and --idna-to-unicode
   -h, --help               Print help and exit
  -V, --version            Print version and exit
   -n, --nfkc               Normalize string according to Unicode v3.2 NFKC
   -p, --profile=STRING     Use specified stringprep profile instead
                             Valid stringprep profiles: â[1mNameprep[0mâ,
                             â[1miSCSI[0mâ, â[1mNodeprep[0mâ, â[1mResourceprep[0mâ, 
                             â[1mtrace[0mâ, â[1mSASLprep[0mâ
   -s, --stringprep         Prepare string according to nameprep profile
  -d, --punycode-decode    Decode Punycode
  -e, --punycode-encode    Encode Punycode
  -a, --idna-to-ascii      Convert to ACE according to IDNA (default mode)
  -u, --idna-to-unicode    Convert from ACE according to IDNA
 Cannot allocate memory Charset â[1m%s[0mâ.
 Code points prohibited by top-level domain Command line interface to the internationalized domain name library.

All strings are expected to be encoded in the preferred charset used
by your locale.  Use â[1m--debug[0mâ to find out what this charset is.  You
can override the charset used by setting environment variable CHARSET.

To process a string that starts with â[1m-[0mâ, for example â[1m-foo[0mâ, use â[1m--[0mâ
to signal the end of parameters, as in â[1midn --quiet -a -- -foo[0mâ.

Mandatory arguments to long options are mandatory for short options too.
 Conflicting bidirectional properties in input Could not convert string in locale encoding Could not convert string in locale encoding. Error in stringprep profile definition Flag conflict with profile Forbidden leading or trailing minus sign (â[1m-[0mâ) Forbidden unassigned code points in input Input already contain ACE prefix (â[1mxn--[0mâ) Input does not start with ACE prefix (â[1mxn--[0mâ) Internationalized Domain Name (IDN) convert STRINGS, or standard input.

 Invalid input Malformed bidirectional string Missing input No top-level domain found in input Non-digit/letter/hyphen in input Output would be too large or too small Output would exceed the buffer space provided Prohibited bidirectional code points in input Prohibited code points in input Punycode failed String not idempotent under ToASCII String not idempotent under Unicode NFKC normalization String preparation failed String size limit exceeded Success System dlopen failed System iconv failed Try â[1m%s --help[0mâ for more information.
 Type each input string on a line by itself, terminated by a newline character.
 Unicode normalization failed (internal error) Unknown error Unknown profile Usage: %s [OPTION]... [STRINGS]...
 could not convert from %s to UTF-8 could not convert from UCS-4 to UTF-8 could not convert from UTF-8 to %s could not convert from UTF-8 to UCS-4 could not do NFKC normalization idna_to_ascii_4z: %s idna_to_unicode_8z4z (TLD): %s idna_to_unicode_8z4z: %s input error malloc only one of -s, -e, -d, -a, -u or -n can be specified punycode_decode: %s punycode_encode: %s stringprep_profile: %s tld_check_4z (position %lu): %s tld_check_4z: %s 