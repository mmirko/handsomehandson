#!/bin/bash
 
rm -rf Example01
reset 

#handsomeh_begin {}
mkdir Example01
#handsomeh_end

#handsomeh_begin {}
cd Example01
#handsomeh_end

#handsomeh_begin {}
cat <<EOF > program.asm
clr r0
inc r0
r2o r0 o0
j 1
EOF
#handsomeh_end

#handsomeh_begin {}
procbuilder -register-size 8 -registers 2 -inputs 0 -outputs 1 -opcodes clr,cpy,inc,j,r2o,rset -save-machine processor -input-assembly program.asm
#handsomeh_end

#handsomeh_begin {}
ls
#handsomeh_end

#handsomeh_begin {}
procbuilder -load-machine processor -show-program-disassembled
#handsomeh_end

#handsomeh_begin {}
procbuilder -load-machine processor -show-program-binary
#handsomeh_end

#handsomeh_begin {}
procbuilder -h 
#handsomeh_end

#handsomeh_begin {}
procbuilder -load-machine processor -create-verilog
#handsomeh_end

#handsomeh_begin {}
ls
#handsomeh_end


cd ..
rm -rf Example01
