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

def isVulnerable(destBufferSize,valueToInsert):
    
    destAddr = register["rdi"]
    dest = memory[destAddr]["value"]
    
    memory[destAddr]["value"] = dest + valueToInsert
    
    return valueToInsert > destBufferSize

def detectFgetsVuln(instruction, function):

    bufferAddress = register["rdi"]
    bytesToGet = register["rsi"]
    
    bufferSize = memory[bufferAddress]["bytes"]
        
    memory[bufferAddress]["value"] = bytesToGet
        
    return bytesToGet > bufferSize

def detectStrcpyVuln(instruction, function):

    dest = register["rdi"]
    src = register["rsi"]

    destSize = memory[dest]["bytes"]
    srcSize = memory[src]["value"]
    
    memory[dest]["value"] = srcSize
        
    return destSize < srcSize


def detectStrncpyVuln(instruction, function):

    destAddr = register["rdi"]
    sizeToCpy = register["rdx"]
    
    destSize = memory[destAddr]["bytes"]
       
    memory[destAddr]["value"] = sizeToCpy
    
    return sizeToCpy > destSize

    
def detectStrcatVuln(instruction, function):
        
    destAddr = register["rdi"]
    srcAddr = register["rsi"]
    
    dest = memory[destAddr]["value"]
    src = memory[srcAddr]["value"]
    
    destBufferSize = memory[destAddr]["bytes"]
 
    #Bytes written in destination buffer minus \0 plus
    #Bytes written in source buffer minus \0 plus
    #\0 to end the string
    resultingBytes = (dest-1) + (src-1) + 1
    
    return isVulnerable(destBufferSize,resultingBytes)


def detectStrncatVuln(instruction, function):
    
    destAddr = register["rdi"]
    size = register["rdx"]
    
    dest = memory[destAddr]["value"]
    destBufferSize = memory[destAddr]["bytes"]
    
    #Bytes written in destination buffer minus \0 plus
    #Bytes in source buffer that are going to be concatenated plus
    #\0 to end the string
    
    resultingBytes = (dest - 1) + size + 1
    
    return isVulnerable(destBufferSize,resultingBytes)
    

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


def addRBPOverflow(instruction, function, buffer, fnname):
    
    return [{
            "vulnerability": "RBPOVERFLOW",
            "overflow_var": memory[buffer]["name"],
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": fnname
            }]


def addRETOverflowOutput(function, instruction, dangerousFunc):

    return {
            "vulnerability": "RETOVERFLOW",
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": dangerousFunc,
            "overflow_var": memory[str(register["rdi"])]["name"]
    }
 
    
def findVariables(instruction, function, comparator, fnname):

    aux = []
    
    buffer = str(register["rdi"])
    
    for off in memory:
        offset = str(off)
        if "0x" in offset:
            if offset[3:] != buffer[3:]: #to avoid the same offset (same buffer)
                if (int(offset[3:],16) <= comparator) and (int(offset[3:],16) > int(buffer[3:],16)): #if it's smaller than the buffer, then it's above it and safe 
                    aux = aux + addVarOver(instruction, function, offset, buffer, fnname)

    return aux


def identifyGetsWrittenVariables(instruction, function):

    found = []
    
    buffer = str(register["rdi"])
    address = buffer[4:]
    intOffset = int(address,16)
    print("buffer :",address)
    
    for off in memory:
        offset = str(off)
        if "0x" in offset:
            #offsets sao negativos, por isso pode-se ignorar sinal
            auxIntOffset = int(offset[4:],16) 
            print("auxIntOffer", auxIntOffset)
            print("intOffset", intOffset)
            if(auxIntOffset < intOffset):
                found = found + addVarOver(instruction, function, offset, buffer, "gets")
        
        #if 0x in offset -> take and see if offset is lower than this one
    
    return found


def identifyFgetsWrittenVariables(instruction, function):
    
    found = []
    
    buffer = str(register["rdi"])
    address = buffer[3:]
    intOffset = int(address,16)
    
    size = register["rsi"]
    
    comparator = intOffset + size
    
    found = findVariables(instruction, function, comparator, "fgets")
    
    return found


def identifyStrcpyWrittenVariables(instruction, function):
    
    found = []
    
    dstBuffer = str(register["rdi"])
    dstAddress = dstBuffer[3:]
    dstIntOffset = int(dstAddress,16)
    
    srcBuffer = str(register["rsi"])
    srcFill = memory[srcBuffer]["value"]
    
    comparator = dstIntOffset + srcFill
    
    found = findVariables(instruction, function, comparator, "strcpy")

    return found


def identifyStrncpyWrittenVariables(instruction, function):
    
    found = []
    
    dstBuffer = str(register["rdi"])
    dstAddress = dstBuffer[3:]
    dstIntOffset = int(dstAddress,16)
    
    sizeToCpy = register["rdx"]
         
    comparator = dstIntOffset + sizeToCpy
    
    found = findVariables(instruction, function, comparator, "strncpy")
    
    return found


def identifyStrcatWrittenVariables(instruction, function):
    
    found = []
    
    dstBuffer = str(register["rdi"])
    dstAddress = dstBuffer[3:]
    dstIntOffset = int(dstAddress,16)
    dstBufferContent = memory[dstBuffer]["value"]
    
    srcBuffer = str(register["rsi"])
    srcBufferContent = memory[srcBuffer]["value"]
    
    
    #Bytes written in destination buffer minus \0 plus
    #Bytes written in source buffer minus \0 plus
    #\0 to end the string
    totalContent = (dstBufferContent-1) + (srcBufferContent-1) + 1
    
    comparator = dstIntOffset + totalContent
    
    found = findVariables(instruction, function, comparator, "strcat")
    
    return found


def identifyStrncatWrittenVariables(instruction, function):
    
    found = []
    
    destAddr = register["rdi"]
    destBufferContent = memory[destAddr]["value"]
    destBufferSize = memory[destAddr]["bytes"]
    #srcAddr = register["rsi"]
    size = register["rdx"]
    
    #Bytes written in destination buffer minus \0 plus
    #Bytes in source buffer that are going to be concatenated plus
    #\0 to end the string
    totalContent = (destBufferContent - 1) + size + 1
    
    comparator = destBufferSize + totalContent
    
    found = findVariables(instruction, function, comparator, "strncat")
    
    return found


def detectVariableOverflow(instruction, function):
    
    result = []
    
    if instruction["args"]["fnname"] == "<gets@plt>":
        result = result + identifyGetsWrittenVariables(instruction, function)
        
    elif instruction["args"]["fnname"] == "<fgets@plt>":
        if detectFgetsVuln(instruction, function):
           result = result + identifyFgetsWrittenVariables(instruction, function)
           
    elif instruction["args"]["fnname"] == "<strcpy@plt>":
        if detectStrcpyVuln(instruction, function):
            result = result + identifyStrcpyWrittenVariables(instruction, function)
    
    elif instruction["args"]["fnname"] == "<strncpy@plt>":
        if detectStrncpyVuln(instruction, function):
            result = result + identifyStrncpyWrittenVariables(instruction, function)
        
    
    elif instruction["args"]["fnname"] == "<strcat@plt>":
        if detectStrcatVuln(instruction, function):
            result = result + identifyStrcatWrittenVariables(instruction, function)
    
    elif instruction["args"]["fnname"] == "<strncat@plt>":
        if detectStrncatVuln(instruction, function):
            result = result + identifyStrncatWrittenVariables(instruction, function)
        
    return result


def isRBPOverflow(bufferOffsetToRBP, bytesToInsert):
    return (bufferOffsetToRBP + bytesToInsert >= 0)

    
def detectRBPOverflow(instruction, function):

    rbpVulnerability = []

    destBuffer = str(register["rdi"])

    dangerousFunc = instruction["args"]["fnname"]

    if dangerousFunc == "<gets@plt>":
        
        vuln = addRBPOverflow(instruction, function, destBuffer, "gets")
        rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<fgets@plt>":

        if detectFgetsVuln(instruction, function):
            
            bytesToCopy = register["rsi"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), bytesToCopy):
                
                vuln = addRBPOverflow(instruction, function, destBuffer, "fgets")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strcpy@plt>":

        if detectStrcpyVuln(instruction, function):
            
            src = str(register["rsi"])
            srcSize = memory[src]["value"]

            if isRBPOverflow(int(destBuffer[3:], 16), srcSize):
                
                vuln = addRBPOverflow(instruction, function, destBuffer, "strcpy")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strcat@plt>":

        if detectStrcatVuln(instruction, function):

            destSize = memory[destBuffer]["value"]

            src = str(register["rsi"])
            srcSize = memory[src]["value"]

            resultingBytes = (destSize - 1) + srcSize + 1

            if isRBPOverflow(int(destBuffer[3:], 16), resultingBytes):
                vuln = addRBPOverflow(instruction, function, destBuffer, "strcat")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strncat@plt>":

        if detectStrncatVuln(instruction, function):

            nBytesToCopy = register["rdx"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), resultingBytes):
                vuln = addRBPOverflow(instruction, function, destBuffer, "strncat")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strncpy@plt>":

        if detectStrncpyVuln(instruction, function):

            nBytesToCopy = register["rdx"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), nBytesToCopy):
                vuln = addRBPOverflow(instruction, function, destBuffer, "strncpy")
                rbpVulnerability = rbpVulnerability + vuln

    return rbpVulnerability


def analyzeStrcpy():

		#RBP+0x8
		retAddress = 8

		dstAddress = str(register["rdi"])
		srcAddress = str(register["rsi"])
		srcSize = memory[srcAddress]["value"]

		return int(dstAddress[3:], 16) + srcSize >= retAddress


def analyzeStrncpy():

		#RBP+0x8
		retAddress = 8

		dstAddress = str(register["rdi"])
		sizeToCpy = register["rdx"]

		return int(dstAddress[3:], 16) + sizeToCpy >= retAddress


def analyzeStrcat():

		#RBP+0x8
		retAddress = 8

		dstAddress = str(register["rdi"])
		srcAddress = str(register["rsi"])

		dstSize = memory[dstAddress]["value"] - 1
		srcSize = memory[srcAddress]["value"]

		return int(dstAddress[3:], 16) + dstSize + srcSize + 1 >= retAddress


def analyzeStrncat():

		#RBP+0x8
		retAddress = 8

		dstAddress = str(register["rdi"])
		dstSize = memory[dstAddress]["value"] - 1
		sizeToCpy = str(register["rdx"])

		return int(dstAddress[3:], 16) + dstSize + sizeToCpy + 1 >= retAddress

def analyzeFgets(instruction, function):

		#RBP+0x8
		retAddress = 8

		bufferAddress = str(register["rdi"])
		srcSize = register["rsi"]

		return detectFgetsVuln(instruction, function) and int(bufferAddress[3:], 16) + srcSize >= retAddress


def detectRETOverflow(instruction, function):

	  RETOverflowVulnerability = []

	  dangerousFunc = instruction["args"]["fnname"]

	  if dangerousFunc == "<strcpy@plt>":

	    if detectStrcpyVuln(instruction, function) and analyzeStrcpy():
	    	RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcpy"))

	  elif dangerousFunc == "<strncpy@plt>" and detectStrncpyVuln(instruction, function) and analyzeStrncpy():
	    RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncpy"))

	  elif dangerousFunc == "<strcat@plt>" and detectStrcatVuln(instruction, function) and analyzeStrcat():
	    RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcat"))

	  elif dangerousFunc == "<strncat@plt>" and detectStrncatVuln(instruction, function) and analyzeStrncat():
	  	RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncat"))

	  elif dangerousFunc == "<fgets@plt>":

	    if analyzeFgets(instruction, function):
	      RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "fgets"))

	  elif dangerousFunc == "<gets@plt>":
	    RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "gets"))

	  return RETOverflowVulnerability


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
    
    if destination == "edx":
        destination = "rdx" #edx = rdx
        
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
        
    if destination == "edx":
        destination = "rdx" #edx = rdx
            
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
            
    if destination == "edx":
        destination = "rdx" #edx = rdx

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


def analyzeCall(instruction, function):

    global vulnerabilities

    print("call", instruction["args"]["fnname"])

    dangerousFunctions = ["<gets@plt>", "<strcpy@plt>", "<strcat@plt>", "<fgets@plt>", "<strncpy@plt>", "<strncat@plt>"]

    if instruction["args"]["fnname"] in dangerousFunctions:
        vulnerabilities = vulnerabilities + detectRETOverflow(instruction, function)
        
        vulnerabilities = vulnerabilities + detectRBPOverflow(instruction, function)

        vulnerabilities = vulnerabilities + detectVariableOverflow(instruction, function)


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
    print('python3 %s <program>.json' % progName)
    sys.exit()


if __name__ == '__main__':
    
    if len(sys.argv) < 2:
        usage(sys.argv[0])
    
    program = readJson(sys.argv[1])
    runFunction(program, "main")
    #writeJson(sys.argv[1], vulnerabilities)
    print("vulnerabilities", vulnerabilities)
