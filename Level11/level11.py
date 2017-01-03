import socket
import random
import string
import pdb

IP_LOW_OCT = 1
IP_HIGH_OCT = 255

PORT_LOW = 1
PORT_HIGH = 65535

MAX_RANDOM_GARBAGE_PACKET_COUNT = 30
MIN_GARBAGE_PACKET_LENGTH = 15
MAX_GARBAGE_PACKET_LENGTH = 30

PASSWORD_LENGTH = 12
PASSWORD_INPUT_TRIES = 3
class Stage():
	def __init__(self, id, message, ip = None, port=None):
		self.id = id
		
		if ip is None:
			self.ip = rand_ip()
		else:
			self.ip = ip
		
		if port is None:
			self.port = rand_port()
		else:
			self.port = port
		
		self.message = message

def rand_ip_oct():
	return random.randint(IP_LOW_OCT, IP_HIGH_OCT)
	
def rand_ip():
	return "{}.{}.{}.{}".format(rand_ip_oct(),rand_ip_oct(),rand_ip_oct(),rand_ip_oct())
	
def rand_port():
	return random.randint(PORT_LOW, PORT_HIGH)

def rand_garbage_message():
	garbage_message_length = random.randint(MIN_GARBAGE_PACKET_LENGTH, MAX_GARBAGE_PACKET_LENGTH) 
	return rand_string(garbage_message_length)
	
def rand_string(length):
	random_string = ""
	
	for i in range(length):
		random_string += random.choice(string.ascii_letters)
	return random_string
	
def send_garbage_packet():
	ip_garbage_packet = rand_ip()
	port_garbage_packet = rand_port()
	
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
	sock.sendto(rand_garbage_message(), (ip_garbage_packet, port_garbage_packet))
	sock.close()
	

def send_garbage_packets(stage):
	garbage_packet_count = random.randint(4,MAX_RANDOM_GARBAGE_PACKET_COUNT)
	#print "Sending {} garbage packets".format(garbage_packet_count)
	
	send_stage_message_iteration = random.randrange(garbage_packet_count)
	
	for i in range(garbage_packet_count):
		if (i == send_stage_message_iteration):
			print "Stage{} Ip:{}, Port:{}".format(stage.id, stage.ip, stage.port)
			
			#print stage.message	
			while True:
				try:
					sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
					sock.sendto(stage.message, (stage.ip, stage.port))
					sock.close()
					break
				except socket.error, msg :
					pass
				
		try:
			send_garbage_packet()
		except socket.error, msg :
			pass



password = rand_string(PASSWORD_LENGTH)

stage_list = []
next_stage_ip = ""
next_stage_port = ""
current_stage_ip = ""
current_stage_port = ""

next_stage_ip = rand_ip()
next_stage_port = rand_port()
stage1_msg = "Hello there! Try to follow the trail. Next clue is in the packet IP:{}, Port {}".format(next_stage_ip, next_stage_port)
stage1 = Stage(1, stage1_msg)

current_stage_ip = next_stage_ip
current_stage_port = next_stage_port
next_stage_ip = rand_ip()
next_stage_port = rand_port()

stage2_msg = "You found me! Next clue is in the packet IP:{}, Port {}".format(next_stage_ip, next_stage_port)
stage2 = Stage(2, stage2_msg, current_stage_ip, current_stage_port)

for i in range(PASSWORD_LENGTH):
	current_stage_ip = next_stage_ip
	current_stage_port = next_stage_port
	next_stage_ip = rand_ip()
	next_stage_port = rand_port()
	

	stage_msg = "The {}th letter of the password is '{}'. Next clue is packet IP:{}, Port {}".format(i+1, password[i], next_stage_ip, next_stage_port) 
	stage = Stage(i+3, stage_msg, current_stage_ip, current_stage_port)
	stage_list.append(stage)

print "Stage{} Ip:{}, Port:{}".format(stage1.id, stage1.ip, stage1.port)
while True:
	try:
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
		sock.sendto(stage1.message, (stage1.ip, stage1.port))
		sock.close()					
		break
	except socket.error, msg :
		pass

send_garbage_packets(stage2)

for i in range(PASSWORD_LENGTH):
	send_garbage_packets(stage_list[i])

for i in range(PASSWORD_INPUT_TRIES):
	user_password = raw_input("Enter 12 letter password (case-sensitive): ")
	if (user_password == password):
		print "OK!"
		break
	else:
		print "Failed! {} more tries left!".format(PASSWORD_INPUT_TRIES - i - 1)

