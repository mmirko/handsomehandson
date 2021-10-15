# Handsome Hands-on

Simple Python3 program to create demos from actual shell scripts

## Usage

The purpose of HandsomeHandson (HH) is to transform a Linux shell script into a demo (target) script. I created it to simplify the creation of hands-on for my projects.  
The two scripts (the source one and the demo) execute the same commands but the demo will pause the execution waiting for a key press according to some comments placed in the source script.  The pause is meant to be used by an hypothetical speaker that is talking about the current script's line. HH can also create a sequence of frames, a video, a gif or a Beamer slide showing the script execution.

HH accepts the following options:

<pre>
Usage:
  hh -s &lt;sourcescript&gt; [--marker &lt;textmarker&gt;] [-d] (-t &lt;targetscript&gt; | -f &lt;framesdirectory&gt; | -v &lt;videofile&gt; | -g &lt;giffile&gt; | -b &lt;beamerdirectory&gt;)
  hh -h | --help

Options:
  -h --help                                         Show this screen.
  -s &lt;sourcescript&gt;, --source &lt;sourcescript&gt;        Source script
  --marker &lt;textmarker&gt;                             Marker to intercept commands [default: handsomeh]
  -d                                                Debug
  -t &lt;targetscript&gt;, --target &lt;targetscript&gt;        Target script
  -v &lt;videofile&gt;, --video &lt;videofile&gt;               Video file
  -g &lt;giffile&gt;, --gif &lt;giffile&gt;                     GIF image
  -f &lt;framesdirectory&gt;, --frames &lt;framesdirectory&gt;  Target frames directory
  -b &lt;beamerdirectory&gt;, --beamer &lt;beamerdirectory&gt;  Target beamer directory
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

is transformed by HH into a usable demo script (targetscript) by giving this command:

```console
hh -s sourcescript -t targetscript
```

The resulting script is the following. This is usable interactively in an hypothetical live presentation. 

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

HH can also create a gif file that shows the script running this way:

```console
hh -s sourcescript -g testscript01.gif
```
The result is the following

![Example result](testscript01.gif)

## Block commands

## Examples

## Installation

### Dependencies

docopt

imagemagick

xdotool

ffmpeg
