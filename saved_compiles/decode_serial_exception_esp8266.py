#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# decode_serial_exception_esp8266.py
#
# Decode pasted ESP8266 serial exception dump using xtensa-lx106-elf-addr2line + firmware.elf
#
# Handles:
#   - ESP8266 "CUT HERE FOR EXCEPTION DECODER" dumps
#   - epc1/epc2/epc3/excvaddr/depc register line
#   - >>>stack>>> / <<<stack<<< blocks
#   - wrapped/split 8-digit hex values caused by serial copy/paste
#   - duplicated or malformed whitespace
#
# Date Modified: 26May26

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple


# ============================================================
# >>> PASTE YOUR FULL ESP8266 SERIAL EXCEPTION DUMP HERE <<<
# ============================================================

def _SerialCrashDumpBlock() -> str:
    return r"""
                                 
                         
             
--------------- CUT HERE FOR EXCEPTION DECODER ---------------

Unhandled C++ exception: OOM

>>>stack>>>

ctx: sys
sp: 3fffead0 end: 3fffffb0 offset: 0010
3fffeae0:  3fffee20 3fffee48 0000011c 4022af40  
3fffeaf0:  00000000 3fffee48 3fff2940 4022348e  
3fffeb00:  31623824 34323337 34662d64 652d6132  
3fffeb10:  2d383732 36373237 3832662d 34366232  
3fffeb20:  33346236 675f0b34 6c676f6f 6e6f7a65  
3fffeb30:  745f0465 6c057063 6c61636f 00000000  
3fffeb40:  00000000 00000000 00000000 00000000  
3fffeb50:  00000000 00000000 00000000 00000000  
3fffeb60:  00000000 00000000 00000000 00000000  
3fffeb70:  00000000 00000000 00000000 00000000  
3fffeb80:  00000000 00000000 00000000 00000000  
3fffeb90:  00000000 00000000 00000000 00000000  
3fffeba0:  00000000 00000000 00000000 00000000  
3fffebb0:  00000000 00000000 00000000 00000000  
3fffebc0:  00000000 00000000 00000000 00000000  
3fffebd0:  00000000 00000000 00000000 00000000  
3fffebe0:  00000000 00000000 00000000 00000000  
3fffebf0:  00000000 00000000 00000000 00000000  
3fffec00:  0010003d 00008001 00001194 3fff002c  
3fffec10:  3fffee20 00000001 3fff2940 40220a31  
3fffec20:  00000000 31623824 34323337 34662d64  
3fffec30:  652d6132 2d383732 36373237 3832662d  
3fffec40:  34366232 33346236 6f6c0534 006c6163  
3fffec50:  00000000 00000000 00000000 00000000  
3fffec60:  00000000 00000000 00000000 00000000  
3fffec70:  00000000 00000000 00000000 00000000  
3fffec80:  00000000 00000000 00000000 00000000  
3fffec90:  00000000 00000000 00000000 00000000  
3fffeca0:  00000000 00000000 00000000 00000000  
3fffecb0:  00000000 00000000 00000000 00000000  
3fffecc0:  00000000 00000000 00000000 00000000  
3fffecd0:  00000000 00000000 00000000 00000000  
3fffece0:  00000000 00000000 00000000 00000000  
3fffecf0:  00000000 00000000 00000000 00000000  
3fffed00:  00000000 00000000 00000000 00000000  
3fffed10:  00000000 00000000 00000000 00000000  
3fffed20:  00000000 00ff002c 00000001 40102924  
3fffed30:  3ffec102 40281aa3 3ffef820 3fff34f4  
3fffed40:  3fff31b4 3fff31e8 3fff34e8 40265164  
3fffed50:  00000005 00000000 00000020 40100490  
3fffed60:  3fffb5bc 401061ff 00000005 40102924  
3fffed70:  3ffec105 40105bbb 3ffefb68 3fffb60c  
3fffed80:  401034af 3ffefb68 3fff31b4 4026522c  
3fffed90:  000072c8 00000e59 3ffe86e8 40100d3e  
3fffeda0:  0000001a 3fff0bf0 00000020 3fff0560  
3fffedb0:  0000001a 00000020 3fffbf0c 4010108c  
3fffedc0:  402711f3 3fff0bf0 00000002 402711e0  
3fffedd0:  00000002 4027112f 00000002 40270284  
3fffede0:  402702ad 3fffee90 3fff0bf0 0000001a  
3fffedf0:  4026dd0c 3fffee90 3fff0a98 3fff049c  
3fffee00:  4026000a 3fffee90 3fffee90 00000030  
3fffee10:  6f720003 00000002 3fffaf54 40222f91  
3fffee20:  3fffeeaa 00000001 3fffaf54 40222f91  
3fffee30:  3fff0000 00000000 00000000 3f000001  
3fffee40:  3fff0000 00000000 00000000 4022329c  
3fffee50:  00000000 00000003 3fffeea0 40223605  
3fffee60:  00000000 005a623f 4026ea72 3fffb37c  
3fffee70:  3fffb3ae 00000000 3fff2940 40220d9d  
3fffee80:  00000000 00000000 00000000 00000000  
3fffee90:  00000000 00000000 00000000 4025f86a  
3fffeea0:  00000000 00000002 00000003 40221392  
3fffeeb0:  3fffb3ae 00000000 3fffac74 4026342c  
3fffeec0:  000014e9 3fff31b4 3fff31b4 3fffb1cc  
3fffeed0:  3fff0000 000000fe 00000020 3fff31b4  
3fffeee0:  3fffb37c 3fffb39a 3fff3418 40265ca0  
3fffeef0:  00000014 00000000 3fff31b4 40100d3e  
3fffef00:  40281c10 3fffaedc 3fff4ff4 3fffb1cc  
3fffef10:  3fff31b4 00000008 3fffb37c 4025ea4d  
3fffef20:  3fffdc80 3fff4ff4 3fffaedc 4025e870  
3fffef30:  40284dc1 3fff4ff4 3fffaedc 40284dd3  
3fffef40:  3fffb38c 3fffb37c 00000000 3fffdab0  
3fffef50:  4028183f 00000000 3fffaedc 402860ab  
3fffef60:  40000f49 3fffdab0 ffffff01 40000f49  
3fffef70:  40000e19 0009da0b bff00000 0000bfff  
3fffef80:  00000005 aa55aa55 0000004a 40105cbd  
3fffef90:  40105cc3 bff00000 0000bfff 635f5f37  
3fffefa0:  4010000d bff00000 0009da0b 401000ab  
3fffefb0:  00000000 3fffef4c 00000000 3ffffe08  
3fffefc0:  3fffffd0 00000000 00000000 feefeffe  
3fffefd0:  feefeffe feefeffe feefeffe feefeffe  
3fffefe0:  feefeffe feefeffe feefeffe feefeffe  
3fffeff0:  feefeffe feefeffe feefeffe feefeffe  
3ffff000:  feefeffe feefeffe feefeffe feefeffe  
3ffff010:  feefeffe feefeffe feefeffe feefeffe  
3ffff020:  feefeffe feefeffe feefeffe feefeffe  
3ffff030:  feefeffe feefeffe feefeffe feefeffe  
3ffff040:  feefeffe feefeffe feefeffe feefeffe  
3ffff050:  feefeffe feefeffe feefeffe feefeffe  
3ffff060:  feefeffe feefeffe feefeffe feefeffe  
3ffff070:  feefeffe feefeffe feefeffe feefeffe  
3ffff080:  feefeffe feefeffe feefeffe feefeffe  
3ffff090:  feefeffe feefeffe feefeffe feefeffe  
3ffff0a0:  feefeffe feefeffe feefeffe feefeffe  
3ffff0b0:  feefeffe feefeffe feefeffe feefeffe  
3ffff0c0:  feefeffe feefeffe feefeffe feefeffe  
3ffff0d0:  feefeffe feefeffe feefeffe feefeffe  
3ffff0e0:  feefeffe feefeffe feefeffe feefeffe  
3ffff0f0:  feefeffe feefeffe feefeffe feefeffe  
3ffff100:  feefeffe feefeffe feefeffe feefeffe  
3ffff110:  feefeffe feefeffe feefeffe feefeffe  
3ffff120:  feefeffe feefeffe feefeffe feefeffe  
3ffff130:  feefeffe feefeffe feefeffe feefeffe  
3ffff140:  feefeffe feefeffe feefeffe feefeffe  
3ffff150:  feefeffe feefeffe feefeffe feefeffe  
3ffff160:  feefeffe feefeffe feefeffe feefeffe  
3ffff170:  feefeffe feefeffe feefeffe feefeffe  
3ffff180:  feefeffe feefeffe feefeffe feefeffe  
3ffff190:  feefeffe feefeffe feefeffe feefeffe  
3ffff1a0:  feefeffe feefeffe feefeffe feefeffe  
3ffff1b0:  feefeffe feefeffe feefeffe feefeffe  
3ffff1c0:  feefeffe feefeffe feefeffe feefeffe  
3ffff1d0:  feefeffe feefeffe feefeffe feefeffe  
3ffff1e0:  feefeffe feefeffe feefeffe feefeffe  
3ffff1f0:  feefeffe feefeffe feefeffe feefeffe  
3ffff200:  feefeffe feefeffe feefeffe feefeffe  
3ffff210:  feefeffe feefeffe feefeffe feefeffe  
3ffff220:  feefeffe feefeffe feefeffe feefeffe  
3ffff230:  feefeffe feefeffe feefeffe feefeffe  
3ffff240:  feefeffe feefeffe feefeffe feefeffe  
3ffff250:  feefeffe feefeffe feefeffe feefeffe  
3ffff260:  feefeffe feefeffe feefeffe feefeffe  
3ffff270:  feefeffe feefeffe feefeffe feefeffe  
3ffff280:  feefeffe feefeffe feefeffe feefeffe  
3ffff290:  feefeffe feefeffe feefeffe feefeffe  
3ffff2a0:  feefeffe feefeffe feefeffe feefeffe  
3ffff2b0:  feefeffe feefeffe feefeffe feefeffe  
3ffff2c0:  feefeffe feefeffe feefeffe feefeffe  
3ffff2d0:  feefeffe feefeffe feefeffe feefeffe  
3ffff2e0:  feefeffe feefeffe feefeffe feefeffe  
3ffff2f0:  feefeffe feefeffe feefeffe feefeffe  
3ffff300:  feefeffe feefeffe feefeffe feefeffe  
3ffff310:  feefeffe feefeffe feefeffe feefeffe  
3ffff320:  feefeffe feefeffe feefeffe feefeffe  
3ffff330:  feefeffe feefeffe feefeffe feefeffe  
3ffff340:  feefeffe feefeffe feefeffe feefeffe  
3ffff350:  feefeffe feefeffe feefeffe feefeffe  
3ffff360:  feefeffe feefeffe feefeffe feefeffe  
3ffff370:  feefeffe feefeffe feefeffe feefeffe  
3ffff380:  feefeffe feefeffe feefeffe feefeffe  
3ffff390:  feefeffe feefeffe feefeffe feefeffe  
3ffff3a0:  feefeffe feefeffe feefeffe feefeffe  
3ffff3b0:  feefeffe feefeffe feefeffe feefeffe  
3ffff3c0:  feefeffe feefeffe feefeffe feefeffe  
3ffff3d0:  feefeffe feefeffe feefeffe feefeffe  
3ffff3e0:  feefeffe feefeffe feefeffe feefeffe  
3ffff3f0:  feefeffe feefeffe feefeffe feefeffe  
3ffff400:  feefeffe feefeffe feefeffe feefeffe  
3ffff410:  feefeffe feefeffe feefeffe feefeffe  
3ffff420:  feefeffe feefeffe feefeffe feefeffe  
3ffff430:  feefeffe feefeffe feefeffe feefeffe  
3ffff440:  feefeffe feefeffe feefeffe feefeffe  
3ffff450:  feefeffe feefeffe feefeffe feefeffe  
3ffff460:  feefeffe feefeffe feefeffe feefeffe  
3ffff470:  feefeffe feefeffe feefeffe feefeffe  
3ffff480:  00000000 00000000 0000000a 00000000  
3ffff490:  00000000 00000000 0000000a 3ffe83c4  
3ffff4a0:  00000000 00000000 3ffff610 4023c254  
3ffff4b0:  3fffa8e4 3ffff5a3 00000000 0000001c  
3ffff4c0:  00000000 feefeffe 00000000 3ffe83c4  
3ffff4d0:  00000032 3ffe83c4 3ffff610 4023c254  
3ffff4e0:  4023c190 3fffa8e4 00000032 feefeffe  
3ffff4f0:  feefeffe feefeffe 4023d5cd 3ffff610  
3ffff500:  4028e8e0 3ffe83c4 3ffff560 4023d6fc  
3ffff510:  00000032 3ffff5d2 00000001 3ffe83c4  
3ffff520:  00000002 3ffe83c4 3ffff610 4023c254  
3ffff530:  3ffff5d2 4028e8e0 00000002 4023d6fc  
3ffff540:  00000001 feefeffe feefeffe 4028e8e2  
3ffff550:  4028e8e0 3ffe83c4 3ffff610 4023c451  
3ffff560:  00000000 00000032 00000000 00000000  
3ffff570:  00000032 00000043 00302073 3ffe8f00  
3ffff580:  3ffe8f01 3ffe83c4 3ffff640 4023c6a8  
3ffff590:  00000004 ffffffff ffffffff 00000002  
3ffff5a0:  00000001 0000000a 00302075 0000001c  
3ffff5b0:  00001000 feefeffe 00000000 3ffe83c4  
3ffff5c0:  3ffff740 3ffff710 00000014 4023c254  
3ffff5d0:  3ffff740 3ffff710 00000008 00000032  
3ffff5e0:  00000002 feefeffe 00000000 3ffe83c4  
3ffff5f0:  3ffff6d0 3ffff6c0 00000024 3fff2bdc  
3ffff600:  3ffff710 3ffe83c4 00000044 40238dfd  
3ffff610:  3fffa95f feefeffe 00000000 ffff0208  
3ffff620:  3fffa91c 00000043 3ffff6e0 3fff2e00  
3ffff630:  00000019 3ffe83c4 00000019 402360b5  
3ffff640:  3ffff77a 00000032 0000000e ffff0208  
3ffff650:  3ffff770 00000018 3ffff6c3 00000000  
3ffff660:  00000000 00000000 3ffe86e4 3ffea592  
3ffff670:  3ffea593 3ffe83c4 3ffff730 4023c6a8  
3ffff680:  00005cd8 00000b9b 3ffe86e8 40100d3e  
3ffff690:  3fffa95f fffffffc 00000008 3fff2bdc  
3ffff6a0:  3ffff710 00000020 3fffa91c 4010108c  
3ffff6b0:  3ffff710 00000008 3fffa91c 40228b0d  
3ffff6c0:  30305430 3a30303a 57203030 53204e52  
3ffff6d0:  203a5445 70696b53 676e6970 74655320  
3ffff6e0:  676e6974 76615373 6c6c4165 6966202c  
3ffff6f0:  7973656c 6d657473 73696420 006c6261  
3ffff700:  3ffff740 3ffff710 00000008 feefef6e  
3ffff710:  00000000 feefeffe 3fff6d48 40202c60  
3ffff720:  3fff07d8 00000000 00000000 402360b5  
3ffff730:  3fff6d8c 4028e8d8 3ffff740 00000043 <
3ffff740:  00000019 3fff6060 00000001 4022e548  
3ffff750:  3fffa8e4 000000d4 00000003 402089c1  
3ffff760:  3ffff810 3ffff800 00000008 3fff2e00  
3ffff770:  30305430 3a30303a 00003030 00000000  
3ffff780:  00000000 00000000 00000000 40208588  
3ffff790:  3ffff810 3ffff800 00000008 3ffff810  
3ffff7a0:  3ffff800 00000008 00000003 3fff2e00  
3ffff7b0:  00005ca0 00000b94 3ffe86e8 40100d3e  
3ffff7c0:  00000000 00000003 3fffa8e4 3fff2e00  
3ffff7d0:  00000000 00000020 3fffa8e4 4010108c  
3ffff7e0:  feef07d6 00000009 3fff6d34 40202c54  
3ffff7f0:  00000004 00000000 0000000a 00000000  
3ffff800:  00000004 00000000 0000000a 3ffe83c4  
3ffff810:  00000000 00000000 3ffff980 4023c254  
3ffff820:  3fffab04 3ffff913 00000000 0000001c  
3ffff830:  00004000 feefeffe 00004000 3ffe83c4  
3ffff840:  00000010 3ffe83c4 3ffff980 4023c254  
3ffff850:  4023c190 3fffab04 00000010 feefeffe  
3ffff860:  feefeffe feefeffe 4023d5cd 3ffff980  
3ffff870:  4028e8e0 3ffe83c4 3ffff8d0 4023d6fc  
3ffff880:  00000010 3ffff942 00000001 3ffe83c4  
3ffff890:  00000002 3ffe83c4 3ffff980 4023c254  
3ffff8a0:  3ffff942 4028e8e0 00000002 4023d6fc  
3ffff8b0:  00000001 00000000 00000000 4028e8e2  
3ffff8c0:  4028e8e0 3ffe83c4 3ffff980 4023c451  
3ffff8d0:  00000000 00000010 00000000 00000000  
3ffff8e0:  00000010 00000021 3f302073 3ffe8f00  
3ffff8f0:  3ffe8f01 3ffe83c4 3ffff9b0 4023c6a8  
3ffff900:  00000004 ffffffff ffffffff 00000002  
3ffff910:  00000001 0000000a 3f302075 4023c254  
3ffff920:  4023c190 3ffffbb0 00000000 3ffff980  
3ffff930:  3ffffab0 3ffffa80 00000014 3ffffa50  
3ffff940:  3ffffab0 3ffffa80 00000008 00000010  
3ffff950:  00000002 3ffffa43 00000000 3ffe83c4  
3ffff960:  3ffffa40 3ffffa30 00000024 3fff2bdc  
3ffff970:  3ffffa80 3ffe83c4 00000040 40238dfd  
3ffff980:  3ffffa51 00000000 0000001e ffff0208  
3ffff990:  3ffffa30 0000003f 3ffffa50 000001f5  
3ffff9a0:  00000019 3ffe83c4 00000019 402360b5  
3ffff9b0:  3ffffaea 00000010 0000000e ffff0208  
3ffff9c0:  3ffffa01 00000018 00000001 00000000  
3ffff9d0:  3ffffa11 00000000 3ffe86e4 00000001  
3ffff9e0:  3fffafb4 40222954 00000020 00000000  
3ffff9f0:  00000000 00000001 00000001 40223525  
3ffffa00:  0000000c 3fff2940 3ffffea0 40223552  
3ffffa10:  0000020c 00000015 00000001 40223525  
3ffffa20:  3fffafb4 3ffffea0 3ffffea0 4022366a  
3ffffa30:  3fffafb4 00000000 3ffffea0 402238c1  
3ffffa40:  00000000 00000000 00000000 00000000  
3ffffa50:  00000000 00000000 00000000 00000000  
3ffffa60:  00000000 00000000 00000000 00000000  
3ffffa70:  00000000 00000000 00000000 00000000  
3ffffa80:  00000000 00000000 00000000 00000000  
3ffffa90:  00000000 00000000 00000000 00000000  
3ffffaa0:  00000000 00000000 00000000 00000000  
3ffffab0:  00000000 00000000 00000000 00000000  
3ffffac0:  00000000 00000000 00000000 00000000  
3ffffad0:  00000000 00000000 00000000 00000000  
3ffffae0:  00000000 00000000 00000000 00000000  
3ffffaf0:  00000000 00000000 00000000 00000000  
3ffffb00:  00000000 00000000 00000000 00000000  
3ffffb10:  00000000 00000000 00000000 00000000  
3ffffb20:  00000000 00000000 00000000 00000000  
3ffffb30:  00000000 00000000 00000000 00000000  
3ffffb40:  00000000 3ffffea0 3fff2940 402235a6  
3ffffb50:  00000001 0000001b 3ffffea0 4022369a  
3ffffb60:  00000001 3ffffea0 3ffffea0 3ffffddc  
3ffffb70:  00000001 3ffffea0 3fff2940 40223b8d  
3ffffb80:  00000000 00000000 00000000 00000000  
3ffffb90:  00000000 00000000 00000000 00000000  
3ffffba0:  00000000 00000000 00000000 00000000  
3ffffbb0:  00000000 00000000 00000000 00000000  
3ffffbc0:  00000000 00000000 00000000 00000000  
3ffffbd0:  00000000 00000000 40282993 00000001  
3ffffbe0:  ffffffff 00000000 3ffec251 00000008  
3ffffbf0:  402829e2 3ffef820 3fff4d4c 00000001  
3ffffc00:  00000002 00000000 00000020 40100490  
3ffffc10:  00000005 00000005 00000002 40102924  
3ffffc20:  3ffec102 40281aa3 3ffef820 3fffbf0c  
3ffffc30:  00000000 4026ac8b 3fff0bf0 3fff4d4c  
3ffffc40:  00000000 00000002 00000001 3ffef820  
3ffffc50:  3fffbf4e 401061ff 3fffb26c 3fff319c  
3ffffc60:  3fff4ff4 00000000 00000000 00000000  
3ffffc70:  00000000 4025e455 3fffb26c 3fff319c  
3ffffc80:  39020000 03320130 03383631 07323931  
3ffffc90:  612d6e69 04726464 61707261 3fff31b4  
3ffffca0:  3fff31b4 3fffbf0c 3fffbf0c 4025e687  
3ffffcb0:  3fff31e8 3fffbf0c 00000000 4025eada  
3ffffcc0:  3ffffcf0 00000000 00000000 00000000  
3ffffcd0:  00000000 00000000 00000000 3fffbf5c  
3ffffce0:  00000005 00000000 00000020 40100490  
3ffffcf0:  00000000 00000000 00000005 40102924  
3ffffd00:  00000005 00000000 00000020 40100490  
3ffffd10:  401034af 3ffefb68 00000005 40102924  
3ffffd20:  3ffec105 40105bbb 3ffefb68 4010368c  
3ffffd30:  00000005 00000000 00000020 40100490  
3ffffd40:  00000014 012202d3 00000005 40102924  
3ffffd50:  3ffec105 40105bbb 3ffefb68 00000001  
3ffffd60:  401034af 3ffefb68 40103b46 00000100  
3ffffd70:  00000014 01226fb1 3fff049c 4010368c  
3ffffd80:  3ffec998 00000000 00000000 40102924  
3ffffd90:  00000014 01226fb1 40103b46 00000100  
3ffffda0:  3ffec998 7fffffff 00002200 00000001  
3ffffdb0:  00000001 00006208 3fff049c 4010368c  
3ffffdc0:  3ffec998 00000000 00000000 01226fb1  
3ffffdd0:  3ffec9a4 00000001 3ffffecc 4022a492  
3ffffde0:  3fffaedc 3ffffe3e 00000022 00000001  
3ffffdf0:  40288ef3 00000030 00000010 3ffffeb0  
3ffffe00:  00000009 3ffffe3e 00000002 40224481  
3ffffe10:  00000009 3ffffe3e 0000000a 40228d80  
3ffffe20:  3fffaedc 3ffffe50 00000000 00000000  
3ffffe30:  3fffaedc 00000010 00000020 401010c0  
3ffffe40:  3fffaedc 00000010 3ffffef0 4022a0a6  
3ffffe50:  00000000 00000020 00000020 401010c0  
3ffffe60:  00006298 00000c53 3ffe86e8 40100d3e  
3ffffe70:  3fffaedc 0000000c 3ffffef0 00000000  
3ffffe80:  3ffffefc 00000020 3fffaedc 4010108c  
3ffffe90:  3ffffefc 3ffffef0 3ffffecc 40229ff1  
3ffffea0:  000062c0 00000c58 3ffe86e8 40100d3e  
3ffffeb0:  4022f25c 00000000 000003e8 00000000  
3ffffec0:  3ffea5c4 00000020 3fffaf04 4010108c  
3ffffed0:  3ffe9d58 00000000 3fff6d34 40202c54  
3ffffee0:  00000000 00004a5c c5604189 0055fb44  
3ffffef0:  3fff6d8c 000c000f 00000000 00000000  
3fffff00:  3fff6d84 00000032 3fff7ef8 4020793c  
3fffff10:  3fff6d84 00000019 3fff7ee4 4021b5c0  
3fffff20:  3fff6d70 00000000 3fff7ee4 4021b8ed  
3fffff30:  3fff6d80 00000001 00000015 40202507  
3fffff40:  3fff6d80 00000001 00000005 4021c531  
3fffff50:  3fff6d80 00000001 3fff6d40 40202c78  
3fffff60:  00000000 00004a5c 3fff6d34 40202c54  
3fffff70:  000007d1 00004a5c d53f7ced 0055fb56  
3fffff80:  00000000 00004a5c d2b020c4 3fff2e00  
3fffff90:  3fffdad0 000003e8 3fff5bd4 3fff2e00  
3fffffa0:  3fffdad0 00000000 3fff2dd4 3fff2e00  
<<<stack<<<

last failed alloc call: 4022348E(284)

--------------- CUT HERE FOR EXCEPTION DECODER ---------------





                
                                                                
              



""".strip()


# ============================================================
# CONFIGURATION
# ============================================================

ENV_NAME = "testbed__nodemcu__sonoff_basic"   # PlatformIO environment name
PROJECT_ROOT = None                           # None = auto-detect folder containing saved_compiles

DEDUPE_CONSECUTIVE_ONLY = False
INCLUDE_EPC_REGISTERS_IN_DECODE = True
INCLUDE_STACK_CODE_ADDRESSES_IN_DECODE = True

# Decode only likely ESP8266 code addresses from the stack.
# 0x402xxxxx = flash mapped code
# 0x401xxxxx = IRAM code
# 0x400xxxxx = ROM/SDK region, usually not useful with your firmware. Disabled by default.
DECODE_FLASH_CODE = True
DECODE_IRAM_CODE = True
DECODE_ROM_CODE = False


# ============================================================
# Basic helpers
# ============================================================

def _find_project_root(start: Path) -> Path:
    p = start.resolve()

    for _ in range(14):
        if (p / "saved_compiles").is_dir():
            return p

        if p.parent == p:
            break

        p = p.parent

    raise FileNotFoundError("Could not locate project root containing 'saved_compiles'.")


def _normalise_serial_text(text: str) -> str:
    """
    Make ESP8266 serial dumps parseable without destroying meaning.

    Main fixes:
      1. Remove whitespace inside broken 8-digit hex values:
           0x00000\n000 -> 0x00000000
           402\n17ccd    -> 40217ccd

      2. Add predictable spacing around stack markers.

      3. Collapse excessive whitespace without flattening stack rows.
    """

    if not text:
        return ""

    s = text.replace("\r", "\n")

    # Repair 0x-prefixed split hex values, e.g. 0x00000\n000 -> 0x00000000.
    split_0x_pattern = re.compile(
        r"0x([0-9a-fA-F]{1,7})\s+([0-9a-fA-F]{1,7})"
    )

    changed = True
    while changed:
        changed = False

        def _join_0x_if_total_8(match: re.Match) -> str:
            nonlocal changed
            a = match.group(1)
            b = match.group(2)

            if len(a) + len(b) == 8:
                changed = True
                return "0x" + a + b

            return match.group(0)

        s = split_0x_pattern.sub(_join_0x_if_total_8, s)

    # Repair bare split stack values, e.g. 402\n17ccd -> 40217ccd.
    # Keep this conservative: only join when total is exactly 8 hex chars.
    split_bare_pattern = re.compile(
        r"(?<![0-9a-fA-Fx])([0-9a-fA-F]{1,7})\s+([0-9a-fA-F]{1,7})(?![0-9a-fA-F])"
    )

    changed = True
    while changed:
        changed = False

        def _join_bare_if_total_8(match: re.Match) -> str:
            nonlocal changed
            a = match.group(1)
            b = match.group(2)

            if len(a) + len(b) == 8:
                changed = True
                return a + b

            return match.group(0)

        s = split_bare_pattern.sub(_join_bare_if_total_8, s)

    # Ensure adjacent 0x values are separated.
    s = re.sub(r"(0x[0-9a-fA-F]{8})(?=0x[0-9a-fA-F]{8})", r"\1 ", s)

    # Strip trailing spaces from each line and collapse excessive blank lines.
    lines = [line.strip() for line in s.split("\n")]
    s = "\n".join(lines)
    s = re.sub(r"\n{3,}", "\n\n", s)

    return s.strip()


def _extract_first_regex(pattern: str, text: str, flags: int = 0) -> Optional[str]:
    m = re.search(pattern, text, flags)
    if not m:
        return None
    return m.group(1)


def _hex_to_int_or_none(value: Optional[str]) -> Optional[int]:
    if not value:
        return None

    value = value.strip()

    try:
        if value.lower().startswith("0x"):
            return int(value, 16)
        return int(value, 16)
    except ValueError:
        return None


def _fmt_hex(value: Optional[int]) -> str:
    if value is None:
        return "N/A"
    return f"0x{value:08x}"


def _dedupe_consecutive(values: List[int]) -> List[int]:
    if not values:
        return []

    out = [values[0]]

    for x in values[1:]:
        if x != out[-1]:
            out.append(x)

    return out


def _dedupe_all_keep_order(values: List[int]) -> List[int]:
    seen = set()
    out = []

    for x in values:
        if x in seen:
            continue

        seen.add(x)
        out.append(x)

    return out


# ============================================================
# ESP8266 crash extraction
# ============================================================

def _extract_exception_number(text: str) -> Optional[int]:
    x = _extract_first_regex(r"\bException\s*\((\d+)\)\s*:", text, re.IGNORECASE)
    if x is None:
        return None

    try:
        return int(x)
    except ValueError:
        return None


def _extract_esp8266_registers(text: str) -> Dict[str, int]:
    """
    Extract ESP8266 exception registers:
      epc1 epc2 epc3 excvaddr depc
    """

    regs: Dict[str, int] = {}

    for name in ["epc1", "epc2", "epc3", "excvaddr", "depc"]:
        m = re.search(rf"\b{name}\s*=\s*(0x[0-9a-fA-F]{{8}})", text, re.IGNORECASE)
        if not m:
            continue

        value = _hex_to_int_or_none(m.group(1))
        if value is not None:
            regs[name.lower()] = value

    return regs


def _extract_stack_metadata(text: str) -> Dict[str, Optional[str]]:
    meta: Dict[str, Optional[str]] = {
        "ctx": None,
        "sp": None,
        "end": None,
        "offset": None,
    }

    ctx = _extract_first_regex(r"\bctx\s*:\s*([A-Za-z0-9_]+)", text, re.IGNORECASE)
    if ctx:
        meta["ctx"] = ctx

    m = re.search(
        r"\bsp\s*:\s*([0-9a-fA-F]{8})\s+end\s*:\s*([0-9a-fA-F]{8})\s+offset\s*:\s*([0-9a-fA-F]{4})",
        text,
        re.IGNORECASE,
    )

    if m:
        meta["sp"] = "0x" + m.group(1).lower()
        meta["end"] = "0x" + m.group(2).lower()
        meta["offset"] = "0x" + m.group(3).lower()

    return meta


def _extract_stack_block(text: str) -> str:
    m = re.search(r">>>stack>>>(.*?)<<<stack<<<", text, re.IGNORECASE | re.DOTALL)
    if not m:
        return ""

    return m.group(1)


def _extract_stack_words(text: str) -> List[Tuple[int, int]]:
    """
    Extract stack words.

    Returns list of:
      (stack_address, value)

    Example input row:
      3fffff60:  00000000 40217ccd 3fff93d4 402071ab
    """

    stack_block = _extract_stack_block(text)
    if not stack_block:
        return []

    words: List[Tuple[int, int]] = []

    for line in stack_block.splitlines():
        m = re.match(r"^\s*([0-9a-fA-F]{8})\s*:\s*(.*)$", line)
        if not m:
            continue

        try:
            row_address = int(m.group(1), 16)
        except ValueError:
            continue

        row_values = re.findall(r"\b([0-9a-fA-F]{8})\b", m.group(2))

        for i, raw_value in enumerate(row_values):
            try:
                value = int(raw_value, 16)
            except ValueError:
                continue

            word_address = row_address + (i * 4)
            words.append((word_address, value))

    return words


def _is_probable_code_address(value: int) -> bool:
    if DECODE_FLASH_CODE and 0x40200000 <= value <= 0x40300000:
        return True

    if DECODE_IRAM_CODE and 0x40100000 <= value <= 0x40200000:
        return True

    if DECODE_ROM_CODE and 0x40000000 <= value <= 0x40100000:
        return True

    return False


def _extract_stack_code_addresses(text: str) -> List[int]:
    words = _extract_stack_words(text)
    return [value for _, value in words if _is_probable_code_address(value)]


def _exception_meaning(exception_number: Optional[int], regs: Dict[str, int]) -> str:
    """
    ESP8266 exception cause quick interpretation.

    Exception (3) is LoadStoreError on Xtensa LX106 in common ESP8266 decoder output.
    Keep this intentionally practical rather than pretending every cause is certain from text alone.
    """

    excvaddr = regs.get("excvaddr")

    if exception_number is None:
        return "No ESP8266 exception number found."

    if exception_number == 0:
        return "Illegal instruction, often corrupted function pointer, bad return address, or invalid code execution."

    if exception_number == 3:
        if excvaddr == 0:
            return "Load/store error with null address involvement."
        return "Load/store error. Often invalid memory access, bad pointer, or access to an invalid region."

    if exception_number == 9:
        return "Unaligned memory access."

    if exception_number == 28:
        if excvaddr == 0:
            return "LoadProhibited-style invalid read, likely null pointer dereference."
        return "LoadProhibited-style invalid read."

    if exception_number == 29:
        if excvaddr == 0:
            return "StoreProhibited-style invalid write, likely null pointer dereference."
        return "StoreProhibited-style invalid write."

    return "ESP8266 exception cause extracted, but no specific interpretation is mapped in this script."


# ============================================================
# addr2line helpers
# ============================================================

def _auto_find_esp8266_addr2line() -> str:
    """
    Find xtensa-lx106-elf-addr2line from PlatformIO or PATH.
    """

    home = Path.home()

    exe_names = [
        "xtensa-lx106-elf-addr2line.exe",
        "xtensa-lx106-elf-addr2line",
    ]

    candidate_dirs = [
        home / ".platformio" / "packages" / "toolchain-xtensa" / "bin",
        home / ".platformio" / "packages" / "toolchain-xtensa-lx106" / "bin",
        home / ".platformio" / "packages" / "toolchain-xtensa-esp8266" / "bin",
    ]

    for d in candidate_dirs:
        for exe_name in exe_names:
            c = d / exe_name
            if c.is_file():
                return str(c)

    for p in os.environ.get("PATH", "").split(os.pathsep):
        for exe_name in exe_names:
            cand = Path(p) / exe_name
            if cand.is_file():
                return str(cand)

    raise FileNotFoundError("Could not find xtensa-lx106-elf-addr2line in PlatformIO packages or PATH.")


def _addr2line(addr2line_exe: str, elf_path: Path, pc: int) -> str:
    cmd = [
        addr2line_exe,
        "-f", "-C", "-p", "-i",
        "-e", str(elf_path),
        f"0x{pc:08x}",
    ]

    try:
        out = subprocess.check_output(cmd, stderr=subprocess.STDOUT)
        return out.decode("utf-8", errors="replace").strip()

    except subprocess.CalledProcessError as ex:
        return ex.output.decode("utf-8", errors="replace").strip()

    except Exception as ex:
        return f"addr2line failed: {ex}"


# ============================================================
# Print helpers
# ============================================================

def _print_summary(exception_number: Optional[int], regs: Dict[str, int], stack_meta: Dict[str, Optional[str]]) -> None:
    print("ESP8266 Exception Summary:")
    print(f"  Exception   : {exception_number if exception_number is not None else 'N/A'}")
    print(f"  Meaning     : {_exception_meaning(exception_number, regs)}")

    print("\nRegisters:")
    for name in ["epc1", "epc2", "epc3", "excvaddr", "depc"]:
        print(f"  {name:<8}: {_fmt_hex(regs.get(name))}")

    print("\nStack Metadata:")
    print(f"  ctx         : {stack_meta.get('ctx') or 'N/A'}")
    print(f"  sp          : {stack_meta.get('sp') or 'N/A'}")
    print(f"  end         : {stack_meta.get('end') or 'N/A'}")
    print(f"  offset      : {stack_meta.get('offset') or 'N/A'}")


def _print_decode_header(project_root: Path, elf_path: Path, addr2line_exe: str) -> None:
    print("\nDecode Config:")
    print(f"  ENV_NAME     : {ENV_NAME}")
    print(f"  PROJECT_ROOT : {project_root}")
    print(f"  FIRMWARE_ELF : {elf_path}")
    print(f"  ADDR2LINE    : {addr2line_exe}")


# ============================================================
# Main
# ============================================================

def main() -> int:
    raw = _SerialCrashDumpBlock()
    cleaned = _normalise_serial_text(raw)

    exception_number = _extract_exception_number(cleaned)
    regs = _extract_esp8266_registers(cleaned)
    stack_meta = _extract_stack_metadata(cleaned)
    stack_words = _extract_stack_words(cleaned)
    stack_code_addresses = _extract_stack_code_addresses(cleaned)

    decode_pcs: List[int] = []

    if INCLUDE_EPC_REGISTERS_IN_DECODE:
        for name in ["epc1", "epc2", "epc3", "depc"]:
            value = regs.get(name)

            if value is None:
                continue

            if value == 0:
                continue

            if not _is_probable_code_address(value):
                # Keep epc/depc printed in the register summary, but avoid noisy addr2line decode.
                continue

            decode_pcs.append(value)

    if INCLUDE_STACK_CODE_ADDRESSES_IN_DECODE:
        decode_pcs.extend(stack_code_addresses)

    if DEDUPE_CONSECUTIVE_ONLY:
        decode_pcs = _dedupe_consecutive(decode_pcs)
    else:
        decode_pcs = _dedupe_all_keep_order(decode_pcs)

    print("------------------------------------------------------------")
    print("ESP8266 Serial Exception Decoder")
    print("------------------------------------------------------------\n")

    print("Sanitised Crash Snippet:")
    print(cleaned)
    print("\n------------------------------------------------------------\n")

    _print_summary(exception_number, regs, stack_meta)

    print("\nStack Words:")
    if stack_words:
        for stack_address, value in stack_words:
            marker = " CODE" if _is_probable_code_address(value) else ""
            print(f"  0x{stack_address:08x}: 0x{value:08x}{marker}")
    else:
        print("  No stack words found.")

    print("\nDecode PCs:")
    if decode_pcs:
        for i, pc in enumerate(decode_pcs):
            print(f"  [{i:02d}] 0x{pc:08x}")
    else:
        print("  No likely firmware code addresses found for addr2line decode.")

    if not decode_pcs:
        print("\nNo addresses available for addr2line decode.")
        return 0

    try:
        script_path = Path(__file__).resolve()
        project_root = Path(PROJECT_ROOT).resolve() if PROJECT_ROOT else _find_project_root(script_path)

        elf_path = project_root / "saved_compiles" / ENV_NAME / "firmware.elf"
        if not elf_path.is_file():
            print(f"\nWARNING: Missing firmware ELF: {elf_path}")
            print("Crash parsing completed, but addr2line decode was skipped.")
            return 0

        addr2line_exe = _auto_find_esp8266_addr2line()

        _print_decode_header(project_root, elf_path, addr2line_exe)

        print("\nDecoded:")
        for pc in decode_pcs:
            line = _addr2line(addr2line_exe, elf_path, pc)
            print(f"  0x{pc:08x}: {line}")

    except Exception as ex:
        print(f"\nWARNING: addr2line decode skipped due to error: {ex}")
        print("Crash parsing still completed successfully.")
        return 0

    return 0


if __name__ == "__main__":
    sys.exit(main())
