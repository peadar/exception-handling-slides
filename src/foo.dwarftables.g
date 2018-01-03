00000034 000000000000001c 00000000 CIE
  Version:               1
  Augmentation:          &quot;zPLR&quot;
  Code alignment factor: 1
  Data alignment factor: -8
  Return address column: 16
  Augmentation data:     03 00 00 00 00 03 1b

  DW_CFA_def_cfa: r7 (rsp) ofs 8
  DW_CFA_offset: r16 (rip) at cfa-8
  DW_CFA_nop
  DW_CFA_nop

...

0000008c 0000000000000024 0000005c FDE cie=00000034 pc=0000000000000030..0000000000000091

  Augmentation data:     00 00 00 00 - This is our LSDA data - filled in at link time
  DW_CFA_advance_loc: 1 to 0000000000000031
  DW_CFA_def_cfa_offset: 16
  DW_CFA_offset: r3 (rbx) at cfa-16
  DW_CFA_advance_loc: 4 to 0000000000000035
  DW_CFA_def_cfa_offset: 32
  DW_CFA_advance_loc: 54 to 000000000000006b
  DW_CFA_remember_state
  DW_CFA_def_cfa_offset: 16
  DW_CFA_advance_loc: 1 to 000000000000006c
  DW_CFA_def_cfa_offset: 8
  DW_CFA_advance_loc: 1 to 000000000000006d
  DW_CFA_restore_state
  DW_CFA_nop
  DW_CFA_nop
