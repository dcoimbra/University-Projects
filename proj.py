import sys
import json
import string

register = {
    "rdi": 0,
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
    "rsp": hex(sys.maxsize),
    "rip": 0
}

memory = {}

def registerToRegister(dest, val):
    register[dest] = register[val]
    
def registerToPointer(dest, val):
    memory[val[11:-1]]['value'] = register[val]  
    
def pointerToRegister(dest, val):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = memory[val[11:-1]]['value']
    
def numToPointer(dest, val):
    memory[dest[11:-1]]['value'] = int(val,16)
    
def numToRegister(dest, val):   
    register[dest] = int(val,16)

def isNumber(val):
    return all(c in string.hexdigits for c in val[2:])

def initializeLocalVariables(program, function):

    for var in program[function]["variables"]: 
        memory[var["address"]] = {"value": 0, "type": var["type"], "bytes": var["bytes"]}

def mov(destination, value):
    
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


def detectVariableOverflowFunction(program, function):
    
    initializeLocalVariables(program, function)
    
    for instr in program[function]["instructions"]:
        if instr['op'] == "mov":
            
            destination = instr['args']['dest']
            value = instr['args']['value']
            
            mov(destination, value)
            
def analyzeProgram(program):
    detectVariableOverflowFunction(program, "main")
    #mais funcoes que detetam vulnerabilidades
    
def readJson(file):
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
    
    program = readJson(sys.argv[1])
    analyzeProgram(program)