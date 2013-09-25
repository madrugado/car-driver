import sys
import struct

def interpret(filename):
    with open(filename, "rb") as f:
        while True:
            try:
                sec = f.read(4)
                s = str(struct.unpack("<l", sec)[0])
                usec = f.read(4)
                s += "." + str(struct.unpack("<l", usec)[0])
                
                command = f.read(2)
                c = (struct.unpack("<B", command[0]))[0]
                if c == 0x01:
                    s += " FORWARD"
                if c == 0x02:
                    s += " BACKWARD"
                if c == 0x04:
                    s += " LEFT"
                if c == 0x08:
                    s += " RIGHT"
                if c == 0x10:
                    s += " STOP"
                if c == 0x20:
                    s += " STRAIGHT"
                if c == 0x80:
                    s += " CONTINUE"
                
                distance = f.read(8)
                s += " " + str(struct.unpack("<d", distance)[0])
                s += "\n"
            except:
                s = None
            
            if s:
                yield s
            else:
                break

def main():
    if len(sys.argv) < 2:
        print "Usage: bin2textlog.py logfile\nOutput: logfile.txt"
        return 0
    with open(sys.argv[1] + ".txt", "wt") as f:
        for s in interpret(sys.argv[1]):
            f.write(s)

if __name__=="__main__":
    main()