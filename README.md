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

HH works grouping lines of the original script into blocks. Blocks can be started with a special comment that HH understands that has the form __#[marker]\_begin__ (the default marker is __handsomeh__ ), and ended with __#[marker]\_end__. The defaults comments are then __#handsomeh_begin__ and __#handsomeh_end__. The begin comment can be followed on the same line by a JSON hash object (eventually empty) that control the behavior of that block.

The following hash keys are valid commands to control the block behavior:

| Command | Default | Meaning | 
|---|---|---|
| show_command_header | yes | Shows an header before the block execution |
| show_command | yes | Shows the block commands |
| show_output_header | yes | Shows an header the introduces the block execution output |
| show_output  | yes | Show the block execution output |
| pre_command_wait | yes | Wait for a keypress before the execution the block |
| post_command_wait | no | Wait for a keypress after the execution the block|
| pre_output_wait | yes | Wait for a keypress before showing the block execution output |
| post_output_wait | no | Wait for a keypress after showing the block execution output |
| subshell | yes | Execute the block in a subshell (only relevant when the output is not shown) |

## Installation

Just copy the HH script in a directory within the PATH. The script has same dependencies:

* __docopt__, a python library to handle command line arguments easily.
* __xdotool__, to grab X windows.
* __imagemagick__, to build gif files.
* __ffmpeg__, to create videos.
