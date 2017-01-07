import socket
import random
import string
import pdb
import sys
import time


IP_LOW_OCT = 1
IP_HIGH_OCT = 255

PORT_LOW = 1
PORT_HIGH = 65535

IP_DICT = {
"64.233.{}.{}" : range(0,191),
"66.102.{}.{}" : range(0,15),
"66.249.{}.{}" : range(64,95),
"72.14.{}.{}" : range(192,255),
"74.125.{}.{}" : range(0,255),
"209.85.{}.{}" : range(128,255),
"216.239.{}.{}" : range(32,63)}

PROTOCOL_LIST = [socket.SOCK_DGRAM, socket.SOCK_STREAM]



MIN_RANDOM_GARBAGE_PACKET_COUNT = 30
MAX_RANDOM_GARBAGE_PACKET_COUNT = 45
MIN_GARBAGE_PACKET_LENGTH = 30
MAX_GARBAGE_PACKET_LENGTH = 60

PASSWORD_LENGTH = 12
PASSWORD_INPUT_TRIES = 3
class Stage():
    supported_encodings = ["hex", "base64", "utf-16", "zlib", "zip"]
    def __init__(self, id, message, encode_message_flag=True, ip = None, port=None):
        self.id = id
        
        if ip is None:
            self.ip = rand_ip()
        else:
            self.ip = ip
        
        if port is None:
            self.port = rand_port()
        else:
            self.port = port
        
        self.encoding_type = None
        self.message = message
        
        if encode_message_flag == True:
            self.encoding_type = random.choice(self.supported_encodings)
            self.__encode_message()
        
    def __encode_message(self):
        self.message = "Message encoded in '{}'. Encoded message:{}".format(self.encoding_type, self.message.encode(self.encoding_type))

def rand_ip_oct():
    return random.randint(IP_LOW_OCT, IP_HIGH_OCT)
    
def rand_ip():
    ip_base = random.choice(IP_DICT.keys())
    ip = ip_base.format(random.choice(IP_DICT[ip_base]),rand_ip_oct())
    return ip
    #return "{}.{}.{}.{}".format(rand_ip_oct(),rand_ip_oct(),rand_ip_oct(),rand_ip_oct())
    
def rand_port():
    return random.randint(PORT_LOW, PORT_HIGH)
    
def rand_protocol():
    return random.choice(PROTOCOL_LIST)

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

def gen_sock_ip_port():
    ip = None
    port = None
    while True:
        try:
            ip = rand_ip()
            port = rand_port()

            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
            # sock = socket.socket(socket.AF_INET, rand_protocol()) 
            sock.settimeout(3)
            sock.connect((ip, 80))
            sock.close()
            
            break
        except Exception as e:
            print "Host {} not found (error {})".format(ip,e)
            
            pass
    
    return ip,port

def send_garbage_packets(stage):
    garbage_packet_count = random.randint(MIN_RANDOM_GARBAGE_PACKET_COUNT,MAX_RANDOM_GARBAGE_PACKET_COUNT)
    #print "Sending {} garbage packets".format(garbage_packet_count)
    
    send_stage_message_iteration = random.randrange(garbage_packet_count)
    
    for i in range(garbage_packet_count):
        if (i == send_stage_message_iteration):
            # print "Iteration: {}, Send Iteration:{}".format(i, send_stage_message_iteration)
            # print "Stage{} Ip:{},Port:{}".format(stage.id, stage.ip, stage.port)          
            try:
                time.sleep(1)
                sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
                # sock = socket.socket(socket.AF_INET, rand_protocol()) 
                sock.connect((stage.ip, stage.port))
                sock.send(stage.message)
                sock.close()
                break
            except socket.error, msg :
                print "Unable to complete action. Exiting..."
                sys.exit(1)
                
                
        try:
            # a =1
            send_garbage_packet()
        except socket.error, msg :
            pass



password = rand_string(PASSWORD_LENGTH)

stage_list = []
next_stage_ip = ""
next_stage_port = ""
current_stage_ip = ""
current_stage_port = ""


# print "Creating stage1"
next_stage_ip, next_stage_port = gen_sock_ip_port()
stage1_msg = "Hello there! Try to follow the trail. Next clue is in the packet IP:{}, Port {}".format(next_stage_ip, next_stage_port)
stage1 = Stage(1, stage1_msg, encode_message_flag=False)
# print "Stage1 created"


# print "Creating stage2"
current_stage_ip = next_stage_ip
current_stage_port = next_stage_port
next_stage_ip, next_stage_port = gen_sock_ip_port()

stage2_msg = "You found me! Next clue is in the packet IP:{}, Port {}".format(next_stage_ip, next_stage_port)
stage2 = Stage(2, stage2_msg, False, current_stage_ip, current_stage_port)
# print "Stage2 created"

i = 0
for i in range(PASSWORD_LENGTH):
    # print "Generating stage {}".format(i + 3)
    
    current_stage_ip = next_stage_ip
    current_stage_port = next_stage_port
    next_stage_ip, next_stage_port = gen_sock_ip_port()
    
    
    stage_msg = "The {}th letter of the password is '{}'.".format(i+1, password[i])
    if i != PASSWORD_LENGTH - 1:
        stage1_msg += " Next clue is packet IP:{}, Port {}".format(next_stage_ip, next_stage_port) 
    else:
        stage1_msg += " This is the last letter of the password."
        
    stage = Stage(i+3, stage_msg, True, current_stage_ip, current_stage_port)
    stage_list.append(stage)
    # print "Stage {} generated".format(i + 3)

# print "Stage{} Ip:{}, Port:{}".format(stage1.id, stage1.ip, stage1.port)
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
    sock.connect((stage1.ip, stage1.port))
    sock.send(stage1.message)
    sock.close()                    
except socket.error, msg :
    print "Unable to complete action. Exiting..."
    sys.exit(1)
        

send_garbage_packets(stage2)

i = 0
for i in range(PASSWORD_LENGTH):
    random_stage = random.randint(0,PASSWORD_LENGTH - i -1)
    stage = stage_list[random_stage]
    stage_list.remove(stage)
    send_garbage_packets(stage)


for i in range(PASSWORD_INPUT_TRIES):
    user_password = raw_input("Enter 12 letter password (case-sensitive): ")
    if (user_password == password):
        print "OK!"
        break
    else:
        print "Failed! {} more tries left!".format(PASSWORD_INPUT_TRIES - i - 1)

