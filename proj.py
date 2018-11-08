import sys
import json
import string

register = {"rdi": 0,
               "rsi": 0,
               "rdx": 0,
               "rcx": 0,
               "r8": 0,
               "r9": 0,
               "rax": 0,
               "rbx": 0,
               "r10": 0,
               "r11": 0,
               "r12": 0,
               "r13": 0,
               "r14": 0,
               "r15": 0,
               "rbp": 0,
               "rsp": 0,
               "rip": 0
               }



memory = {}


def registerToRegister(dest, val):
    
    print("r2T:", dest, val)
    
    register[dest] = register[val]
    
def registerToPointer(dest, val):
    
    print("r2p:", dest, val)
    
    memory[val[11:-1]]['value'] = register[val]  
    
def pointerToRegister(dest, val):
    
    print("p2r:", dest, val)
    
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = memory[val[11:-1]]['value']
    
def numToPointer(dest, val):
    
    print("n2p:", dest, val)
    memory[dest[11:-1]]['value'] = int(val,16)
    
def numToRegister(dest, val):   
    
    print("n2r:", dest, val)
    
    register[dest] = int(val,16)


def isNumber(val):
    print("isN", val)
    return all(c in string.hexdigits for c in val[2:])

def dectectVariableOverflowFunction(prog):
    
    #names = ["<gets@plt>", "<strcpy@plt>", "strcat@plt", "fgets@plt", "strncpy@plt", "strncat@plt"]
    
    
    for var in prog["main"]["variables"]:
        
        memory[var["address"]] = {"value": 0, "type": var["type"], "bytes": var["bytes"]}
        
        
        '''for y in x:
            
            if y == "address":
                print(x[y])
                #memory[x[y]] = x'''
    
    for instr in prog["main"]["instructions"]:
        if instr['op'] == "mov":
            
            destination = instr['args']['dest']
            value = instr['args']['value']
            
            if destination == "esi":
                    destination = "rsi" #esi = rsi
            
            if (destination in register.keys()) and (value in register.keys()):
                
                registerToRegister(destination,value)
                      
            elif (destination in register.keys()) and (isNumber(value)):
              
                numToRegister(destination,value)
            
            elif (destination in register.keys()) and ("PTR" in value):
                
                pointerToRegister(destination,value)
            
            elif ("PTR" in destination) and (value in register.keys()):
                
                registerToPointer(destination,value)
                
            elif ("PTR" in destination) and (isNumber(value)):
                
                numToPointer(destination,value)
            
    print(register)
    print(memory)
            
    
            
            
            
    
    
    '''for fun in prog:
        print(fun)
        aux = prog[fun]
        for stuff in aux["variables"]:
            print("->" + stuff) 
            aux2 = prog[fun][stuff]
                for di in aux2:
                    for keys in di:
                        print(keys)
                        if keys == "name":
                            print()
                
'''
                #print(prog[fun][stuff])
                    #name = fun + stuff
                    #print(name)
    
    
    #print(memory)
    
    
def readJson(file):
    print("Reading file %s" % file)
    f = open(file, 'r')
    file_contents = f.read()
    f.close()
    return json.loads(file_contents)

def usage(progName):
    print('Usage:')
    print(' python %s <program>.json' % progName)
    sys.exit()

if __name__ == '__main__':
    if len(sys.argv) < 2:
        usage(sys.argv[0])

    #print(readFile(sys.argv[1]))
    
    prog = readJson(sys.argv[1])

    dectectVariableOverflowFunction(prog)