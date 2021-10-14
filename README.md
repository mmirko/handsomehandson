# Handsome Hands-on

Simple Python3 program to create demos from actual shell scripts

## Usage

The purpose of HandsomeHandson (HH) is to transform a Linux shell script into a demo (target) script. I created it to simplify the creation of hands-on for my projects.  
The two scripts (the source one and the demo) execute the same commands but the demo will pause the execution waiting for a key press according to some comments placed in the source script.  The pause is meant to be used by an hypothetical speaker that is talking about the current script's line. HH can also create a sequence of frames, a video, a gif or a Beamer slide showing the script execution.

d


HH accepts the following options:

<pre>
Usage:
  hh -s &lt;sourcescript&gt; -t &lt;targetscript&gt; [-v &lt;videodirectory &gt;] [-b &lt;beamerdirectory &gt;] [--marker &lt;textmarker&gt;] [-d]
  hh -h | --help

Options:
  -h --help                                         Show this screen.
  -s &lt;sourcescript&gt;, --source &lt;sourcescript&gt;        Source script
  -t &lt;targetscript&gt;, --target &lt;targetscript&gt;        Target script
  -v &lt;videodirectory&gt;, --video &lt;videodirectory&gt;     Target video directory
  -b &lt;beamerdirectory&gt;, --beamer &lt;beamerdirectory&gt;  Target beamer directory
  --marker &lt;textmarker&gt;                             Marker to intercept commands
  -d                                                Debug  
</pre>

## Example

This is a simple example. A Script like the following (sourcescript):

```bash
#!/bin/bash

reset

#handsomeh_begin {}
echo "Test01"
#handsomeh_end

#handsomeh_begin {"show_output_header": "yes"}
echo "Test02"
#handsomeh_end
```

is transformed by HH into (targetscript):

```bash
#!/bin/bash

reset

# Pre-Command wait
read -n 1 -s -r

# Command header
[ Command > "

# Command display
cat << EndOfBlock
echo "Test01"
EndOfBlock

# Pre-output wait
read -n 1 -s -r

# Output header
[ Output  > "

# Output display
echo "Test01"

# Pre-Command wait
read -n 1 -s -r

# Command header
[ Command > "

# Command display
cat << EndOfBlock
echo "Test02"
EndOfBlock

# Pre-output wait
read -n 1 -s -r

# Output header
[ Output  > "

# Output display
echo "Test02"

# Final wait
read -n 1 -s -r
```


## Phasis

## Block commands

## Examples

## Installation

### Dependencies

docopt

imagemagick

xdotool

ffmpeg


mimic

### TODO

Several parts could be improved, I don't know if or when I will do it. The first that came to mind are:  

- Make the sleep time among frames in the video customizable in each block.

- Images borders

