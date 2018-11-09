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
    "rbp": int("0x7fffffff", 16),
    "rsp": int("0x7fffffff", 16),
    "rip": 0
}

memory = {}

vulnerabilities = []

def detectFgetsVuln(instruction, function):

    bufferAddress = register["rdi"]
    size = register["rsi"]
    

    if bufferAddress in memory.keys():
        bufferSize = memory[bufferAddress]["bytes"]
        memory[bufferAddress]["value"] = size
        
        return size > bufferSize

def detectStrcpyVuln(instruction, function):

    dest = register["rdi"]
    src = register["rsi"]

    destSize = memory[dest]["bytes"]
    srcSize = memory[src]["bytes"]

    memory[dest]["value"] = srcSize

    return destSize < srcSize




def addVarOver(instruction, function, offset, buffer, fnname):
    
    print("instruction", instruction)
    
    return [{
            "fnname": fnname,
            "vuln_function": function,
            "overflown_var": memory[offset]["name"],
            "vulnerability": "VAROVERFLOW",
            "overflow_var": memory[buffer]["name"],
            "address": instruction["address"]
            }]


def identifyAllVariables(instruction, function):
    
    found = []
    
    buffer = str(register["rdi"])
    address = buffer[4:]
    intOffset = int(address,16)
    print("buffer :",address)
    
    for off in memory:
        offset = str(off)
        if "0x" in offset:
            #offsets nao negativos, por isso pode-se ignorar sinal
            auxIntOffset = int(offset[4:],16) 
            print("auxIntOffer", auxIntOffset)
            print("intOffset", intOffset)
            if(auxIntOffset < intOffset):
                found = found + addVarOver(instruction, function, offset, buffer, "gets")
        
        #if 0x in offset -> take and see if offset is lower than this one
    
    return found
 
def identifyWrittenVariables(instruction, function):
    
    found = []
    
    buffer = str(register["rdi"])
    address = buffer[3:]
    intOffset = int(address,16)
    
    size = register["rsi"]
    
    comparator = intOffset + size
    
    for off in memory:
        offset = str(off)
        if "0x" in offset:
            if offset[3:] != address:
                if int(offset[3:],16) <= comparator:
                    found = found + addVarOver(instruction, function, offset, buffer, "fgets")
                    print(found)
    return found

def detectVariableOverflow(instruction, function):
    
    result = []
    
    if instruction["args"]["fnname"] == "<gets@plt>":
        result = result + identifyAllVariables(instruction, function)
        
    if instruction["args"]["fnname"] == "<fgets@plt>":
        if(detectFgetsVuln(instruction, function)):
           result = result + identifyWrittenVariables(instruction, function)
           
        
    return result

def addRegisterToRegister(dest, val):
    register[dest] = register[dest] + register[val]
    
def addRegisterToPointer(dest, val):
    memory[val[11:-1]]['value'] = memory[val[11:-1]]['value'] + register[val]  
    
def addPointerToRegister(dest, val):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = register[dest] + memory[val[11:-1]]['value']
    
def addNumToPointer(dest, val):
    memory[dest[11:-1]]['value'] = memory[dest[11:-1]]['value'] + int(val,16)
    
def addNumToRegister(dest, val):   
    register[dest] = register[dest] + int(val,16)

def subNumToRegister(dest, val):   
    register[dest] = register[dest] - int(val,16)

def subRegisterToRegister(dest, val):
    register[dest] = register[dest] - register[val]
    
def subRegisterToPointer(dest, val):
    memory[val[11:-1]]['value'] = memory[val[11:-1]]['value'] - register[val]  
    
def subPointerToRegister(dest, val):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = register[dest] - memory[val[11:-1]]['value']
    
def subNumToPointer(dest, val):
    memory[dest[11:-1]]['value'] = memory[dest[11:-1]]['value'] - int(val,16)
    

def movRegisterToRegister(dest, val):
    register[dest] = register[val]
    
def movRegisterToPointer(dest, val):
    memory[val[11:-1]]['value'] = register[val]  
    
def movPointerToRegister(dest, val):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = memory[val[11:-1]]['value']
    
def movNumToPointer(dest, val):
    memory[dest[11:-1]]['value'] = int(val,16)
    
def movNumToRegister(dest, val):   
    register[dest] = int(val,16)

def isNumber(val):
    return all(c in string.hexdigits for c in val[2:])

def initializeLocalVariables(program, function):

    for var in program[function]["variables"]: 
        memory[var["address"]] = {"name": var["name"], "value": 0, "type": var["type"], "bytes": var["bytes"] }

def mov(destination, value):

    print("mov", destination, value)
    
    if destination == "esi":
        destination = "rsi" #esi = rsi

    if destination == "eax":
        destination = "rax" #eax = rax
            
    if (destination in register.keys()) and (value in register.keys()):
        
        movRegisterToRegister(destination,value)
                
    elif (destination in register.keys()) and (isNumber(value)):
        
        movNumToRegister(destination,value)
    
    elif (destination in register.keys()) and ("PTR" in value):
        
        movPointerToRegister(destination,value)
    
    elif ("PTR" in destination) and (value in register.keys()):
        
        movRegisterToPointer(destination,value)
        
    elif ("PTR" in destination) and (isNumber(value)):
        
        movNumToPointer(destination,value)

def add(destination, value):
    
    print("add", destination, value)

    if destination == "esi":
        destination = "rsi" #esi = rsi

    if destination == "eax":
        destination = "rax" #eax = rax
            
    if (destination in register.keys()) and (value in register.keys()):
        
        addRegisterToRegister(destination,value)
                
    elif (destination in register.keys()) and (isNumber(value)):
        
        addNumToRegister(destination,value)
    
    elif (destination in register.keys()) and ("PTR" in value):
        
        addPointerToRegister(destination,value)
    
    elif ("PTR" in destination) and (value in register.keys()):
        
        addRegisterToPointer(destination,value)
        
    elif ("PTR" in destination) and (isNumber(value)):
        
        addNumToPointer(destination,value)

def sub(destination, value):

    print("sub", destination, value)
    
    if destination == "esi":
        destination = "rsi" #esi = rsi

    if destination == "eax":
        destination = "rax" #eax = rax
            
    if (destination in register.keys()) and (value in register.keys()):
        
        subRegisterToRegister(destination,value)
                
    elif (destination in register.keys()) and (isNumber(value)):
        
        subNumToRegister(destination,value)
    
    elif (destination in register.keys()) and ("PTR" in value):
        
        subPointerToRegister(destination,value)
    
    elif ("PTR" in destination) and (value in register.keys()):
        
        subRegisterToPointer(destination,value)
        
    elif ("PTR" in destination) and (isNumber(value)):
        
        subNumToPointer(destination,value)

def lea(destination, value):

    print("lea", destination, value)

    register[destination] = value[1:-1]

def push(value):

    print("push", value)

    subNumToRegister("rsp", "0x8")

    memory[register["rsp"]] = { "value": register[value] }

def leave():

    print("leave")

    movRegisterToRegister("rsp", "rbp")
    
    register["rbp"] = memory[register["rsp"]]["value"]
    memory.pop(register["rsp"], None)

def runInstruction(instr):
        
    if instr['op'] == "mov":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        mov(destination, value)
    
    elif instr['op'] == "add":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        add(destination, value)

    elif instr['op'] == "sub":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        sub(destination, value)

    elif instr['op'] == "lea":
        
        destination = instr['args']['dest']
        value = instr['args']['value']

        lea(destination, value)

    elif instr['op'] == "push":
        
        value = instr["args"]["value"]
        push(value)

    elif instr["op"] == "leave":
        leave()
        
    
def detectRBPOverflow(instruction, function):

    rbpVulnerability = []

    '''
    genericVuln = {
                "vulnerability": "RBPOVERFLOW",
                "vuln_function": function,
                "address": instruction["address"],
                "fnname" : "gets",
                "overflow_var" : memory[register["rdi"]]["name"]
    }
    '''

    vuln = {
        "vulnerability": "RBPOVERFLOW",
        "vuln_function": function,
        "address": instruction["address"]
    }

    if instruction["args"]["fnname"] == "<gets@plt>":
        
        vuln["fnname"] = "gets"
        rbpVulnerability.append(vuln)

    elif instruction["args"]["fnname"] == "<fgets@plt>":

        hasFgetsVuln = detectFgetsVuln(instruction, function)

        if hasFgetsVuln:
            bufferAddress = str(register["rdi"])
            bufferSize = register["rsi"]

            rbpOffset = int(bufferAddress[3:], 16) + bufferSize

            if rbpOffset >= 0:
                
                vuln["fnname"] = "fgets"
                vuln["overflow_var"] = memory[register["rdi"]]["name"]
                rbpVulnerability.append(vuln)

    return rbpVulnerability

def analyzeStrcpy():
	return

def detectRETOverflow(instruction, function):

		print("RETOV", function)

		dangerousFunc = instruction["args"]["fnname"]

		RETOverflowVulnerability = []
		
		vulnerabilityObj = {
				"vulnerability": "RETOVERFLOW",
	      "vuln_function": function,
	      "address": instruction["address"],
		}

		if dangerousFunc == "<strcpy@plt>":
			analyzeStrcpy()

		elif dangerousFunc == "<strcat@plt>":
			return

		elif dangerousFunc == "<fgets@plt>":
			return

		elif dangerousFunc == "<strncpy@plt>":
			return

		elif dangerousFunc == "<strncat@plt>":
			return

		else:
			vulnerabilityObj["fnname"] = "gets"
			vulnerabilityObj["overflow_var"] = str(register["rdi"])

			RETOverflowVulnerability.append(vulnerabilityObj)
	
		return RETOverflowVulnerability

def analyzeCall(instruction, function):

    global vulnerabilities

    print("call", instruction["args"]["fnname"])

    dangerousFunctions = ["<gets@plt>", "<strcpy@plt>", "<strcat@plt>", "<fgets@plt>", "<strncpy@plt>", "<strncat@plt>"]

    if instruction["args"]["fnname"] in dangerousFunctions:
        vulnerabilities = vulnerabilities + detectRBPOverflow(instruction, function)

        vulnerabilities = vulnerabilities + detectVariableOverflow(instruction, function)

        vulnerabilities = vulnerabilities + detectRETOverflow(instruction, function)

def runFunction(program, function):
    
    initializeLocalVariables(program, function)
    
    for instr in program[function]["instructions"]:

        if instr['op'] == "nop":
            print("nop")
            continue
        
        elif instr['op'] == "call":
            analyzeCall(instr, function)

        elif instr['op'] == "ret":
            print("ret")
            return

        runInstruction(instr)

        print(memory)
        print(register, "\n")

def writeJson(progName, vulnerabilities):
    f = open("%s.output.json" % progName, 'w+')
    f.write(json.dumps(vulnerabilities, indent=4, separators=(',', ': ')))
    f.close()
    
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
    runFunction(program, "main")
    #writeJson(sys.argv[1], vulnerabilities)
    print ("vulnerabilities", vulnerabilities)
