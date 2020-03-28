#!/bin/bash

source ./common.sh

rm -rf Example01
reset 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'mkdir Example01'
mkdir Example01

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'cd Example01'
cd Example01

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'cat <<EOF > program.asm'
echo 'clr r0'
echo 'inc r0'
echo 'r2o r0 o0'
echo 'j 1'
echo 'EOF'
cat <<EOF > program.asm
clr r0
inc r0
r2o r0 o0
j 1
EOF

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'procbuilder -register-size 8 -registers 2 -outputs 1 -opcodes clr,cpy,inc,j,r2o,rset -save-machine processor -input-assembly program.asm'
procbuilder -register-size 8 -registers 2 -inputs 0 -outputs 1 -opcodes clr,cpy,inc,j,r2o,rset -save-machine processor -input-assembly program.asm

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'ls'

# Wait keypress
read -n 1 -s -r

# Command result
echo -e "\033[31m\033[11D[ Output  > \033[0m"
ls

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'procbuilder -load-machine processor -show-program-disassembled'

# Wait keypress
read -n 1 -s -r 

# Command result
echo -e "\033[31m\033[11D[ Output  > \033[0m"
procbuilder -load-machine processor -show-program-disassembled

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'procbuilder -load-machine processor -show-program-binary'

# Wait keypress
read -n 1 -s -r 

# Command result
echo -e "\033[31m\033[11D[ Output  > \033[0m"
procbuilder -load-machine processor -show-program-binary

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'procbuilder -h'

# Wait keypress
read -n 1 -s -r 

# Command result
echo -e "\033[31m\033[11D[ Output  > \033[0m"
procbuilder -h 2>&1 | grep "  -"

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'procbuilder -load-machine processor -create-verilog'
procbuilder -load-machine processor -create-verilog

# Wait keypress
read -n 1 -s -r 

# Command execution
echo -en "\033[32m\033[11D[ Command > \033[0m"
echo 'ls'

# Wait keypress
read -n 1 -s -r

# Command result
echo -e "\033[31m\033[11D[ Output  > \033[0m"
ls

# Wait keypress
read -n 1 -s -r 


cd ..
#rm -rf Example01
