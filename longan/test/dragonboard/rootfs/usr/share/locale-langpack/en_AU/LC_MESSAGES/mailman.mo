��    8      �  O   �      �  9  �  �     `   �     #     =     P     d     �  �  �  ,  �  �  �  
   �  �  �     �  ]   �  �    (   �  �   �  �  C       �  #  0   �  6	  �     -$  �  E$    *&  �  I.  u  �0  �  I2  ;   �@  "  A  [   3B  �  �B  *  WK  �  �L  �  _N  @  �O  .  ?T  �  nW  �   
Y    �Y    �Z  �   �\  �  �]  2   �_  2   �_  *  #`  �   Nd  1   �d  ,    e  .   Me  �   |e  �   Of  �  �f  �   �i  �  Ij  ;  �k  �   m  `   �m     "n     <n     Pn      en     �n  �  �n  -  �r  �  �t  
   �x  �  �x     �z  ^   �z  �  {  )   �}  �   �}  �  M~     �  �  .�  0   с  6	  �     9�  �  R�    8�  �  X�  u  �  �  X�  <   �  "   �  [   C�  �  ��  *  g�  �  ��  �  o�  @  �  /  P�  �  ��  �   �    ��    ��  �   ��  �  ��  2   ��  2   �  ,  7�  �   d�  2   �  -   8�  /   f�  �   ��  �   h�  �  �  �   ��     "       3      )       1   8                           .   &      *      $                     (       /              '                                        +   	   ,      #   0       -   !          2   5                 6          4             %                        
      7    
Check a given Mailman translation, making sure that variables and
tags referenced in translation are the same variables and tags in
the original templates and catalog.

Usage:

cd $MAILMAN_DIR
%(program)s [-q] <lang>

Where <lang> is your country code (e.g. 'it' for Italy) and -q is
to ask for a brief summary.
             You have successfully canceled the posting of your message with
            the Subject: header <em>%(subject)s</em> to the mailing list
            %(listname)s.  (but we obscure the addresses so they are not
                easily recognizable by spammers).  (requires authorization) ...after this one. ...before this one. ...or specify a file to upload: Authorization failed. Clean up an .mbox archive file.

The archiver looks for Unix-From lines separating messages in an mbox archive
file.  For compatibility, it specifically looks for lines that start with
"From " -- i.e. the letters capital-F, lowercase-r, o, m, space, ignoring
everything else on the line.

Normally, any lines that start "From " in the body of a message should be
escaped such that a > character is actually the first on a line.  It is
possible though that body lines are not actually escaped.  This script
attempts to fix these by doing a stricter test of the Unix-From lines.  Any
lines that start "From " but do not pass this stricter test are escaped with a
> character.

Usage: cleanarch [options] < inputfile > outputfile
Options:
    -s n
    --status=n
        Print a # character every n lines processed

    -q / --quiet
        Don't print changed line information to standard error.

    -n / --dry-run
        Don't actually output anything.

    -h / --help
        Print this message and exit
 Cull bad and shunt queues, recommended once per day.

This script goes through the 'bad' and 'shunt' queue directories and,
if mm_cfg.BAD_SHUNT_STALE_AFTER is > 0, it removes all files more than
that many seconds old.

If mm_cfg.BAD_SHUNT_ARCHIVE_DIRECTORY is a writable directory, the old
files are moved there. Otherwise they are deleted.

Only regular files immediately subordinate to the 'bad' and 'shunt'
directories are processed. Anything else is skipped.

Usage: %(PROGRAM)s [options]

Options:
    -h / --help
        Print this message and exit.
 Dump the contents of any Mailman `database' file.

Usage: %(PROGRAM)s [options] filename

Options:

    --marshal/-m
        Assume the file contains a Python marshal, overridding any automatic
        guessing.

    --pickle/-p
        Assume the file contains a Python pickle, overridding any automatic
        guessing.

    --noprint/-n
        Don't attempt to pretty print the object.  This is useful if there's
        some problem with the object and you just want to get an unpickled
        representation.  Useful with `python -i bin/dumpdb <file>'.  In that
        case, the root of the tree will be left in a global called "msg".

    --help/-h
        Print this help message and exit

If the filename ends with `.db', then it is assumed that the file contains a
Python marshal.  If the file ends with `.pck' then it is assumed to contain a
Python pickle.  In either case, if you want to override the default assumption
-- or if the file ends in neither suffix -- use the -p or -m flags.
 Finalizing Fix the MM2.1b4 archives.

Usage: %(PROGRAM)s [options] file ...

Where options are:
    -h / --help
        Print this help message and exit.

Only use this to `fix' some archive database files that may have gotten
written in Mailman 2.1b4 with some bogus data.  Use like this from your
$PREFIX directory

%% %(PROGRAM)s `grep -l _mlist archives/private/*/database/*-article`

(note the backquotes are required)

You will need to run `bin/check_perms -f' after running this script.
 Full Personalization Fundamental list characteristics, including descriptive
            info and basic behaviors. Generate binary message catalog from textual translation description.

This program converts a textual Uniforum-style message catalog (.po file) into
a binary GNU catalog (.mo file).  This is essentially the same function as the
GNU msgfmt program, however, it is a simpler implementation.

Usage: msgfmt.py [OPTIONS] filename.po

Options:
    -o file
    --output-file=file
        Specify the output file to write to.  If omitted, output will go to a
        file named filename.mo (based off the input file name).

    -h
    --help
        Print this message and exit.

    -V
    --version
        Display version information and exit.
 Last autoresponse notification for today Leave the initial password (and confirmation) fields
                blank if you want Mailman to autogenerate the list
                passwords. Mailman prefixes <tt>Subject:</tt> headers with
             <a href="?VARHELP=general/subject_prefix">text you can
             customize</a> and normally, this prefix shows up in messages
             gatewayed to Usenet.  You can set this option to <em>No</em> to
             disable the prefix on gated messages.  Of course, if you turn off
             normal <tt>Subject:</tt> prefixes, they won't be prefixed for
             gated messages either. Mass catchup completed Move a message from the shunt queue to the original queue.

Usage: %(PROGRAM)s [options] [directory]

Where:

    -h / --help
        Print help and exit.

Optional `directory' specifies a directory to dequeue from other than
qfiles/shunt.  *** Warning ***  Do not unshunt messages that weren't
shunted to begin with.  For example, running unshunt on qfiles/out/
will result in losing all the messages in that queue.
 Natural language (internationalization) options. Normally, Mailman sends the regular delivery messages to
                 the mail server in batches.  This is much more efficent
                 because it reduces the amount of traffic between Mailman and
                 the mail server.

                 <p>However, some lists can benefit from a more personalized
                 approach.  In this case, Mailman crafts a new message for
                 each member on the regular delivery list.  Turning this
                 feature on may degrade the performance of your site, so you
                 need to carefully consider whether the trade-off is worth it,
                 or whether there are other ways to accomplish what you want.
                 You should also carefully monitor your system load to make
                 sure it is acceptable.

                 <p>Select <em>No</em> to disable personalization and send
                 messages to the members in batches.  Select <em>Yes</em> to
                 personalize deliveries and allow additional substitution
                 variables in message headers and footers (see below).  In
                 addition, by selecting <em>Full Personalization</em>, the
                 <code>To</code> header of posted messages will be modified to
                 include the member's address instead of the list's posting
                 address.

                 <p>When personalization is enabled, a few more expansion
                 variables can be included in the <a
                 href="?VARHELP=nondigest/msg_header">message header</a> and
                 <a href="?VARHELP=nondigest/msg_footer">message footer</a>.

                 <p>These additional substitution variables will be available
                 for your headers and footers, when this feature is enabled:

                 <ul><li><b>user_address</b> - The address of the user,
                         coerced to lower case.
                     <li><b>user_delivered_to</b> - The case-preserved address
                         that the user is subscribed with.
                     <li><b>user_password</b> - The user's password.
                     <li><b>user_name</b> - The user's full name.
                     <li><b>user_optionsurl</b> - The url to the user's option
                         page.
                 </ul>
                  Posted message canceled Re-generate the Pipermail gzip'd archive flat files.

This script should be run nightly from cron.  When run from the command line,
the following usage is understood:

Usage: %(program)s [-v] [-h] [listnames]

Where:
    --verbose
    -v
        print each file as it's being gzip'd

    --help
    -h
        print this message and exit

    listnames
        Optionally, only compress the .txt files for the named lists.  Without 
        this, all archivable lists are processed.

 Run one or more qrunners, once or repeatedly.

Each named runner class is run in round-robin fashion.  In other words, the
first named runner is run to consume all the files currently in its
directory.  When that qrunner is done, the next one is run to consume all the
files in /its/ directory, and so on.  The number of total iterations can be
given on the command line.

Usage: %(PROGRAM)s [options]

Options:

    -r runner[:slice:range]
    --runner=runner[:slice:range]
        Run the named qrunner, which must be one of the strings returned by
        the -l option.  Optional slice:range if given, is used to assign
        multiple qrunner processes to a queue.  range is the total number of
        qrunners for this queue while slice is the number of this qrunner from
        [0..range).

        If using the slice:range form, you better make sure that each qrunner
        for the queue is given the same range value.  If slice:runner is not
        given, then 1:1 is used.

        Multiple -r options may be given, in which case each qrunner will run
        once in round-robin fashion.  The special runner `All' is shorthand
        for a qrunner for each listed by the -l option.

    --once
    -o
        Run each named qrunner exactly once through its main loop.  Otherwise,
        each qrunner runs indefinitely, until the process receives a SIGTERM
        or SIGINT.

    -l/--list
        Shows the available qrunner names and exit.

    -v/--verbose
        Spit out more debugging information to the logs/qrunner log file.

    -s/--subproc
        This should only be used when running qrunner as a subprocess of the
        mailmanctl startup script.  It changes some of the exit-on-error
        behavior to work better with that framework.

    -h/--help
        Print this message and exit.

runner is required unless -l or -h is given, and it must be one of the names
displayed by the -l switch.

Note also that this script should be started up from mailmanctl as a normal
operation.  It is only useful for debugging if it is run separately.
 Set the site password, prompting from the terminal.

The site password can be used in most if not all places that the list
administrator's password can be used, which in turn can be used in most places
that a list users password can be used.

Usage: %(PROGRAM)s [options] [password]

Options:

    -c/--listcreator
        Set the list creator password instead of the site password.  The list
        creator is authorized to create and remove lists, but does not have
        the total power of the site administrator.

    -h/--help
        Print this help message and exit.

If password is not given on the command line, it will be prompted for.
 Setting this option causes member email addresses to be
             transformed when they are presented on list web pages (both in
             text and as links), so they're not trivially recognizable as
             email addresses.  The intention is to prevent the addresses
             from being snarfed up by automated web scanners for use by
             spammers. Several protocols now in wide use attempt to ensure that use
             of the domain in the author's address (ie, in the From: header
             field) is authorized by that domain.  These protocols may be
             incompatible with common list features such as footers, causing
             participating email services to bounce list traffic merely
             because of the address in the From: field.  <b>This has resulted
             in members being unsubscribed despite being perfectly able to
             receive mail.</b>
             <p>
             The following actions are applied to all list messages when
             selected here.  To apply these actions only to messages where the
             domain in the From: header is determined to use such a protocol,
             see the <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a> settings under Privacy options...
             -&gt; Sender filters.
             <p>Settings:<p>
             <dl>
             <dt>No</dt>
             <dd>Do nothing special.  This is appropriate for anonymous lists.
             It is appropriate for dedicated announcement lists, unless the
             From: address of authorized posters might be in a domain with a
             DMARC or similar policy. It is also appropriate if you choose to
             use dmarc_moderation_action other than Accept for this list.</dd>
             <dt>Munge From</dt>
             <dd>This action replaces the poster's address in the From: header
             with the list's posting address and adds the poster's address to
             the addresses in the original Reply-To: header.</dd>
             <dt>Wrap Message</dt>
             <dd>Just wrap the message in an outer message with the From:
             header containing the list's posting address and with the original
             From: address added to the addresses in the original Reply-To:
             header and with Content-Type: message/rfc822.  This is effectively
             a one message MIME format digest.</dd>
             </dl>
             <p>The transformations for anonymous_list are applied before
             any of these actions. It is not useful to apply actions other
             than No to an anonymous list, and if you do so, the result may
             be surprising.
             <p>The Reply-To: header munging actions below interact with these
             actions as follows:
             <p> first_strip_reply_to = Yes will remove all the incoming
             Reply-To: addresses but will still add the poster's address to
             Reply-To: for all three settings of reply_goes_to_list which
             respectively will result in just the poster's address, the
             poster's address and the list posting address or the poster's
             address and the explicit reply_to_address in the outgoing
             Reply-To: header. If first_strip_reply_to = No the poster's
             address in the original From: header, if not already included in
             the Reply-To:, will be added to any existing Reply-To:
             address(es).
             <p>These actions, whether selected here or via <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a>, do not apply to messages in digests
             or archives or sent to usenet via the Mail&lt;-&gt;News gateways.
             <p>If <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a> applies to this message with an
             action other than Accept, that action rather than this is
             applied Should Mailman perform a <em>catchup</em> on the newsgroup? Should Mailman personalize each non-digest delivery?
                 This is often useful for announce-only lists, but <a
                 href="?VARHELP=nondigest/personalize">read the details</a>
                 section for a discussion of important performance
                 issues. Show member addresses so they're not directly recognizable
             as email addresses? Synchronize a mailing list's membership with a flat file.

This script is useful if you have a Mailman mailing list and a sendmail
:include: style list of addresses (also as is used in Majordomo).  For every
address in the file that does not appear in the mailing list, the address is
added.  For every address in the mailing list that does not appear in the
file, the address is removed.  Other options control what happens when an
address is added or removed.

Usage: %(PROGRAM)s [options] -f file listname

Where `options' are:

    --no-change
    -n
        Don't actually make the changes.  Instead, print out what would be
        done to the list.

    --welcome-msg[=<yes|no>]
    -w[=<yes|no>]
        Sets whether or not to send the newly added members a welcome
        message, overriding whatever the list's `send_welcome_msg' setting
        is.  With -w=yes or -w, the welcome message is sent.  With -w=no, no
        message is sent.

    --goodbye-msg[=<yes|no>]
    -g[=<yes|no>]
        Sets whether or not to send the goodbye message to removed members,
        overriding whatever the list's `send_goodbye_msg' setting is.  With
        -g=yes or -g, the goodbye message is sent.  With -g=no, no message is
        sent.

    --digest[=<yes|no>]
    -d[=<yes|no>]
        Selects whether to make newly added members receive messages in
        digests.  With -d=yes or -d, they become digest members.  With -d=no
        (or if no -d option given) they are added as regular members.

    --notifyadmin[=<yes|no>]
    -a[=<yes|no>]
        Specifies whether the admin should be notified for each subscription
        or unsubscription.  If you're adding a lot of addresses, you
        definitely want to turn this off!  With -a=yes or -a, the admin is
        notified.  With -a=no, the admin is not notified.  With no -a option,
        the default for the list is used.

    --file <filename | ->
    -f <filename | ->
        This option is required.  It specifies the flat file to synchronize
        against.  Email addresses must appear one per line.  If filename is
        `-' then stdin is used.

    --help
    -h
        Print this message.

    listname
        Required.  This specifies the list to synchronize.
 The attached message was received as a bounce, but either the bounce format
was not recognized, or no member addresses could be extracted from it.  This
mailing list has been configured to send all unrecognized bounce messages to
the list administrator(s).

For more information see:
%(adminurl)s

 The capitalization of this name can be changed to make it
             presentable in polite company as a proper noun, or to make an
             acronym part all upper case, etc.  However, the name will be
             advertised as the email address (e.g., in subscribe confirmation
             notices), so it should <em>not</em> be otherwise altered.  (Email
             addresses are not case sensitive, but they are sensitive to
             almost everything else :-) The text will be treated as html <em>except</em> that
             newlines will be translated to &lt;br&gt; - so you can use links,
             preformatted text, etc, but don't put in carriage returns except
             where you mean to separate paragraphs.  And review your changes -
             bad html (like some unterminated HTML constructs) can prevent
             display of the entire listinfo page. The topic filter categorizes each incoming email message
             according to <a
            href="http://docs.python.org/library/re.html">regular
             expression filters</a> you specify below.  If the message's
             <code>Subject:</code> or <code>Keywords:</code> header contains a
             match against a topic filter, the message is logically placed
             into a topic <em>bucket</em>.  Each user can then choose to only
             receive messages from the mailing list for a particular topic
             bucket (or buckets).  Any message not categorized in a topic
             bucket registered with the user is not delivered to the list.

             <p>Note that this feature only works with regular delivery, not
             digest delivery.

             <p>The body of the message can also be optionally scanned for
             <code>Subject:</code> and <code>Keywords:</code> headers, as
             specified by the <a
       href="?VARHELP=topics/topics_bodylines_limit">topics_bodylines_limit</a>
             configuration variable. This page allows you as the list owner, to permanently
    remove this mailing list from the system.  <strong>This action is not
    undoable</strong> so you should undertake it only if you are absolutely
    sure this mailing list has served its purpose and is no longer necessary.

    <p>Note that no warning will be sent to your list members and after this
    action, any subsequent messages sent to the mailing list, or any of its
    administrative addreses will bounce.

    <p>You also have the option of removing the archives for this mailing list
    at this time.  It is almost always recommended that you do
    <strong>not</strong> remove the archives, since they serve as the
    historical record of your mailing list.

    <p>For your safety, you will be asked to reconfirm the list password.
     This value may be either the name of your news server, or
             optionally of the format name:port, where port is a port number.

             The news server is not part of Mailman proper.  You have to
             already have access to an NNTP server, and that NNTP server must
             recognize the machine this mailing list runs on as a machine
             capable of reading and posting news. Turn this on if you want password reminders to be sent once
             per month to your members.  Note that members may disable their
             own individual password reminders. Warning: Private archive directory is other-executable (o+x).
         This could allow other users on your system to read private archives.
         If you're on a shared multiuser system, you should consult the
         installation manual on how to fix this. When <a href="?VARHELP=nondigest/personalize">personalization</a> is enabled
for this list, additional substitution variables are allowed in your headers
and footers:

<ul><li><b>user_address</b> - The address of the user,
        coerced to lower case.
    <li><b>user_delivered_to</b> - The case-preserved address
        that the user is subscribed with.
    <li><b>user_password</b> - The user's password.
    <li><b>user_name</b> - The user's full name.
    <li><b>user_optionsurl</b> - The url to the user's option
        page.
</ul>
 When this option is enabled, all list traffic is emergency
             moderated, i.e. held for moderation.  Turn this option on when
             your list is experiencing a flamewar and you want a cooling off
             period. When you tell Mailman to perform a catchup on the newsgroup,
             this means that you want to start gating messages to the mailing
             list with the next new message found.  All earlier messages on
             the newsgroup will be ignored.  This is as if you were reading
             the newsgroup yourself, and you marked all current messages as
             <em>read</em>.  By catching up, your mailing list members will
             not see any of the earlier messages. You are not authorized to create new mailing lists You are not authorized to delete this mailing list You can create a new mailing list by entering the
    relevant information into the form below.  The name of the mailing list
    will be used as the primary address for posting messages to the list, so
    it should be lowercased.  You will not be able to change this once the
    list is created.

    <p>You also need to enter the email address of the initial list owner.
    Once the list is created, the list owner will be given notification, along
    with the initial list password.  The list owner will then be able to
    modify the password and add or remove additional list owners.

    <p>If you want Mailman to automatically generate the initial list admin
    password, click on `Yes' in the autogenerate field below, and leave the
    initial list password fields empty.

    <p>You must have the proper authorization to create new mailing lists.
    Each site should have a <em>list creator's</em> password, which you can
    enter in the field at the bottom.  Note that the site administrator's
    password can also be used for authentication.
     You have canceled the re-enabling of your membership.  If
    we continue to receive bounces from your address, it could be deleted from
    this mailing list. You have canceled your change of address request. You have canceled your subscription request. You have canceled your unsubscription request. You invited %(address)s to your list, but in a
deliberate malicious attempt, they tried to confirm the invitation to a
different list.  We just thought you'd like to know.  No further action by you
is required. You must confirm your unsubscription request by turning
                on the checkbox below the <em>Unsubscribe</em> button.  You
                have not been unsubscribed! Your confirmation is required in order to complete the
    subscription request to the mailing list <em>%(listname)s</em>.  Your
    subscription settings are shown below; make any necessary changes and hit
    <em>Subscribe</em> to complete the confirmation process.  Once you've
    confirmed your subscription request, you will be shown your account
    options page which you can use to further customize your membership
    options.

    <p>Note: your password will be emailed to you once your subscription is
    confirmed.  You can change it by visiting your personal options page.

    <p>Or hit <em>Cancel my subscription request</em> if you no longer want to
    subscribe to this list. Your urgent message to the %(realname)s mailing list was not authorized for
delivery.  The original message as received by Mailman is attached.
 Project-Id-Version: mailman
Report-Msgid-Bugs-To: FULL NAME <EMAIL@ADDRESS>
PO-Revision-Date: 2014-08-03 09:58+0000
Last-Translator: Jared Norris <jarednorris@ubuntu.com>
Language-Team: English (Australia) <en_AU@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Launchpad-Export-Date: 2018-07-12 13:21+0000
X-Generator: Launchpad (build 18719)
 
Check a given Mailman translation, making sure that variables and
tags referenced in translation are the same variables and tags in
the original templates and catalogue.

Usage:

cd $MAILMAN_DIR
%(program)s [-q] <lang>

Where <lang> is your country code (e.g. 'it' for Italy) and -q is
to ask for a brief summary.
             You have successfully cancelled the posting of your message with
            the Subject: header <em>%(subject)s</em> to the mailing list
            %(listname)s.  (but we obscure the addresses so they are not
                easily recognisable by spammers).  (requires authorisation) ... after this one. ... before this one. ... or specify a file to upload: Authorisation failed. Clean up an .mbox archive file.

The archiver looks for Unix-From lines separating messages in an mbox archive
file.  For compatibility, it specifically looks for lines that start with
"From " -- i.e. the letters capital-F, lower-case, o, m, space, ignoring
everything else on the line.

Normally, any lines that start "From " in the body of a message should be
escaped such that a > character is actually the first on a line.  It is
possible though that body lines are not actually escaped.  This script
attempts to fix these by doing a stricter test of the Unix-From lines.  Any
lines that start "From " but do not pass this stricter test are escaped with a
> character.

Usage: cleanarch [options] < inputfile > outputfile
Options:
    -s n
    --status=n
        Print a # character every n lines processed

    -q / --quiet
        Don't print changed line information to standard error.

    -n / --dry-run
        Don't actually output anything.

    -h / --help
        Print this message and exit
 Cull bad and shunt queues, recommended once per day.

This script goes through the 'bad' and 'shunt' queue directories and,
if mm_cfg.BAD_SHUNT_STALE_AFTER is > 0, it removes all files more than
that many seconds old.

If mm_cfg.BAD_SHUNT_ARCHIVE_DIRECTORY is a writeable directory, the old
files are moved there. Otherwise they are deleted.

Only regular files immediately subordinate to the 'bad' and 'shunt'
directories are processed. Anything else is skipped.

Usage: %(PROGRAM)s [options]

Options:
    -h / --help
        Print this message and exit.
 Dump the contents of any Mailman `database' file.

Usage: %(PROGRAM)s [options] filename

Options:

    --marshal/-m
        Assume the file contains a Python marshal, overriding any automatic
        guessing.

    --pickle/-p
        Assume the file contains a Python pickle, overriding any automatic
        guessing.

    --noprint/-n
        Don't attempt to pretty print the object.  This is useful if there's
        some problem with the object and you just want to get an unpickled
        representation.  Useful with `python -i bin/dumpdb <file>'.  In that
        case, the root of the tree will be left in a global called "msg".

    --help/-h
        Print this help message and exit

If the filename ends with `.db', then it is assumed that the file contains a
Python marshal.  If the file ends with `.pck' then it is assumed to contain a
Python pickle.  In either case, if you want to override the default assumption
-- or if the file ends in neither suffix -- use the -p or -m flags.
 Finalising Fix the MM2.1b4 archives.

Usage: %(PROGRAM)s [options] file ...

Where options are:
    -h / --help
        Print this help message and exit.

Only use this to `fix' some archive database files that may have gotten
written in Mailman 2.1b4 with some bogus data.  Use like this from your
$PREFIX directory

%% %(PROGRAM)s `grep -l _mlist archives/private/*/database/*-article`

(note the back-quotes are required)

You will need to run `bin/check_perms -f' after running this script.
 Full Personalisation Fundamental list characteristics, including descriptive
            info and basic behaviours. Generate binary message catalogue from textual translation description.

This program converts a textual Uniforum-style message catalogue (.po file) into
a binary GNU catalogue (.mo file).  This is essentially the same function as the
GNU msgfmt program, however, it is a simpler implementation.

Usage: msgfmt.py [OPTIONS] filename.po

Options:
    -o file
    --output-file=file
        Specify the output file to write to.  If omitted, output will go to a
        file named filename.mo (based off the input file name).

    -h
    --help
        Print this message and exit.

    -V
    --version
        Display version information and exit.
 Last auto-response notification for today Leave the initial password (and confirmation) fields
                blank if you want Mailman to auto-generate the list
                passwords. Mailman prefixes <tt>Subject:</tt> headers with
             <a href="?VARHELP=general/subject_prefix">text you can
             customise</a> and normally, this prefix shows up in messages
             gatewayed to Usenet.  You can set this option to <em>No</em> to
             disable the prefix on gated messages.  Of course, if you turn off
             normal <tt>Subject:</tt> prefixes, they won't be prefixed for
             gated messages either. Mass catch-up completed Move a message from the shunt queue to the original queue.

Usage: %(PROGRAM)s [options] [directory]

Where:

    -h / --help
        Print help and exit.

Optional `directory' specifies a directory to de-queue from other than
qfiles/shunt.  *** Warning ***  Do not unshunt messages that weren't
shunted to begin with.  For example, running unshunt on qfiles/out/
will result in losing all the messages in that queue.
 Natural language (internationalisation) options. Normally, Mailman sends the regular delivery messages to
                 the mail server in batches.  This is much more efficent
                 because it reduces the amount of traffic between Mailman and
                 the mail server.

                 <p>However, some lists can benefit from a more personalised
                 approach.  In this case, Mailman crafts a new message for
                 each member on the regular delivery list.  Turning this
                 feature on may degrade the performance of your site, so you
                 need to carefully consider whether the trade-off is worth it,
                 or whether there are other ways to accomplish what you want.
                 You should also carefully monitor your system load to make
                 sure it is acceptable.

                 <p>Select <em>No</em> to disable personalisation and send
                 messages to the members in batches.  Select <em>Yes</em> to
                 personalise deliveries and allow additional substitution
                 variables in message headers and footers (see below).  In
                 addition, by selecting <em>Full Personalisation</em>, the
                 <code>To</code> header of posted messages will be modified to
                 include the member's address instead of the list's posting
                 address.

                 <p>When personalisation is enabled, a few more expansion
                 variables can be included in the <a
                 href="?VARHELP=nondigest/msg_header">message header</a> and
                 <a href="?VARHELP=nondigest/msg_footer">message footer</a>.

                 <p>These additional substitution variables will be available
                 for your headers and footers, when this feature is enabled:

                 <ul><li><b>user_address</b> - The address of the user,
                         coerced to lower case.
                     <li><b>user_delivered_to</b> - The case-preserved address
                         that the user is subscribed with.
                     <li><b>user_password</b> - The user's password.
                     <li><b>user_name</b> - The user's full name.
                     <li><b>user_optionsurl</b> - The url to the user's option
                         page.
                 </ul>
                  Posted message cancelled Re-generate the Pipermail gzip'd archive flat files.

This script should be run nightly from cron.  When run from the command line,
the following usage is understood:

Usage: %(program)s [-v] [-h] [listnames]

Where:
    --verbose
    -v
        print each file as it's being gzip'd

    --help
    -h
        print this message and exit

    listnames
        Optionally, only compress the .txt files for the named lists.  Without 
        this, all archiveable lists are processed.

 Run one or more qrunners, once or repeatedly.

Each named runner class is run in round-robin fashion.  In other words, the
first named runner is run to consume all the files currently in its
directory.  When that qrunner is done, the next one is run to consume all the
files in /its/ directory, and so on.  The number of total iterations can be
given on the command line.

Usage: %(PROGRAM)s [options]

Options:

    -r runner[:slice:range]
    --runner=runner[:slice:range]
        Run the named qrunner, which must be one of the strings returned by
        the -l option.  Optional slice:range if given, is used to assign
        multiple qrunner processes to a queue.  range is the total number of
        qrunners for this queue while slice is the number of this qrunner from
        [0..range).

        If using the slice:range form, you better make sure that each qrunner
        for the queue is given the same range value.  If slice:runner is not
        given, then 1:1 is used.

        Multiple -r options may be given, in which case each qrunner will run
        once in round-robin fashion.  The special runner `All' is shorthand
        for a qrunner for each listed by the -l option.

    --once
    -o
        Run each named qrunner exactly once through its main loop.  Otherwise,
        each qrunner runs indefinitely, until the process receives a SIGTERM
        or SIGINT.

    -l/--list
        Shows the available qrunner names and exit.

    -v/--verbose
        Spit out more debugging information to the logs/qrunner log file.

    -s/--subproc
        This should only be used when running qrunner as a subprocess of the
        mailmanctl startup script.  It changes some of the exit-on-error
        behaviour to work better with that framework.

    -h/--help
        Print this message and exit.

runner is required unless -l or -h is given, and it must be one of the names
displayed by the -l switch.

Note also that this script should be started up from mailmanctl as a normal
operation.  It is only useful for debugging if it is run separately.
 Set the site password, prompting from the terminal.

The site password can be used in most if not all places that the list
administrator's password can be used, which in turn can be used in most places
that a list users password can be used.

Usage: %(PROGRAM)s [options] [password]

Options:

    -c/--listcreator
        Set the list creator password instead of the site password.  The list
        creator is authorised to create and remove lists, but does not have
        the total power of the site administrator.

    -h/--help
        Print this help message and exit.

If password is not given on the command line, it will be prompted for.
 Setting this option causes member email addresses to be
             transformed when they are presented on list web pages (both in
             text and as links), so they're not trivially recognisable as
             email addresses.  The intention is to prevent the addresses
             from being snarfed up by automated web scanners for use by
             spammers. Several protocols now in wide use attempt to ensure that use
             of the domain in the author's address (ie, in the From: header
             field) is authorised by that domain.  These protocols may be
             incompatible with common list features such as footers, causing
             participating email services to bounce list traffic merely
             because of the address in the From: field.  <b>This has resulted
             in members being unsubscribed despite being perfectly able to
             receive mail.</b>
             <p>
             The following actions are applied to all list messages when
             selected here.  To apply these actions only to messages where the
             domain in the From: header is determined to use such a protocol,
             see the <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a> settings under Privacy options...
             -&gt; Sender filters.
             <p>Settings:<p>
             <dl>
             <dt>No</dt>
             <dd>Do nothing special.  This is appropriate for anonymous lists.
             It is appropriate for dedicated announcement lists, unless the
             From: address of authorised posters might be in a domain with a
             DMARC or similar policy. It is also appropriate if you choose to
             use dmarc_moderation_action other than Accept for this list.</dd>
             <dt>Munge From</dt>
             <dd>This action replaces the poster's address in the From: header
             with the list's posting address and adds the poster's address to
             the addresses in the original Reply-To: header.</dd>
             <dt>Wrap Message</dt>
             <dd>Just wrap the message in an outer message with the From:
             header containing the list's posting address and with the original
             From: address added to the addresses in the original Reply-To:
             header and with Content-Type: message/rfc822.  This is effectively
             a one message MIME format digest.</dd>
             </dl>
             <p>The transformations for anonymous_list are applied before
             any of these actions. It is not useful to apply actions other
             than No to an anonymous list, and if you do so, the result may
             be surprising.
             <p>The Reply-To: header munging actions below interact with these
             actions as follows:
             <p> first_strip_reply_to = Yes will remove all the incoming
             Reply-To: addresses but will still add the poster's address to
             Reply-To: for all three settings of reply_goes_to_list which
             respectively will result in just the poster's address, the
             poster's address and the list posting address or the poster's
             address and the explicit reply_to_address in the outgoing
             Reply-To: header. If first_strip_reply_to = No the poster's
             address in the original From: header, if not already included in
             the Reply-To:, will be added to any existing Reply-To:
             address(es).
             <p>These actions, whether selected here or via <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a>, do not apply to messages in digests
             or archives or sent to usenet via the Mail&lt;-&gt;News gateways.
             <p>If <a
             href="?VARHELP=privacy/sender/dmarc_moderation_action">
             dmarc_moderation_action</a> applies to this message with an
             action other than Accept, that action rather than this is
             applied Should Mailman perform a <em>catch-up</em> on the newsgroup? Should Mailman personalise each non-digest delivery?
                 This is often useful for announce-only lists, but <a
                 href="?VARHELP=nondigest/personalize">read the details</a>
                 section for a discussion of important performance
                 issues. Show member addresses so they're not directly recognisable
             as email addresses? Synchronise a mailing list's membership with a flat file.

This script is useful if you have a Mailman mailing list and a sendmail
:include: style list of addresses (also as is used in Majordomo).  For every
address in the file that does not appear in the mailing list, the address is
added.  For every address in the mailing list that does not appear in the
file, the address is removed.  Other options control what happens when an
address is added or removed.

Usage: %(PROGRAM)s [options] -f file listname

Where `options' are:

    --no-change
    -n
        Don't actually make the changes.  Instead, print out what would be
        done to the list.

    --welcome-msg[=<yes|no>]
    -w[=<yes|no>]
        Sets whether or not to send the newly added members a welcome
        message, overriding whatever the list's `send_welcome_msg' setting
        is.  With -w=yes or -w, the welcome message is sent.  With -w=no, no
        message is sent.

    --goodbye-msg[=<yes|no>]
    -g[=<yes|no>]
        Sets whether or not to send the goodbye message to removed members,
        overriding whatever the list's `send_goodbye_msg' setting is.  With
        -g=yes or -g, the goodbye message is sent.  With -g=no, no message is
        sent.

    --digest[=<yes|no>]
    -d[=<yes|no>]
        Selects whether to make newly added members receive messages in
        digests.  With -d=yes or -d, they become digest members.  With -d=no
        (or if no -d option given) they are added as regular members.

    --notifyadmin[=<yes|no>]
    -a[=<yes|no>]
        Specifies whether the admin should be notified for each subscription
        or unsubscription.  If you're adding a lot of addresses, you
        definitely want to turn this off!  With -a=yes or -a, the admin is
        notified.  With -a=no, the admin is not notified.  With no -a option,
        the default for the list is used.

    --file <filename | ->
    -f <filename | ->
        This option is required.  It specifies the flat file to synchronise
        against.  Email addresses must appear one per line.  If filename is
        `-' then stdin is used.

    --help
    -h
        Print this message.

    listname
        Required.  This specifies the list to synchronise.
 The attached message was received as a bounce, but either the bounce format
was not recognised, or no member addresses could be extracted from it.  This
mailing list has been configured to send all unrecognised bounce messages to
the list administrator(s).

For more information see:
%(adminurl)s

 The capitalisation of this name can be changed to make it
             presentable in polite company as a proper noun, or to make an
             acronym part all upper case, etc.  However, the name will be
             advertised as the email address (e.g., in subscribe confirmation
             notices), so it should <em>not</em> be otherwise altered.  (Email
             addresses are not case sensitive, but they are sensitive to
             almost everything else :-) The text will be treated as html <em>except</em> that
             newlines will be translated to &lt;br&gt; - so you can use links,
             pre-formatted text, etc, but don't put in carriage returns except
             where you mean to separate paragraphs.  And review your changes -
             bad html (like some unterminated HTML constructs) can prevent
             display of the entire listinfo page. The topic filter categorises each incoming email message
             according to <a
            href="http://docs.python.org/library/re.html">regular
             expression filters</a> you specify below.  If the message's
             <code>Subject:</code> or <code>Keywords:</code> header contains a
             match against a topic filter, the message is logically placed
             into a topic <em>bucket</em>.  Each user can then choose to only
             receive messages from the mailing list for a particular topic
             bucket (or buckets).  Any message not categorised in a topic
             bucket registered with the user is not delivered to the list.

             <p>Note that this feature only works with regular delivery, not
             digest delivery.

             <p>The body of the message can also be optionally scanned for
             <code>Subject:</code> and <code>Keywords:</code> headers, as
             specified by the <a
       href="?VARHELP=topics/topics_bodylines_limit">topics_bodylines_limit</a>
             configuration variable. This page allows you as the list owner, to permanently
    remove this mailing list from the system.  <strong>This action is not
    undoable</strong> so you should undertake it only if you are absolutely
    sure this mailing list has served its purpose and is no longer necessary.

    <p>Note that no warning will be sent to your list members and after this
    action, any subsequent messages sent to the mailing list, or any of its
    administrative addresses will bounce.

    <p>You also have the option of removing the archives for this mailing list
    at this time.  It is almost always recommended that you do
    <strong>not</strong> remove the archives, since they serve as the
    historical record of your mailing list.

    <p>For your safety, you will be asked to reconfirm the list password.
     This value may be either the name of your news server, or
             optionally of the format name:port, where port is a port number.

             The news server is not part of Mailman proper.  You have to
             already have access to an NNTP server, and that NNTP server must
             recognise the machine this mailing list runs on as a machine
             capable of reading and posting news. Turn this on if you want password reminders to be sent once
             per month to your members. Note that members may disable their
             own individual password reminders. Warning: Private archive directory is other-executable (o+x).
         This could allow other users on your system to read private archives.
         If you're on a shared multi-user system, you should consult the
         installation manual on how to fix this. When <a href="?VARHELP=nondigest/personalize">personalisation</a> is enabled
for this list, additional substitution variables are allowed in your headers
and footers:

<ul><li><b>user_address</b> - The address of the user,
        coerced to lower case.
    <li><b>user_delivered_to</b> - The case-preserved address
        that the user is subscribed with.
    <li><b>user_password</b> - The user's password.
    <li><b>user_name</b> - The user's full name.
    <li><b>user_optionsurl</b> - The url to the user's option
        page.
</ul>
 When this option is enabled, all list traffic is emergency
             moderated, i.e. held for moderation.  Turn this option on when
             your list is experiencing a flame-war and you want a cooling off
             period. When you tell Mailman to perform a catch-up on the newsgroup,
             this means that you want to start gating messages to the mailing
             list with the next new message found.  All earlier messages on
             the newsgroup will be ignored.  This is as if you were reading
             the newsgroup yourself, and you marked all current messages as
             <em>read</em>.  By catching up, your mailing list members will
             not see any of the earlier messages. You are not authorised to create new mailing lists You are not authorised to delete this mailing list You can create a new mailing list by entering the
    relevant information into the form below.  The name of the mailing list
    will be used as the primary address for posting messages to the list, so
    it should be lower-cased.  You will not be able to change this once the
    list is created.

    <p>You also need to enter the email address of the initial list owner.
    Once the list is created, the list owner will be given notification, along
    with the initial list password.  The list owner will then be able to
    modify the password and add or remove additional list owners.

    <p>If you want Mailman to automatically generate the initial list admin
    password, click on `Yes' in the auto-generate field below, and leave the
    initial list password fields empty.

    <p>You must have the proper authorisation to create new mailing lists.
    Each site should have a <em>list creator's</em> password, which you can
    enter in the field at the bottom.  Note that the site administrator's
    password can also be used for authentication.
     You have cancelled the re-enabling of your membership.  If
    we continue to receive bounces from your address, it could be deleted from
    this mailing list. You have cancelled your change of address request. You have cancelled your subscription request. You have cancelled your unsubscription request. You invited %(address)s to your list, but in a
deliberate malicious attempt, they tried to confirm the invitation to a
different list.  We just thought you'd like to know. No further action by you
is required. You must confirm your unsubscription request by turning
                on the tickbox below the <em>Unsubscribe</em> button.  You
                have not been unsubscribed! Your confirmation is required in order to complete the
    subscription request to the mailing list <em>%(listname)s</em>.  Your
    subscription settings are shown below; make any necessary changes and hit
    <em>Subscribe</em> to complete the confirmation process.  Once you've
    confirmed your subscription request, you will be shown your account
    options page which you can use to further customise your membership
    options.

    <p>Note: your password will be emailed to you once your subscription is
    confirmed.  You can change it by visiting your personal options page.

    <p>Or hit <em>Cancel my subscription request</em> if you no longer want to
    subscribe to this list. Your urgent message to the %(realname)s mailing list was not authorised for
delivery.  The original message as received by Mailman is attached.
 