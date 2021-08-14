#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

#namechange function, convert .mtf to .txt and vice versa

def namechanging(file_name):
	output=file_name[:len(file_name)-3]
	if ('txt'==file_name[len(file_name)-3:]):
		return output+'mtf'
	elif('mtf'==file_name[len(file_name)-3:]):
		return output+'txt'
	else:
		return None





def encode_main():
	print("In encode_main")
	word=[]
	if(len(sys.argv)==1):
		print("no arguments")
		exit(1)
	input_file=open(sys.argv[1],'r')
	output_file=open(namechanging(sys.argv[1]),mode='w',encoding='latin-1') 
	print("{:c}{:c}{:c}{:c}".format(0xba,0x5e,0xba,0x11),end='',file=output_file) #input magic numbers
	for line in input_file:    #read text line by line
		line=line.strip()       #remove new line signal
		if(line==''):
			a=[]                 #if the line is empty, then just print new line
		else:                   #if is not empty, split the line using ' '
			a=line.split(' ')
		for ele in a:           #read word one by one
			if(ele in word):
				print(chr((len(word)-word.index(ele)+128)),end='',file=output_file)
				word.remove(ele)
				word.append(ele)          #if the word has already been in the array, print the number, remove the previous one and 
			else:                        #put the new one in
				word.append(ele)                                          #if its a new word, print the number and the word char by cahr
				print(chr((len(word)+128)),end='',file=output_file)
				for c in ele:
					print(c,end='',file=output_file)
		print("",file=output_file)
	input_file.close()
	output_file.close()                    #close all files





def decode_main():
	print("In decode_main")
 
	if(len(sys.argv)==1):
		print("no arguments")
		exit(1)
	input_file=open(sys.argv[1],'r',encoding='latin-1')
	output_file=open(namechanging(sys.argv[1]),mode='w')
	if(input_file.read(4)!="{:c}{:c}{:c}{:c}".format(0xba,0x5e,0xba,0x11)):     #if no magic numbers at the beginning, exit
		exit(1)
	a=input_file.read(1)
	word=[]
	num=0

	while(a!=''):                #when arriving the end of the file, stop
		if(a=='\n'):                                     #if the char is a new line signal, write it into the output file
			print("\n",end='',file=output_file)
			a=input_file.read(1)                    
		elif(ord(a)==num+129):                           #if the char read is a continuous number, then put the word after 
			a=input_file.read(1)                          #the number into output file
			s=''
			while(ord(a)<128 and a!='\n'):
				s=s+a;
				a=input_file.read(1)
				if(len(a)==0):
					break
			print(s,end='',file=output_file)
			if(a!="\n"):
				print(" ",end='',file=output_file)
			word.append(s.strip())
			num+=1
		else:                                            #if the char is not a continuous number, then find word according to 
			str=word[(ord(a)-128)*-1]                     #the number in the array, and put the word in the output file
			a=input_file.read(1)
			print(str,end='',file=output_file)
			if(a!="\n"):
				print(" ",end='',file=output_file)
			word.append(str)
			word.remove(str)
	input_file.close()
	output_file.close()                                  #close all files

	



command = os.path.basename(__file__)
if __name__ == "__main__" and command == "text2mtf.py":
	encode_main()
elif __name__ == "__main__" and command == "mtf2text.py":
	decode_main()

