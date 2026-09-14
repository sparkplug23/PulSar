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
                                 
                         
             
0T00:02:00 INF APP: Boot CountSUCCESSFUL BOOT 1 after 120 seconds
             
0T00:02:00 WRN SET: SettingsWrite skipped, TFS disabled
                       

                                                                               
 --------------- CUT HERE FOR EXCEPTION DECODER ---------------

              
                                                                                
Exception (29):
                                                               
epc1=0x40224556 epc2=0x00000000 epc3=0x00000000 excvaddr=0x00000008 depc=0x00000
000

                                                                         
                                                                                
>>>stack>>>

                                                                 
                                                                                
ctx: sys
                                                                      
sp: 3fffe8e0 end: 3fffffb0 offset: 0190
                                       
3fffea70:  3fffeaa0 76726573 3fffeba2 00000000  
                              
3fffea80:  3fffedc0 3fffede8 00000000 4022b3a8  
                              
3fffea90:  00000000 3fffede8 3fff2334 402257cc  
                              
3fffeaa0:  706f5f0f 61686e65 65732d62 72657672  
                              
3fffeab0:  63745f04 6f6c0570 006c6163 00000000  
                              
3fffeac0:  00000000 00000000 00000000 00000000  
                              
3fffead0:  00000000 00000000 00000000 00000000  
                              
3fffeae0:  00000000 00000000 00000000 00000000  
                              
3fffeaf0:  00000000 00000000 00000000 00000000  
                              
3fffeb00:  00000000 00000000 00000000 00000000  
                              
3fffeb10:  00000000 00000000 00000000 00000000  
                              
3fffeb20:  00000000 00000000 00000000 00000000  
                              
3fffeb30:  00000000 00000000 00000000 00000000  
                              
3fffeb40:  00000000 00000000 00000000 00000000  
                              
3fffeb50:  00000000 00000000 00000000 00000000  
                              
3fffeb60:  00000000 00000000 00000000 00000000  
                              
3fffeb70:  00000000 00000000 00000000 00000000  
                              
3fffeb80:  00000000 00000000 00000000 00000000  
                              
3fffeb90:  00000000 00000000 00000000 00000000  
                              
3fffeba0:  000c001c 00000001 00000e10 4022000a  
                              
3fffebb0:  3fffedc0 00000001 3fff2334 402225a1  
                              
3fffebc0:  00000000 6f675f0b 656c676f 74736163  
                              
3fffebd0:  63745f04 6f6c0570 006c6163 00000000  
                              
3fffebe0:  00000000 00000000 00000000 00000000  
                              
3fffebf0:  00000000 00000000 00000000 00000000  
                              
3fffec00:  00000000 00000000 00000000 00000000  
                              
3fffec10:  00000000 00000000 00000000 00000000  
                              
3fffec20:  00000000 00000000 00000000 00000000  
                              
3fffec30:  00000000 00000000 00000000 00000000  
                              
3fffec40:  00000000 00000000 00000000 00000000  
                              
3fffec50:  00000000 00000000 00000000 00000000  
                              
3fffec60:  00000000 00000000 00000000 00000000  
                              
3fffec70:  00000000 00000000 00000000 00000000  
                              
3fffec80:  00000000 00000000 00000000 00000000  
                              
3fffec90:  00000000 00000000 00000000 00000000  
                              
3fffeca0:  00000000 00000000 00000000 00000000  
                              
3fffecb0:  00000000 00000000 00000000 00000000  
                              
3fffecc0:  00000000 000c0018 00000001 4010041c  
                              
3fffecd0:  402252f4 00000030 00000010 ffffffff  
                              
3fffece0:  00000000 00000000 0000001f 4010041c  
                              
3fffecf0:  00000000 00000000 3fffc228 401059d9  
                              
3fffed00:  00000000 00000000 0000001f 4010041c  
                              
3fffed10:  000000c8 00000001 3fffc228 401059d9  
                              
3fffed20:  4000050c 3fffee40 00000000 00000000  
                              
3fffed30:  40259e77 00000030 0000000d ffffffff  
                              
3fffed40:  40259e6d 3ffef81c 00000058 0000012c  
                              
3fffed50:  00000b40 3fffeed8 3fffeed8 00000000  
                              
3fffed60:  00000b40 00000168 00000168 40100cfb  
                              
3fffed70:  0000001a 000000d4 00000020 3ffefcfc  
                              
3fffed80:  0000001a 3fff0388 00000020 40100f7f  
                              
3fffed90:  40101212 00000000 00000002 40101204  
                              
3fffeda0:  402510cc 3fff0388 00000002 401008e4  
                              
3fffedb0:  4026000a 00000000 0000001f 40260fd4  
                              
3fffedc0:  00000002 00000002 3fff4d2c 40225300  
                              
3fffedd0:  40260000 00000000 00000000 00000001  
                              
3fffede0:  3fff0000 00000000 00000000 4022562d  
                              
3fffedf0:  00000000 00000004 3fffee40 3fff4dbc  
                              
3fffee00:  00000000 30303432 00000100 000014e9  
                              
3fffee10:  3fff4ebc 3fff2334 3fff2334 402232a9  
                              
3fffee20:  00000000 00000000 00000000 00000000  
                              
3fffee30:  00000000 00000000 00000000 4025622c  
                              
3fffee40:  00000000 00040001 00000000 40251906  
                              
3fffee50:  00000000 3fff2334 3fff4d2c 4022369d  
                              
3fffee60:  3fff4328 3fff497c 3fff4d2c 402236bc  
                              
3fffee70:  00000011 00000001 00000020 4022e47f  
                              
3fffee80:  3fff4328 3fff497c 3fff4d2c 40221188  
                              
3fffee90:  3fff49ae 00000000 3fff4d7c 3fff497c  
                              
3fffeea0:  3fff49ae 00000000 3fff4d7c 4025226c  
                              
3fffeeb0:  000014e9 3fff4278 3fff4278 40100fb6  
                              
3fffeec0:  00000000 0000012a 0000012a 3fff4278  
                              
3fffeed0:  3fff499a 3fff4318 3fff4278 4025596c  
                              
3fffeee0:  3fff497c 00000014 00000000 40100f7f  
                              
3fffeef0:  40267a70 3fff4ccc 3fff4c14 3fff4c64  
                              
3fffef00:  00000008 3fff4278 3fff497c 4024ea09  
                              
3fffef10:  3fffdc80 3fff4c14 3fff4ccc 4024e82f  
                              
3fffef20:  40271f8e 3fff4c14 3fff4ccc 40271f9f  
                              
3fffef30:  3fff498c 3fff497c 00000000 3ffe85e0  
                              
3fffef40:  40267697 00000000 3fff4ccc 4026d79b  
                              
3fffef50:  40000f49 3fffdab0 3fffdab0 40000f49  
                              
3fffef60:  40000e19 00000005 0008ce74 00000000  
                              
3fffef70:  3fffefc0 aa55aa55 00000044 40105239  
                              
3fffef80:  4010523f 0008ce74 00000000 45450054  
                              
3fffef90:  4010000d 03020054 00470400 0054004d  
                              
3fffefa0:  00000000 3fffef3c 00000000 3ffffe78  
                              
3fffefb0:  3fffffc0 00000000 00000000 feefeffe  
                              
3fffefc0:  feefeffe feefeffe feefeffe feefeffe  
                              
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
                              
3ffff490:  00000000 00000000 0000000a 00000000  
                              
3ffff4a0:  3ffff600 00000000 3ffff600 40238372  
                              
3ffff4b0:  00000000 3ffff5a3 00000000 0000001c  
                              
3ffff4c0:  00000000 feefeffe 00000000 00000032  
                              
3ffff4d0:  3ffff600 00000032 3ffff600 40238372  
                              
3ffff4e0:  0000a000 3fffa98c 3ffff510 feefeffe  
                              
3ffff4f0:  feefeffe feefeffe 40239181 402382a8  
                              
3ffff500:  3ffff600 3fffa98c 3ffff560 402392ac  
                              
3ffff510:  00000032 3ffff5d2 00000000 00000002  
                              
3ffff520:  00000000 00000002 3ffff600 40238372  
                              
3ffff530:  3ffff630 4027f3b4 3ffff590 402392ac  
                              
3ffff540:  00000001 feefeffe feefeffe 4027f3b4  
                              
3ffff550:  00000000 4027f3b6 3ffff600 4023855c  
                              
3ffff560:  00000000 00000032 00000000 00000000  
                              
3ffff570:  00000032 00000043 fe302073 3ffe8c1a  
                              
3ffff580:  00000000 3ffe8c19 3ffff630 402387a7  
                              
3ffff590:  00000004 ffffffff ffffffff 00000002  
                              
3ffff5a0:  00000001 0000000a fe302075 feefeffe  
                              
3ffff5b0:  00000000 a0000000 00000000 0000001c  
                              
3ffff5c0:  3ffff730 3ffff700 00000014 3ffe8344  
                              
3ffff5d0:  00000032 00000002 00000000 40238372  
                              
3ffff5e0:  0000a000 4027de28 00000000 00000043  
                              
3ffff5f0:  3fff25d0 3ffe8344 00000044 40234efd  
                              
3ffff600:  3fffaa07 00000009 00000000 00000000  
                              
3ffff610:  3ffff770 00000000 3ffff770 40238372  
                              
3ffff620:  00000019 3ffff713 00000019 402329c5  
                              
3ffff630:  3ffff76a 3fff5586 0000000e 00000008  
                              
3ffff640:  3ffff770 00000008 3ffff770 40238372  
                              
3ffff650:  00006648 3fff5586 3ffff680 00000043  
                              
3ffff660:  3fff25d0 00000043 00000020 402382a8  
                              
3ffff670:  3ffff770 3fff5586 3ffff6d0 402392ac  
                              
3ffff680:  00000008 3ffff700 3fffa9c4 4023c961  
                              
3ffff690:  3ffe8344 00000008 3fffa9c4 4023c94c  
                              
3ffff6a0:  0000000c 00000008 00000003 402296ca  
                              
3ffff6b0:  30305430 3a30303a 57203030 3ffea4ba  
                              
3ffff6c0:  00000000 3ffea4b9 3ffff770 402387a7  
                              
3ffff6d0:  00000000 00000008 00000000 00000000  
                              
3ffff6e0:  00000008 00000008 73302073 006c6261  
                              
3ffff6f0:  3ffff730 3ffff700 00000008 3ffe836e  
                              
3ffff700:  00000000 3fff2316 3fff6dec 40201e17  
                              
3ffff710:  00ff07d8 00000000 00000000 00000003  
                              
3ffff720:  4027f3ac 00000044 00000000 3fff271c  
                              
3ffff730:  3ffff800 3ffff7f0 00000010 3ffe8344  
                              
3ffff740:  00000008 00000009 00000001 402091d0  
                              
3ffff750:  00000033 4027de28 00000020 3fff271c  
                              
3ffff760:  00000032 3ffe8344 00000032 402329c5  
                              
3ffff770:  3fff5dca 00000cc2 00000029 ffff0208  
                              
3ffff780:  3fff5dc2 00000031 00000008 3fff271c  
                              
3ffff790:  3fff219e 00000003 00000020 40100f7f  
                              
3ffff7a0:  3ffff800 3ffff7f0 00000008 3fff271c  
                              
3ffff7b0:  3fff219e 00000003 3fffa98c 4020930c  
                              
3ffff7c0:  3ffff800 3ffff7f0 00000008 0000000c  
                              
3ffff7d0:  3fff55ec 00000000 3ffff800 3ffff7f0  
                              
3ffff7e0:  0000000c 3ffff7f0 00000008 3fff5586  
                              
3ffff7f0:  00000000 feefeffe 3ffea4ec 3fff5586  
                              
3ffff800:  3fff5df4 00000003 3fff54ac 40202f98  
                              
3ffff810:  00000002 00000000 0000000a 00000000  
                              
3ffff820:  00000002 00000000 0000000a 00000000  
                              
3ffff830:  00000003 00000000 0000000a 00000000  
                              
3ffff840:  00000003 00000000 0000000a 00000000  
                              
3ffff850:  00000005 00000000 00000020 4010041c  
                              
3ffff860:  00000000 3ffff953 00000005 40101e80  
                              
3ffff870:  3ffeb765 40105133 3ffef178 00000036  
                              
3ffff880:  40102a17 3ffef178 3ffff9b0 40238372  
                              
3ffff890:  00000015 02357e5a 3ffefc38 40102bf8  
                              
3ffff8a0:  3ffebfe0 00000000 00000000 402382a8  
                              
3ffff8b0:  00000000 00000000 0000001f 4010041c  
                              
3ffff8c0:  3ffebfe0 7fffffff 3fffc228 401059d9  
                              
3ffff8d0:  4000050c 00004208 3ffff9b0 40238372  
                              
3ffff8e0:  40100fb9 00000030 00000018 ffffffff  
                              
3ffff8f0:  40100fb6 3fff4ccc 3fff68b8 3fff4cc8  
                              
3ffff900:  00000001 3fff4cc8 0000002c 000000a8  
                              
3ffff910:  00000036 3ffffa43 000000ff 00000020  
                              
3ffff920:  40224f34 00000034 00000000 00000030  
                              
3ffff930:  00000008 00000000 0000000a 00000000  
                              
3ffff940:  00000000 a0000000 00000000 0000001c  
                              
3ffff950:  00000000 a0000000 00000000 00000000  
                              
3ffff960:  3ffffac0 00000000 3ffffac0 40238372  
                              
3ffff970:  00000005 00000000 00000020 4010041c  
                              
3ffff980:  00000000 00000000 00000005 40101e80  
                              
3ffff990:  3ffeb765 40105133 3ffef150 401059d9  
                              
3ffff9a0:  00000003 00000000 0000000a 00000000  
                              
3ffff9b0:  00000003 00000000 0000000a 00000000  
                              
3ffff9c0:  00000005 00000000 40268803 00000001  
                              
3ffff9d0:  00000001 00000000 00000014 4010041c  
                              
3ffff9e0:  40268852 3ffeeea8 00000001 4026c3b3  
                              
3ffff9f0:  40268936 3ffeeea8 40101fbb 3ffeeea8  
                              
3ffffa00:  00000014 00000020 00000008 3fffb9d8  
                              
3ffffa10:  402678b8 3ffeeea8 3ffef630 00000020  
                              
3ffffa20:  00000000 4025aa8b 3fff0388 3fff4484  
                              
3ffffa30:  00000000 00000002 00000000 3ffeeea8  
                              
3ffffa40:  3fffb9f2 40105783 3fffb140 3fff3ee8  
                              
3ffffa50:  3fff4c14 3ffffaf1 3ffffab0 402392ac  
                              
3ffffa60:  00000002 4024e3ed 3fffb140 3fff3ee8  
                              
3ffffa70:  3ffe8344 00000000 0000000a 4027d6f9  
                              
3ffffa80:  3ffffaf3 00000003 00000000 00000000  
                              
3ffffa90:  00000000 00000000 0000001f 4010041c  
                              
3ffffaa0:  00000000 3fff4278 3fffc228 401059d9  
                              
3ffffab0:  4000050c ffffffff ffffffff 00000017  
                              
3ffffac0:  40238cc9 00000030 00000000 ffffffff  
                              
3ffffad0:  40239299 3ffe8344 3ffffc40 3ffffbf0  
                              
3ffffae0:  3ffffce0 402382a8 00000001 00000000  
                              
3ffffaf0:  000000a0 00000003 3ffffc83 3ffffc40  
                              
3ffffb00:  3ffe9a17 3ffffce0 402382a8 00000030  
                              
3ffffb10:  3fffb9bc 3fff48d8 3fff4278 00000000  
                              
3ffffb20:  3ffffc80 00000000 3ffffc80 40238372  
                              
3ffffb30:  00000014 3ffffc23 3fffc228 401059d9  
                              
3ffffb40:  4000050c 402800e0 3ffffbf0 00000001  
                              
3ffffb50:  3ffffc80 00000001 3ffffc80 40238372  
                              
3ffffb60:  40238372 3ffe9a2a 3ffffb90 00000001  
                              
3ffffb70:  00000001 00000000 3ffe8344 402382a8  
                              
3ffffb80:  3ffffc80 3ffe9a2a 3ffffbe0 402392ac  
                              
3ffffb90:  00000001 3ffffc83 00000001 00000030  
                              
3ffffba0:  3ffe8344 00000001 00000000 00000000  
                              
3ffffbb0:  3ffffd10 00000000 3ffffd10 40238372  
                              
3ffffbc0:  00000950 3ffffcb3 3ffffbf0 3ffea4ba  
                              
3ffffbd0:  00000000 3ffea4b9 3ffffc80 00000001  
                              
3ffffbe0:  3ffffd10 00000001 3ffffd10 40238372  
                              
3ffffbf0:  00000001 3ffe9a2a 3ffffc20 00000000  
                              
3ffffc00:  3ffe8344 00000030 00000018 402382a8  
                              
3ffffc10:  3ffffd10 3ffe9a2a 3ffffc70 402392ac  
                              
3ffffc20:  00000001 3ffffd13 00000032 3ffea4ba  
                              
3ffffc30:  3ffe8344 3ffea4b9 00000000 00000001  
                              
3ffffc40:  3ffffd70 00000001 3ffffd70 40238372  
                              
3ffffc50:  00000001 3ffe9a2a 3ffffc80 3ffea4ba  
                              
3ffffc60:  00000000 3ffea4b9 3ffffd10 402387a7  
                              
3ffffc70:  00000000 00000001 00000000 00000000  
                              
3ffffc80:  00000001 00000001 00302073 ffff0208  
                              
3ffffc90:  3ffe8344 00000f9f 00000000 00000036  
                              
3ffffca0:  00006d30 3ffffd60 00000010 3ffffd10  
                              
3ffffcb0:  00000005 00000000 00000020 4010041c  
                              
3ffffcc0:  00000000 3ffea4b9 00000000 40101e80  
                              
3ffffcd0:  3ffffda0 3ffffd90 00000010 3ffe8344  
                              
3ffffce0:  00000001 00000009 00000001 3ffffd00  
                              
3ffffcf0:  00000015 096d3ea0 3ffefc38 0000138a  
                              
3ffffd00:  3fff7f8c 3ffe8344 00000fa0 402329c5  
                              
3ffffd10:  3fff1206 096d3ea0 00000f9e ffff0208  
                              
3ffffd20:  3fff1205 00000f9f 00000000 00000001  
                              
3ffffd30:  00000001 00006208 00000010 3ffe8344  
                              
3ffffd40:  3ffec01c 00000009 00000001 096d3ea0  
                              
3ffffd50:  3ffebfe0 2c9f0300 4000050c 3fffc278  
                              
3ffffd60:  00000978 3fffc200 00000022 402329c5  
                              
3ffffd70:  4021b4fc 00000030 3ffffda0 00000000  
                              
3ffffd80:  3fff28ac 00000010 00000020 40100fb6  
                              
3ffffd90:  00000950 3ffffe90 3ffffe80 40229ef4  
                              
3ffffda0:  3ffffdfe 00000000 00000000 40100ffd  
                              
3ffffdb0:  3fff28ac 00000020 00000020 3ffffe80  
                              
3ffffdc0:  40229ec8 3ffffe80 3ffffdfe 40229611  
                              
3ffffdd0:  00000039 0000000a 3ffffdfd 00000000  
                              
3ffffde0:  3fff4ccc 00000010 3ffffeb0 4022a79e  
                              
3ffffdf0:  0022ef68 00000000 401007a0 3039fec2  
                              
3ffffe00:  0000000b 00000002 00000009 4022ac44  
                              
3ffffe10:  00000009 00000020 3ffffe90 00000000  
                              
3ffffe20:  00000950 0000012a 0000012a 40100cfb  
                              
3ffffe30:  0000000c 00000003 0000000a 00000000  
                              
3ffffe40:  3ffffebc 4022ef68 00000020 40100f7f  
                              
3ffffe50:  00000978 0000012f 0000012f 40100cfb  
                              
3ffffe60:  3ffffebc 4022ef68 3ffffe90 00000000  
                              
3ffffe70:  00000001 00000001 00000020 40100f7f  
                              
3ffffe80:  4022ef68 00000000 000003e8 00000000  
                              
3ffffe90:  3fff4c00 000c001f 3ffffeb0 4022a70e  
                              
3ffffea0:  3ffea4ec 3fff6b0c 3fff2316 402132aa  
                              
3ffffeb0:  3fff4c00 000c000f 00ff6b0c 4022ef58  
                              
3ffffec0:  5a02a8c0 4022ef58 5a02a8c0 00000000  
                              
3ffffed0:  3ffea4ec 3fff6b0c 3fff2316 40213304  
                              
3ffffee0:  3ffea4ec 00000038 00000bbb 40212d7e  
                              
3ffffef0:  3ffea4ec 00000038 3fff6d84 40216cdd  
                              
3fffff00:  00000000 4bc6a7f0 5374bc6a 00000000  
                              
3fffff10:  00000000 4bc6a7f0 5c6a7ef9 00000000  
                              
3fffff20:  00000000 00000000 4bc6a7f0 00000000  
                              
3fffff30:  00000000 00000000 401007a0 000269cd  
                              
3fffff40:  00000000 00000000 3fff6ddc 40201e3b  
                              
3fffff50:  02ca07d6 3fffff60 00000008 3fff271c  
                              
3fffff60:  007a1200 bd406325 3fff5f00 0000000f  
                              
3fffff70:  00000000 00000000 3fff54ac 40202f98  
                              
3fffff80:  00000000 00000000 00000001 4010041c  
                              
3fffff90:  3fffdad0 00000000 3fff26dc 4010043d  
                              
3fffffa0:  3fffdad0 00000000 3fff26dc 4022b68d  
                              
<<<stack<<<
                                                                   
             
                                                                 
last failed alloc call: 402257C2(540)
                                         

                                                                               
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
