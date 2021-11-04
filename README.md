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
| pre_command_audio | None | Audio file to use as description before the execution of the block (video only) |
| post_command_audio | None | Audio file to use as description after the execution of the block (video only) |
| pre_output_audio | None | Audio file to use as description before showing the block execution output (video only) |
| post_output_audio | None | Audio file to use as description after showing the block execution output (video only) |
| pre_command_text | None | Text file to use as description before the execution of the block (video only) |
| post_command_text | None | Text file to use as description after the execution of the block (video only) |
| pre_output_text | None | Text file to use as description before showing the block execution output (video only) |
| post_output_text | None | Text file to use as description after showing the block execution output (video only) |
| subshell | yes | Execute the block in a subshell (only relevant when the output is not shown) |

## Video and speech synthesis

HH can also produce videos of the demos using a syntax similar to the previous examples.

```console
hh -s sourcescript -v testscript01.mp4
```

When in video mode, hh can also use the related block commands shown in the previous table. In particular for every block can be specified:

- an audio file to merge with the video when the specific block is reached.
- a text file to be converted in audio by the mimic speech synthesis application.

This is an example of a such a block commands:

```console
#handsomeh_begin {"pre_command_text": "/home/mirko/sometext.txt"}
```

When using the speech synthesis, the text can also be given inline putting the "content:" prefix in the value of the command. This is an example:

```console
#handsomeh_begin {"pre_command_text": "content:This will be synthesized."}
```

## Installation

Just copy the HH script in a directory within the PATH. The script has same dependencies:

- __docopt__, a python library to handle command line arguments easily.
- __xdotool__, to grab X windows.
- __imagemagick__, to build gif files.
- __ffmpeg__, to manupulate audio and videos files.
- __mimic__, for the speech synthesis.

## Issues

The code is written quickly and has some issues. These are the first that came to mind:

- There is a poor error handling. If an error arise things are not done and no message is reported.
- block commands cannot be more than a single line.
- Images for gifs and videos are grabbed from an X window. This means that the focus should stay on that window when HH runs.
