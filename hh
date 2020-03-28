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

def Alert(message):
    print ("\033[31m[Alert]\033[0m - " + message)

def Warning(message):
    print ("\033[33m[Warn]\033[0m  - " + message)

def Info(message):
    print ("\033[32m[Info]\033[0m  - " + message)

def Debug(message):
    print ("\033[35m[Debug]\033[0m - " + message)

def main():
    arguments = docopt(__doc__, version='Handsome Hands-on 1.0')
    debug = arguments["-d"]
    if debug: Debug("Arguments:\n"+str(arguments))

    marker = "#"+arguments["--marker"]
    if debug: Debug("Marker: "+marker)

    sourcescript = arguments["--source"]
    if debug: Debug("Source script: "+sourcescript)   

    try:
        ssource = open(sourcescript)
    except:
        Alert("Error opening source script '"+sourcescript+"'")
        sys.exit(1)        

    targetscript = arguments["--target"]
    if debug: Debug("Target script: "+targetscript)   

    if path.exists(targetscript):
        Alert("Error: target file '"+targetscript+"' exists")
        ssource.close()
        sys.exit(1)

    beamerdir = arguments["--beamer"]
    if beamerdir != None:
        if path.exists(beamerdir):
            Alert("Error: beamer dir '"+beamerdir+"' exists")
            ssource.close()
            sys.exit(1)

    videodir = arguments["--video"]
    if videodir != None:
        if path.exists(videodir):
            Alert("Error: video dir '"+videodir+"' exists")
            ssource.close()
            sys.exit(1)

    for line in ssource:
        m = re.search("^" + marker + "_begin", i)
        if m:



    ssource.close()


#    m = re.search("^" + marker + "_begin", i)
#    if m:
#        try:
#            diffdata = i.replace(marker + "_begin", "")
#            currblockinfo = json.loads(diffdata)
#            if "name" in currblockinfo:
#                beginblock = True
#
#                # The name
#                name = currblockinfo["name"]
#
#                # The type
#                eltype = "repo"
#                if "type" in currblockinfo:
#                    eltype = currblockinfo["type"]
#                else:
#                    if debug:
#                        Debug("Implicit 'repo' type for " + name)
#                
#                if eltype == "repo":
#                    if debug:
#                        Debug("Selected 'repo' type for " + name)
#                elif eltype == "local":
#                    if debug:
#                        Debug("Selected 'local' type for " + name)
#                else:
#                    Alert("type not defined in "+i.strip())
#                    sys.exit(1)
#
#                #The ID and the Matadata
#                idd = ""
#                meta = {}
#                if "id" in currblockinfo:
#                    idd = currblockinfo["id"]
#                    if eltype == "repo":
#                        meta = elements[idd]
#                    else:
#                        meta = localelements[idd]
#                    if debug:
#                        Debug("Id for "+ name + " is " + idd + " with metadata " + str(meta))
#                else:
#                    Debug("Searching for " + name + " element id in " + eltype)
#                    if eltype == "repo":
#                        if name in name2id:
#                            if len(name2id[name]) == 1:
#                                idd = name2id[name][0]
#                                meta = elements[idd]
#                                if debug:
#                                    Debug("Id for "+ name + " is " + idd + " with metadata " + str(meta))
#                            else:
#                                Alert("Multiple id found for "+i.strip())
#                                sys.exit(1)
#                        else:
#                            Alert("No id found for "+i.strip())
#                            sys.exit(1)
#                    else:
#                        if name in localname2id:
#                            if len(localname2id[name]) == 1:
#                                idd = localname2id[name][0]
#                                meta = localelements[idd]
#                                if debug:
#                                    Debug("Id for "+ name + " is " + idd + " with metadata " + str(meta))
#                            else:
#                                Alert("Multiple id found for "+i.strip())
#                                sys.exit(1)
#                        else:
#                            Alert("No id found for "+i.strip())
#                            sys.exit(1)
#                if idd=="":
#                    Alert("Cannot find id for "+i.strip())
#                    sys.exit(1)
#
#                # Compute the new header
#                newheader = {}
#                newheader["name"]=name
#                newheader["id"]=idd
#                newheader["type"]=eltype
#
#                for verb in ["use_frame","use_title","use_subtitle"]:
#                    if verb in currblockinfo:
#                        newheader[verb]=currblockinfo[verb]
#
#                fw.write(marker + "_begin "+json.dumps(newheader)+"\n")
#
#                # Compute the data: basefile
#                newdata = ""
#                if "basefile" in meta:
#                    prefix=idd
#                    if eltype=="repo":
#                        prefix=repopath+'/'+idd
#                    newdata = '\\input{'+prefix+'/'+meta["basefile"]+'}\n'
#                
#                # Compute the data: frame and titles
#                frame=None
#                title=None
#                subtitle=None
#
#                if "use_frame" in meta or "use_frame" in currblockinfo:
#                    frame=True
#
#                if "use_title" in currblockinfo:
#                    title=currblockinfo["use_title"]
#                else:
#                    if "use_title" in meta:
#                        title=meta["use_title"]
#
#                if "use_subtitle" in currblockinfo:
#                    subtitle=currblockinfo["use_subtitle"]
#                else:
#                    if "use_subtitle" in meta:
#                        subtitle=meta["use_subtitle"]
#
#                predata=""
#                postdata=""
#
#                if frame != None:
#                    predata='\\begin{frame}'
#                    postdata='\\end{frame}'
#
#                    if title !=None:
#                        predata+='\\frametitle{'+title+'}'
#
#                        if subtitle !=None:
#                            predata+='\\framesubtitle{'+subtitle+'}'
#
#                    predata+='\n'
#                    postdata+='\n'
#
#                newdata=predata+newdata+postdata
#                fw.write(newdata)
#
#                continue
#            else:
#                Alert("name not defined in "+i.strip())
#                sys.exit(1)
#        except BaseException:
#            Alert("Block loading failed reading " + i.strip())
#            sys.exit(1)
#    else:
#        m = re.search("^" + marker + "_end", i)
#        if m:
#            fw.write(i)
#            beginblock = False
#            continue
#        else:
#            if not beginblock:
#                fw.write(i)
#            m = re.search("^" + rmarker, i)
#            if m:
#                try:
#                    diffdata = i.replace(rmarker, "")
#                    checkinfo = json.loads(diffdata)
#                    if "name" in checkinfo:
#                        Info("Reord block: "+checkinfo["name"])
#                except BaseException:
#                    pass
#
#fw.close()
#f.close()
#
#move(filename+"_makenew",filename)


if __name__ == '__main__':
    main()
