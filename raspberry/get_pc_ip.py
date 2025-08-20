import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = socket.gethostname()
port = 1717
s.bind(('',port))
s.listen(1)

c,addr = s.accept()
print(addr[0])
import clientR
clientR.start_threading(str(addr[0]))
