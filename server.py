import serial
import tornado.websocket
import tornado.ioloop
 
 
ser = serial.Serial(2)   # open serial COM3 (change this to whatever port your uC is connected to)
print(ser.portstr)       # check which port was really used (issues on windows)
 
line = 0
listeners = [] #list that will hold all the current connections

#handles all the connections.
class WebSocketHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        listeners.append(self)
    def on_close(self):
        listeners.remove(self)
 
 
application = tornado.web.Application([
	(r"/websocket", WebSocketHandler),
])
 
def main():
    global line
    line=ser.readline()
    #print(listeners) <-- for debugging connection
    for i in range(0, len(listeners)):
        listeners[i].write_message(str(line, 'utf-8'))
 
if __name__ == "__main__":
    application.listen(8888)
    while(1):
        tornado.ioloop.IOLoop.instance().run_sync(main)
