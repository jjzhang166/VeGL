# -*- coding: utf-8 -*-
"""
Created on Sun Dec 29 00:53:25 2019

@author: ncort
"""

import glob, os, re
from pathlib import Path

def do_rename(src, dst):
    try:
        os.rename(src, dst)
    except WindowsError:
        os.rename(src, dst + "(2)")

for file in glob.glob("*/*.png"):
    directories = os.path.abspath(file).rsplit('\\')
    directories = '\\'.join(directories[:-1])
    
    name = os.path.splitext(os.path.basename(file))[0]

    if(re.match('.*(alb|base|Base|Albedo|albedo).*', name)):
        do_rename(file,directories + "\\albedo.png")
        
    elif(re.match('.*(normal|Normal).*', name)):
        do_rename(file,directories + "\\normal.png")

    elif(re.match('.*(ambient|AO|ao|occlusion|Ambient).*', name)):
        do_rename(file,directories + "\\ao.png")

    elif(re.match('.*(roughness|Roughness|Rough).*', name)):
        do_rename(file,directories + "\\roughness.png")

    elif(re.match('.*(height|Height).*', name)):
        do_rename(file,directories + "\\height.png")

    elif(re.match('.*(metal|Metal|nmap).*', name)):
        do_rename(file,directories + "\\metal.png")

