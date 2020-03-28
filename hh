#!/usr/bin/env python3

"""Handsome Hands-on - A tool to create handons pre-scripted demo
   Copyright 2020 - Mirko Mariotti - https://www.mirkomariotti.it

Usage:
  hh -s <sourcescript> -t <targetscript> [-v <videodirectory>] [-b <beamerdirectory>] [--marker <textmarker>] [-d]
  hh -h | --help

Options:
  -h --help                                         Show this screen.
  -s <sourcescript>, --source <sourcescript>        Source script
  -t <targetscript>, --target <targetscript>        Target script
  -v <videodirectory>, --video <videodirectory>     Target video directory
  -b <beamerdirectory>, --beamer <beamerdirectory>  Target beamer directory
  --marker <textmarker>                             Marker to intercept commands [default: handsomeh]
  -d                                                Debug
"""
from docopt import docopt
from os import path
import sys
import re
import json

def Alert(message):
    print ("\033[31m[Alert]\033[0m - " + message)

def Warning(message):
    print ("\033[33m[Warn]\033[0m  - " + message)

def Info(message):
    print ("\033[32m[Info]\033[0m  - " + message)

def Debug(message):
    print ("\033[35m[Debug]\033[0m - " + message)

def targetcommitblock(debug,blockinfo,block):
    # Possible command:
    # "show_command_header" - yes no - defaults to yes
    # "show_command" - yes no - defaults to yes
    # "show_output_header" - yes no - defaults to yes
    # "show_output" - yes no - defaults to yes

    # "pre_command_wait" - yes no - defaults to yes
    # "post_command_wait" - yes no - defaults to no
    # "pre_output_wait" - yes no - defaults to yes
    # "post_output_wait" - yes no - defaults to no

    result=""

    if "pre_command_wait" in blockinfo and blockinfo["pre_command_wait"]=="no":
        if debug: Debug("  Block pre-command wait not used")
    else:
        if debug: Debug("  Block pre-command wait used")
        result+="\n# Pre-Command wait\n"
        result+="read -n 1 -s -r\n"

    if "show_command_header" in blockinfo and blockinfo["show_command_header"]=="no":
        if debug: Debug("  Block command header not shown")
    else:
        if debug: Debug("  Block command header shown")
        result+="\n# Command header\n"
        result+="echo -en \"\033[32m\033[11D[ Command > \033[0m\"\n"

    if "show_command" in blockinfo and blockinfo["show_command"]=="no":
        if debug: Debug("  Block command not shown")
    else:
        if debug: Debug("  Block command shown")
        result+="\n# Command display\n"
        result+="cat << EndOfBlock\n"
        result+=block
        result+="EndOfBlock\n"

    if "post_command_wait" in blockinfo and blockinfo["post_command_wait"]=="yes":
        if debug: Debug("  Block post-command wait used")
        result+="\n# Post-Command wait\n"
        result+="read -n 1 -s -r\n"
    else:
        if debug: Debug("  Block post-command wait not used")

    if "pre_output_wait" in blockinfo and blockinfo["pre_output_wait"]=="no":
        if debug: Debug("  Block pre-output wait not used")
    else:
        if debug: Debug("  Block pre-output wait used")
        result+="\n# Pre-output wait\n"
        result+="read -n 1 -s -r\n"

    if "show_output_header" in blockinfo and blockinfo["show_output_header"]=="no":
        if debug: Debug("  Block output header not shown")
    else:
        if debug: Debug("  Block output header shown")
        result+="\n# Output header\n"
        result+="echo -e \"\033[31m\033[11D[ Output  > \033[0m\"\n"

    if "show_output" in blockinfo and blockinfo["show_output"]=="no":
        if debug: Debug("  Block output not shown")
    else:
        if debug: Debug("  Block output shown")
        result+="\n# Output display\n"
        result+=block

    if "post_output_wait" in blockinfo and blockinfo["post_output_wait"]=="yes":
        if debug: Debug("  Block post-output wait used")
        result+="\n# Post-Output wait\n"
        result+="read -n 1 -s -r\n"
    else:
        if debug: Debug("  Block post-output wait not used")


    return result

def main():
    arguments = docopt(__doc__, version='Handsome Hands-on 1.0')
    debug = arguments["-d"]
    if debug: Debug("Arguments:\n"+str(arguments))

    marker = "#"+arguments["--marker"]
    if debug: Debug("Marker: "+marker)

    sourcescript = arguments["--source"]
    if debug: Debug("Source script: "+sourcescript)   

    targetscript = arguments["--target"]
    if debug: Debug("Target script: "+targetscript)   

    if path.exists(targetscript):
        Alert("Error: target file '"+targetscript+"' exists")
        sys.exit(1)

    beamerdir = arguments["--beamer"]
    if beamerdir != None:
        if path.exists(beamerdir):
            Alert("Error: beamer dir '"+beamerdir+"' exists")
            sys.exit(1)

    videodir = arguments["--video"]
    if videodir != None:
        if path.exists(videodir):
            Alert("Error: video dir '"+videodir+"' exists")
            sys.exit(1)

    if debug: Debug("Opening files")

    try:
        ssource = open(sourcescript)
    except:
        Alert("Error opening source script '"+sourcescript+"'")
        sys.exit(1)        

    try:
        ttarget = open(targetscript,"w")
    except:
        Alert("Error opening target script '"+targetscript+"'")
        sys.exit(1)        


    if debug: Debug("Start parsing script")

    insideblock = False
    block = ""

    for line in ssource:
        m = re.search("^" + marker + "_begin", line)
        if m:
            if insideblock:
                Alert("Wrong sequence of blocks")
                sys.exit(1)

            diffdata = line.replace(marker + "_begin", "").strip()
            try:
                blockinfo = json.loads(diffdata)
                insideblock = True
                if debug:
                    Debug("Loaded new block: "+str(diffdata))
                    Debug("Block data:")
            except:
                Alert("Decoding of json failed: "+str(diffdata))
                sys.exit(1)

        else:
            m = re.search("^" + marker + "_end", line)
            if m:
                if not insideblock:
                    Alert("Wrong sequence of blocks")
                    sys.exit(1)

                if debug:
                    print (block.rstrip())
                    Debug("Block ended")
                
                ttarget.write(targetcommitblock(debug,blockinfo,block))

                block = ""
                insideblock = False

            else:
                if insideblock:
                    block+=line

    ssource.close()
    ttarget.close()


if __name__ == '__main__':
    main()
