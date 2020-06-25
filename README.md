###############################################################
#							      #
#			BRAINFUCK			      #
#							      #
###############################################################


Commands:


-> ">" : increase stack pointer

-> "<" : decrease stack pointer

-> "+" : increment value at current byte

-> "-" : decrement value at current byte

-> "." : output ascii code at current byte

-> "," : ask for input

-> "[" : start of loop

-> "]" : end of loop


To compile: ./brainfuck ${filename}
 
./brainfuck hello.bf
hello world!

Use this at your own risk

