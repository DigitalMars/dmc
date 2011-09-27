Sample: Demonstration of Code to Examine SD's (Security Descriptors)

Summary:  In the Win32 .hlp file, if you click on Search, goto
  "Security Overview", then choose from the list of topics under
  Security Overview the sub-topic "Allowing Access", you'll find
  the comment

    Note:  It is fine to write code like this that builds security
      descriptors from scratch.  It is, however, a good practice
      for people who write code that builds or manipulates
      security descriptors to first write code that explores the
      default security descriptors that Windows NT places on
      objects.  For example, if Windows NT by default includes in
      a DACL an ACE granting the Local Logon SID certain access,
      it's good to know that, so that a decision not to grant any
      access to the Local Logon SID would be a conscious decision

Purpose of this sample:  The comment in the .hlp file is accurate,
  however, for many people this task of examining the SD is easier
  if there is sample code to start from.  So, the purpose of this
  sample is to assist people by providing sample code people can
  start from as they examine SD(s).  This sample as is examines
  the SD on files, and this code can be modified to examine the SD
  on other objects

  This sample is not a supported utility

To run:  Type Check_sd to check the SD on the \\.\A:  device

         Type Check_sd d:\a.fil to check the SD on the d:\a.fil
           file.  In this case d: must be formatted NTFS,
           because only NTFS files have SD(s)

Further notes:

- If you recompile with this define set like this

  #define I_DO_NOT_WANT_THIS_CODE_TO_CLUTTER_THIS_PROGRAM_S_OUTPUT (1==0)

    and re-run the program, the program will produce a lot more
      output, including displaying all the info you can access in
      a Win32 program from the process's Access Token, and the
      SD's of some sample objects

  - If you logon, run with the program built to produce the extra
    output as mentioned just above, save that output to a file,
    then log off and re-run the program, save the output of this
    second run to a different file, you can with WinDiff easily
    observe how the local logon SID really does change values for
    each logged on session

- A sample test you could run to exercise DACLs involves using the
  \q_a\samples\sd_flppy sample in conjunction with this check_sd
  sample

  - Log on to a machine as a local Administrator
  - Do
    check_sd \\.\A: >out_bef.a
    check_sd \\.\B: >out_bef.b
  - Logoff
  - Log on the same machine as Guest on the local machine domain
  - Do
    sd_flppy
  - Try
    dir a:    (observe access denied)
    dir b:    (observe access denied)
    copy config.sys a:\   (get device not found)
    copy config.sys b:\   (get device not found)
  - Logoff
  - Log on the same machine as a local Administrator
  - Do
    check_sd \\.\A: >out_aft.a
    check_sd \\.\B: >out_aft.b
  - Browse the differences between out_bef.* and out_aft.*

- The above sample test demonstrates that the ACLs that sd_flppy
  applies survive logoffs.  To demonstrate the DACLs do not
  survive rebooting, simply reboot, log back on as a local
  Administrator, and
    check_sd \\.\A: >out_rbt.a
    check_sd \\.\B: >out_rbt.b
  to see the DACLs are again as they were in
                     out_bef.a
                     out_bef.b