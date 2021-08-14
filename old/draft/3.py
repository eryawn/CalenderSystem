
import os
import re
class Coding:
	def __init__(self,input):
		self.input=input
		self.output=self.nameChanging(self.input)

	def nameChanging(self,file_name):
		output='./'+os.path.basename(file_name)
		output=output[:len(output)-3]
		if ('txt'==file_name[len(file_name)-3:]):
			return output+'mtf'
		elif('mtf'==file_name[len(file_name)-3:]):
			return output+'txt'
		else:
			return None

	def code_cal(self,word,word_array):
		if(word in word_array):
			code=len(word_array)-word_array.index(word)
		else:
			code=len(word_array)+1
		return code

	def code_encode(self,code):
		if(code<=120):
			encoded_code=chr(code+128)
		elif(code>120 and code<=375):
			encoded_code=chr(121+128)+chr(code-121)
		else:
			encoded_code=chr(122+128)+chr((code-376)//256)+chr((code-376)%256)
		return encoded_code

	def delete_codepart(self,ele):
		if(ele[0]==chr(249)):
			ele_part=ele[2:]
		elif(ele[0]==chr(250)):
			ele_part=ele[3:]
		else:
			ele_part=ele[1:]
		return ele_part


	def word_find(self,ele,word_array):
		if(ele[0]==chr(249)):
			duplicated_word=word_array[(ord(ele[1])+121)*-1]
		elif(ele[0]==chr(250)):
			duplicated_word=word_array[(ord(ele[1])*256+ord(ele[2])+376)*-1]
		else:
			duplicated_word=word_array[(ord(ele[0])-128)*-1]
		return duplicated_word


	def encode(self):
		print("In encode_main")

		all_word=[]

		input_file=open(self.input,mode='r',newline="")
		output_file=open(self.output,mode='w',encoding='latin-1',newline="")

		print("{:c}{:c}{:c}{:c}".format(0xba,0x5e,0xba,0x12),end='',file=output_file)
		
		for line in input_file:    
			line=line.strip()       
			if(line==''):           
				line_word=[]                 
			else:                    
				line_word=line.split(' ')

			
			for ele in line_word:        
			
				code=self.code_cal(ele,all_word)

				encoded_code=self.code_encode(code)
				print(encoded_code,end='',file=output_file)

				if(ele in all_word):
					all_word.remove(ele)
					all_word.append(ele)       
				else:
					all_word.append(ele) 
					print(ele,end='',file=output_file)
			

			print("",file=output_file)

		input_file.close()
		output_file.close()                    




	def decode(self):
		print("In decode_main")
	 
		input_file=open(self.input,mode='r',encoding='latin-1',newline="")
		output_file=open(self.output,mode='w',newline="")

		#if no magic numbers at the beginning, exit
		magic_num=input_file.read(4)
		if(re.match('\xba\x5e\xba(\x11|\x12)',magic_num)):
			print("not a proper file, lack of magic number")
			exit(1)

		word_obj=re.compile('([\x20-\x80]+)')
		all_word=[]
		text=input_file.read();
		single_word_mode='((?:(?:(?:\xf9(?:.|\n))|(?:\xfa(?:.|\n)(?:.|\n))|(?:[\x80-\xf8]))[\x20-\x80]*)|(?:[\n\r]|(?:\r\n)))'
	
		text_list=re.findall(single_word_mode,text)
		i=0
		while (i<len(text_list)):
				ele=text_list[i]
				if(re.match('[\r\n]+',ele)):
					i+=1
					print("",file=output_file)
					continue
		
				ele_without_code=self.delete_codepart(ele)

				word_match_obj=word_obj.search(ele_without_code)

				if(re.match('[\r\n]+',text_list[i+1])):
					postfix=""
				else:
					postfix=" "

				if word_match_obj:
					print(word_match_obj.group(1)+postfix,end='',file=output_file)
					all_word.append(word_match_obj.group(1))
				else:
					corresponding_word=self.word_find(ele,all_word)
					print(corresponding_word+postfix,end='',file=output_file)
					all_word.append(corresponding_word)
					all_word.remove(corresponding_word)
				i+=1
		
		input_file.close()
		output_file.close()                                  #close all files




def encode(input_file):
	c=Coding(input_file)
	c.encode()

def decode(input_file):
	c=Coding(input_file)
	c.decode()




