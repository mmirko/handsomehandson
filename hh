#!/usr/bin/env python3

"""Handsome Hands-on - A tool to create handons pre-scripted demo
   Copyright 2021 - Mirko Mariotti - https://www.mirkomariotti.it

Usage:
  hh -s <sourcescript> [--marker <textmarker>] [-d] (-t <targetscript> | -f <framesdirectory> | -v <videofile> | -g <giffile> | -b <beamerdirectory>)
  hh -h | --help

Options:
  -h --help                                         Show this screen.
  -s <sourcescript>, --source <sourcescript>        Source script
  --marker <textmarker>                             Marker to intercept commands [default: handsomeh]
  -d                                                Debug
  -t <targetscript>, --target <targetscript>        Target script
  -v <videofile>, --video <videofile>               Video file
  -g <giffile>, --gif <giffile>                     GIF image
  -f <framesdirectory>, --frames <framesdirectory>  Target frames directory
  -b <beamerdirectory>, --beamer <beamerdirectory>  Target beamer directory
"""
from docopt import docopt
from os import path,mkdir,chmod,system,chdir,remove
import sys
import stat
import re
import json
import tempfile
import shutil

def Alert(message):
    print ("\033[31m[Alert]\033[0m - " + message)

def Warning(message):
    print ("\033[33m[Warn]\033[0m  - " + message)

def Info(message):
    print ("\033[32m[Info]\033[0m  - " + message)

def Debug(message):
    print ("\033[35m[Debug]\033[0m - " + message)

def targetcommitblock(debug,blockinfo,block,seq):
    # Possible command:
    # "show_command_header" - yes no - defaults to yes
    # "show_command" - yes no - defaults to yes
    # "show_output_header" - yes no - defaults to yes
    # "show_output" - yes no - defaults to yes

    # "pre_command_wait" - yes no - defaults to yes
    # "post_command_wait" - yes no - defaults to no
    # "pre_output_wait" - yes no - defaults to yes
    # "post_output_wait" - yes no - defaults to no
    
    # "subshell" - yes no - defaults to yes (only used and relevant for not shown output)

    # TODO
    # "pre_command_comment" - comment id
    # "post_command_comment" - commend id
    # "pre_output_comment" - comment id
    # "post_output_comment" - comment id


    result=""
    resultv=""

    if "pre_command_wait" in blockinfo and blockinfo["pre_command_wait"]=="no":
        if debug: Debug("  Block pre-command wait not used")
    else:
        if debug: Debug("  Block pre-command wait used")
        result+="\n# Pre-Command wait\n"
        result+="read -n 1 -s -r\n"
        resultv+="sleep 1\n"
        #resultv+="import -window `xargs -0 -L1 -a /proc/self/environ | grep WINDOWID | cut -d= -f2` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n"
        resultv+="import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n"
        seq+=1

    if "show_command_header" in blockinfo and blockinfo["show_command_header"]=="no":
        if debug: Debug("  Block command header not shown")
    else:
        if debug: Debug("  Block command header shown")
        result+="\n# Command header\n"
        result+="echo -en \"\033[32m\033[11D[ Command > \033[0m\"\n"
        resultv+="echo -en \"\033[32m\033[11D[ Command > \033[0m\"\n"

    if "show_command" in blockinfo and blockinfo["show_command"]=="no":
        if debug: Debug("  Block command not shown")
    else:
        if debug: Debug("  Block command shown")
        result+="\n# Command display\n"
        result+="cat << EndOfBlock\n"
        result+=block
        result+="EndOfBlock\n"
        resultv+="cat << EndOfBlock\n"
        resultv+=block
        resultv+="EndOfBlock\n"

    if "post_command_wait" in blockinfo and blockinfo["post_command_wait"]=="yes":
        if debug: Debug("  Block post-command wait used")
        result+="\n# Post-Command wait\n"
        result+="read -n 1 -s -r\n"
        resultv+="sleep 1\n"
        resultv+="import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n"
        seq+=1
    else:
        if debug: Debug("  Block post-command wait not used")

    if "pre_output_wait" in blockinfo and blockinfo["pre_output_wait"]=="no":
        if debug: Debug("  Block pre-output wait not used")
    else:
        if debug: Debug("  Block pre-output wait used")
        result+="\n# Pre-output wait\n"
        result+="read -n 1 -s -r\n"
        resultv+="sleep 1\n"
        resultv+="import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n"
        seq+=1

    if "show_output_header" in blockinfo and blockinfo["show_output_header"]=="no":
        if debug: Debug("  Block output header not shown")
    else:
        if debug: Debug("  Block output header shown")
        result+="\n# Output header\n"
        result+="echo -e \"\033[31m\033[11D[ Output  > \033[0m\"\n"
        resultv+="echo -e \"\033[31m\033[11D[ Output  > \033[0m\"\n"

    if "show_output" in blockinfo and blockinfo["show_output"]=="no":
        if debug: Debug("  Block output not shown")
        result+="\n# Output not shown\n"
        if "subshell" in blockinfo and blockinfo["subshell"]=="no":
            for line in block.split("\n"):
                line=line.strip()
                if line !="":
                    result+=line+" > /dev/null 2>&1\n"
                    resultv+=line+" > /dev/null 2>&1\n"
        else:
            result+="(\n"
            resultv+="(\n"
            for line in block.split("\n"):
                line=line.strip()
                if line !="":
                    result+=line+"\n"
                    resultv+=line+"\n"
            result+=") > /dev/null 2>&1\n"
            resultv+=") > /dev/null 2>&1\n"
    else:
        if debug: Debug("  Block output shown")
        result+="\n# Output display\n"
        result+=block
        resultv+=block

    if "post_output_wait" in blockinfo and blockinfo["post_output_wait"]=="yes":
        if debug: Debug("  Block post-output wait used")
        result+="\n# Post-Output wait\n"
        result+="read -n 1 -s -r\n"
        resultv+="sleep 1\n"
        resultv+="import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n"
        seq+=1
    else:
        if debug: Debug("  Block post-output wait not used")


    return result,resultv,seq

def main():
    arguments = docopt(__doc__, version='Handsome Hands-on 1.0')
    debug = arguments["-d"]
    if debug: Debug("Arguments:\n"+str(arguments))

    marker = "#"+arguments["--marker"]
    if debug: Debug("Marker: "+marker)

    sourcescript = arguments["--source"]
    if debug: Debug("Source script: "+sourcescript)   

    targetscript = arguments["--target"]
    if debug:
        if targetscript: Debug("Target script: "+targetscript)
        else: Debug("No target script")

    videofile = arguments["--video"]
    if debug:
        if videofile: Debug("Video file: "+videofile)
        else: Debug("No video file")

    giffile = arguments["--gif"]
    if debug:
        if giffile: Debug("Gif file: "+giffile)
        else: Debug("No gif file")

    beamerdir = arguments["--beamer"]
    if debug:
        if beamerdir: Debug("Beamer directory: "+beamerdir)
        else: Debug("No beamer dir")

    framesdir = arguments["--frames"]
    if debug:
        if framesdir: Debug("Frames directory: "+framesdir)
        else: Debug("No frames dir")
 
    # beamerdir = arguments["--beamer"]
    # if beamerdir != None:
    #     if path.exists(beamerdir):
    #         Alert("Error: beamer dir '"+beamerdir+"' exists")
    #         sys.exit(1)

    # framesdir = arguments["--frames"]
    # if framesdir != None:
    #     if path.exists(framesdir):
    #         Alert("Error: frames dir '"+framesdir+"' exists")
    #         sys.exit(1)

    if debug: Debug("Opening files")

    try:
        ssource = open(sourcescript)
    except:
        Alert("Error opening source script '"+sourcescript+"'")
        sys.exit(1)        

    # if beamerdir != None:
    #     try:
    #         mkdir(beamerdir)
    #     except:
    #         Alert("Error: creation of the directory '"+beamerdir+"' failed")
    #         sys.exit(1)

    #     try:
    #         btarget = open(beamerdir+"/script.sh","w")
    #     except:
    #         Alert("Error opening target script '"+beamerdir+"/script.sh'")
    #         sys.exit(1)        

    # if framesdir != None:
    #     try:
    #         mkdir(framesdir)
    #     except:
    #         Alert("Error: creation of the directory '"+framesdir+"' failed")
    #         sys.exit(1)


    ttarget = tempfile.NamedTemporaryFile(mode="w+t",delete=False)
    imagesdir = tempfile.TemporaryDirectory()

    try:
        vtarget = open(imagesdir.name+"/script.sh","w")
    except:
        Alert("Error opening target script '"+framesdir+"/script.sh'")
        sys.exit(1)        

    if debug: Debug("Temporary script file " + str(ttarget.name) )
    if debug: Debug("Temporary directory " + str(imagesdir.name))

    if debug: Debug("Start parsing script")
    
    insideblock = False
    block = ""
    seq=0

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
                
                result,resultv,seq=targetcommitblock(debug,blockinfo,block,seq)
                ttarget.write(result)
                if imagesdir != None: vtarget.write(resultv)

                block = ""
                insideblock = False

            else:
                if insideblock:
                    block+=line
                else:
                    ttarget.write(line)

                    if imagesdir != None:
                        if line == "#!/bin/bash\n":
                            line += "export HHCWD=`pwd`\n"
                        vtarget.write(line)
 
    ttarget.write("\n# Final wait\n")
    ttarget.write("read -n 1 -s -r\n")
    
    ssource.close()
    ttarget.close()

    # if beamerdir != None:
    #     btarget.write("sleep 1\n")
    #     #btarget.write("import -window `xargs -0 -L1 -a /proc/self/environ | grep WINDOWID | cut -d= -f2` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n")
    #     btarget.write("import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n")
    #     btarget.close()
    #     chmod(beamerdir+"/script.sh", stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
    #     chdir(beamerdir)
    #     system("./script.sh")
    #     chdir("..")
    #     btarget=open(beamerdir+"/frame.tex","w")
    #     btarget.write("\\begin{frame}{Frame Title}\n")
    #     btarget.write("\\begin{figure}[ht]\n")
    #     btarget.write("\\begin{overlayarea}{9cm}{8cm}\n")
    #     for i in range(seq):
    #         btarget.write("\\only<"+str(i+1)+">{\\includegraphics[width=9cm]{frameimg"+"{:0>3d}".format(i)+".jpg}}\n")
    #     btarget.write("\\end{overlayarea}\n")
    #     btarget.write("\\end{figure}\n")
    #     btarget.write("\\end{frame}\n")


    if imagesdir != None and not targetscript:
        vtarget.write("sleep 1\n")
        vtarget.write("import -window `xdotool getwindowfocus` $HHCWD\"\"/frameimg"+"{:0>3d}".format(seq)+".jpg\n")
        vtarget.close()
        chmod(imagesdir.name+"/script.sh", stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
        chdir(imagesdir.name)
        system("./script.sh")

    if targetscript:
        if path.exists(targetscript):
            Alert("Error: target file '"+targetscript+"' exists")
            remove(ttarget.name)
            sys.exit(1)
        shutil.copy2(ttarget.name,targetscript)
        chmod(targetscript, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR)
        remove(ttarget.name)

if __name__ == '__main__':
    main()
