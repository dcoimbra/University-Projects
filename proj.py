#!/usr/bin/env python3

# Group 2
# 84708 - David Coimbra
# 84750 - Nuno Bombico
# 84842 - Jose Ferrao 

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
    
    destAddr = register["rdi"]["value"]
    context = register["rdi"]["context"]
    
    dest = memory[context][destAddr]["value"]
    
    memory[context][destAddr]["value"] = dest + valueToInsert
    
    return valueToInsert > destBufferSize

def detectFgetsVuln():

    bufferAddress = register["rdi"]["value"]
    context = register["rdi"]["context"]

    bytesToGet = register["rsi"]
    
    bufferSize = memory[context][bufferAddress]["bytes"]
        
    memory[context][bufferAddress]["value"] = bytesToGet
        
    return bytesToGet > bufferSize

def detectStrcpyVuln():

    dest = register["rdi"]["value"]
    destContext = register["rdi"]["context"]
    
    src = register["rsi"]["value"]
    srcContext = register["rsi"]["context"]

    destSize = memory[destContext][dest]["bytes"]
    srcSize = memory[srcContext][src]["value"]
    
    memory[destContext][dest]["value"] = srcSize
        
    return destSize < srcSize


def detectStrncpyVuln():

    destAddr = register["rdi"]["value"]
    destContext = register["rdi"]["context"]

    srcAddr = register["rsi"]["value"]
    srcContext = register["rsi"]["context"]

    sizeToCpy = register["rdx"]

    srcWritten = memory[srcContext][srcAddr]["value"]
    
    destSize = memory[destContext][destAddr]["bytes"]

    memory[destContext][destAddr]["value"] = sizeToCpy


    #If it's lower, it doesn't have \0, so, in the worst case, we'll
    #assume the \0 is further away. The size written to the buffer being 1000 is symbolic.
    if sizeToCpy < srcWritten:
            memory[destContext][destAddr]["value"] = 1000 
                                             
    else:
        memory[destContext][destAddr]["value"] = sizeToCpy
    
    
    return sizeToCpy > destSize

    
def detectStrcatVuln():
        
    destAddr = register["rdi"]["value"]
    destContext = register["rdi"]["context"]

    srcAddr = register["rsi"]["value"]
    srcContext = register["rsi"]["context"]
    
    dest = memory[destContext][destAddr]["value"]
    src = memory[srcContext][srcAddr]["value"]
    
    destBufferSize = memory[destContext][destAddr]["bytes"]
 
    #Bytes written in destination buffer minus \0 plus
    #Bytes written in source buffer. The \0 is inside the src buffer
    resultingBytes = (dest-1) + src
    
    return isVulnerable(destBufferSize,resultingBytes)


def detectStrncatVuln():
    
    destAddr = register["rdi"]["value"]
    context = register["rdi"]["context"]

    size = register["rdx"]
    
    dest = memory[context][destAddr]["value"]
    destBufferSize = memory[context][destAddr]["bytes"]
    
    #Bytes written in destination buffer minus \0 plus
    #Bytes in source buffer that are going to be concatenated plus
    #\0 to end the string
    
    resultingBytes = (dest - 1) + size + 1
    
    return isVulnerable(destBufferSize,resultingBytes)
    

def addVarOver(instruction, function, offset, buffer, fnname):
        
    return [{
            "fnname": fnname,
            "vuln_function": function,
            "overflown_var": memory[function][offset]["name"],
            "vulnerability": "VAROVERFLOW",
            "overflow_var": memory[function][buffer]["name"],
            "address": instruction["address"]
            }]


def addRBPOverflow(instruction, function, buffer, context, fnname):
    
    return [{
            "vulnerability": "RBPOVERFLOW",
            "overflow_var": memory[context][buffer]["name"],
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
            "overflow_var": memory[register["rdi"]["context"]][str(register["rdi"]["value"])]["name"]
    }


def addINVALIDACCSOutput(function, instruction, dangerousFunc):

    return {
            "vulnerability": "INVALIDACCS",
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": dangerousFunc,
            "overflow_var": memory[register["rdi"]["context"]][str(register["rdi"]["value"])]["name"]
    }
 

def addStackCorruption(instruction, function, dangerousFunc):
    
    return [{
            "overflow_address": "rbp+0x10",
            "vulnerability": "SCORRUPTION",
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": dangerousFunc,
            "overflow_var": memory[register["rdi"]["context"]][str(register["rdi"]["value"])]["name"]      
            }]
    
def findVariables(instruction, function, comparator, fnname):

    aux = []
    
    buffer = str(register["rdi"]["value"])
    
    for off in memory[function]:
        offset = str(off)
        if "0x" in offset:
            if offset[3:] != buffer[3:]: #to avoid the same offset (same buffer)
                if (int(offset[3:],16) < comparator) and (int(offset[3:],16) > int(buffer[3:],16)): #if it's smaller than the buffer, then it's above it and safe 
                    aux = aux + addVarOver(instruction, function, offset, buffer, fnname)

    return aux


def identifyGetsWrittenVariables(instruction, function):

    found = []
    
    buffer = str(register["rdi"]["value"])
    address = buffer[4:]
    intOffset = int(address,16)
    
    for off in memory[function]:
        offset = str(off)
        if "0x" in offset:
            #offsets sao negativos, por isso pode-se ignorar sinal
            auxIntOffset = int(offset[4:],16) 

            if(auxIntOffset < intOffset):
                found = found + addVarOver(instruction, function, offset, buffer, "gets")
        
        #if 0x in offset -> take and see if offset is lower than this one
    
    return found

def detectVariableOverflow(instruction, function):
    
    result = []
    
    dstBuffer = str(register["rdi"]["value"])
    dstAddress = dstBuffer[3:]
    intOffset = int(dstAddress,16)
    
    if instruction["args"]["fnname"] == "<gets@plt>":
        result = result + identifyGetsWrittenVariables(instruction, function)
        
    elif instruction["args"]["fnname"] == "<fgets@plt>":
        if detectFgetsVuln():
            
            size = register["rsi"]
            
            comparator = intOffset + size
            result = result + findVariables(instruction, function, comparator, "fgets")
                       
    elif instruction["args"]["fnname"] == "<strcpy@plt>":
        if detectStrcpyVuln():
            
            srcBuffer = str(register["rsi"]["value"])
            srcContext = register["rsi"]["context"]

            srcFill = memory[srcContext][srcBuffer]["value"]
            
            comparator = intOffset + srcFill
            
            result = result + findVariables(instruction, function, comparator, "strcpy")
    
    elif instruction["args"]["fnname"] == "<strncpy@plt>":
        if detectStrncpyVuln():
                   
            sizeToCpy = register["rdx"]
                 
            comparator = intOffset + sizeToCpy
            
            result = result + findVariables(instruction, function, comparator, "strncpy")
    
    elif instruction["args"]["fnname"] == "<strcat@plt>":
        if detectStrcatVuln():
            
            dstBufferContent = memory[dstBuffer]["value"]
            
            srcBuffer = str(register["rsi"])
            srcBufferContent = memory[srcBuffer]["value"]
            
            totalContent = (dstBufferContent-1) + srcBufferContent
            
            comparator = intOffset + totalContent
            
            result = result + findVariables(instruction, function, comparator, "strcat")
                
    elif instruction["args"]["fnname"] == "<strncat@plt>":
        if detectStrncatVuln():
            
            destBufferContent = memory[dstBuffer]["value"]
            destBufferSize = memory[dstBuffer]["bytes"]

            size = register["rdx"]

            totalContent = (destBufferContent - 1) + size + 1
            
            comparator = destBufferSize + totalContent
            
            result = result + findVariables(instruction, function, comparator, "strncat")
        
    return result


def isStackCorrupted(instruction, function, comparator, fnname):
    
    #The offset that begins to be considered is rbp+0x10, if it's main or f1 
    #which is 16, converted to int
    outOfBounds = 16
    
    aux = []
    
    if (comparator > outOfBounds):
        aux = aux + addStackCorruption(instruction, function, fnname) 
    
    return aux


def detectStackCorruption(instruction, function):
    
    result = []
    
    
    dstBuffer = str(register["rdi"]["value"])
    dstContext = register["rdi"]["context"]

    dstAddress = dstBuffer[3:]
    intOffset = int(dstAddress,16)
    

    if instruction["args"]["fnname"] == "<gets@plt>":
        result = result + addStackCorruption(instruction, function, "gets")
    
    
    elif instruction["args"]["fnname"] == "<fgets@plt>":
        if detectFgetsVuln():
            
            size = register["rsi"]
            comparator = intOffset + size
            
            result = result + isStackCorrupted(instruction, function, comparator, "fgets")
            
            
    elif instruction["args"]["fnname"] == "<strcpy@plt>":
        if detectStrcpyVuln():
            
            srcBuffer = str(register["rsi"]["value"])
            srcContext = register["rsi"]["context"]

            srcFill = memory[srcContext][srcBuffer]["value"]
            
            comparator = intOffset + srcFill
            
            result = result + isStackCorrupted(instruction, function, comparator, "strcpy")       
            
    
    elif instruction["args"]["fnname"] == "<strncpy@plt>":
        if detectStrncpyVuln():
            
            sizeToCpy = register["rdx"]
                 
            comparator = intOffset + sizeToCpy
            
            result = result + isStackCorrupted(instruction, function, comparator, "strncpy")
            
    
    elif instruction["args"]["fnname"] == "<strcat@plt>":
        if detectStrcatVuln():
            
            dstBufferContent = memory[dstContext][dstBuffer]["value"]
            
            srcBuffer = str(register["rsi"]["value"])
            srcContext = register["rsi"]["context"]

            srcBufferContent = memory[srcContext][srcBuffer]["value"]
            
            totalContent = (dstBufferContent-1) + srcBufferContent
            
            comparator = intOffset + totalContent
            
            result = result + isStackCorrupted(instruction, function, comparator, "strcat")
            
    
    elif instruction["args"]["fnname"] == "<strncat@plt>":
        if detectStrncatVuln():
            
            destBufferContent = memory[dstContext][dstBuffer]["value"]
            destBufferSize = memory[dstContext][dstBuffer]["bytes"]
            
            size = register["rdx"]
            

            totalContent = (destBufferContent - 1) + size + 1
            
            comparator = destBufferSize + totalContent
            
            result = result + isStackCorrupted(instruction, function, comparator, "strncat")

    return result



def isRBPOverflow(bufferOffsetToRBP, bytesToInsert):
    return (bufferOffsetToRBP + bytesToInsert >= 0)

    
def detectRBPOverflow(instruction, function):

    rbpVulnerability = []

    destBuffer = str(register["rdi"]["value"])
    destContext = register["rdi"]["context"]

    dangerousFunc = instruction["args"]["fnname"]

    if dangerousFunc == "<gets@plt>":
        
        vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "gets")
        rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<fgets@plt>":

        if detectFgetsVuln():
            
            bytesToCopy = register["rsi"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), bytesToCopy):
                
                vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "fgets")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strcpy@plt>":

        if detectStrcpyVuln():
            
            src = str(register["rsi"]["value"])
            srcContext = register["rsi"]["context"]
            srcSize = memory[srcContext][src]["value"]

            if isRBPOverflow(int(destBuffer[3:], 16), srcSize):
                
                vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "strcpy")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strcat@plt>":

        if detectStrcatVuln():

            destSize = memory[function][destBuffer]["value"]

            src = str(register["rsi"]["value"])
            srcContext = register["rsi"]["context"]

            srcSize = memory[srcContext][src]["value"]

            resultingBytes = (destSize - 1) + srcSize + 1

            if isRBPOverflow(int(destBuffer[3:], 16), resultingBytes):
                vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "strcat")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strncat@plt>":

        if detectStrncatVuln():

            nBytesToCopy = register["rdx"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), resultingBytes):
                vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "strncat")
                rbpVulnerability = rbpVulnerability + vuln

    elif dangerousFunc == "<strncpy@plt>":

        if detectStrncpyVuln():

            nBytesToCopy = register["rdx"]
            
            if isRBPOverflow(int(destBuffer[3:], 16), nBytesToCopy):
                vuln = addRBPOverflow(instruction, function, destBuffer, destContext, "strncpy")
                rbpVulnerability = rbpVulnerability + vuln

    return rbpVulnerability


def analyzeStrcpy(function):
    
    #RBP+0x8
    retAddress = 8

    dstAddress = str(register["rdi"]["value"])

    srcAddress = str(register["rsi"]["value"])
    srcContext = register["rsi"]["context"]
    srcSize = memory[srcContext][srcAddress]["value"]

    return int(dstAddress[3:], 16) + srcSize >= retAddress


def analyzeStrncpy():

    #RBP+0x8
    retAddress = 8

    dstAddress = str(register["rdi"]["value"])
    sizeToCpy = register["rdx"]

    return int(dstAddress[3:], 16) + sizeToCpy >= retAddress


def analyzeStrcat(function):

    #RBP+0x8
    retAddress = 8

    dstAddress = str(register["rdi"]["value"])
    dstContext = register["rdi"]["context"]

    srcAddress = str(register["rsi"]["value"])
    srcContext = register["rsi"]["context"]

    dstSize = memory[dstContext][dstAddress]["value"] - 1
    srcSize = memory[srcContext][srcAddress]["value"]

    return int(dstAddress[3:], 16) + dstSize + srcSize >= retAddress


def analyzeStrncat(function):

    #RBP+0x8
    retAddress = 8

    dstAddress = str(register["rdi"]["value"])
    dstContext = register["rdi"]["context"]

    dstSize = memory[dstContext][dstAddress]["value"] - 1
    sizeToCpy = str(register["rdx"])

    return int(dstAddress[3:], 16) + dstSize + sizeToCpy + 1 >= retAddress

def analyzeFgets(instruction, function):

    #RBP+0x8
    retAddress = 8

    bufferAddress = str(register["rdi"]["value"])

    srcSize = register["rsi"]

    return detectFgetsVuln() and int(bufferAddress[3:], 16) + srcSize >= retAddress


def detectRETOverflow(instruction, function):

    RETOverflowVulnerability = []

    dangerousFunc = instruction["args"]["fnname"]

    if dangerousFunc == "<strcpy@plt>":

        if detectStrcpyVuln() and analyzeStrcpy(function):
            RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcpy"))

    elif dangerousFunc == "<strncpy@plt>" and detectStrncpyVuln() and analyzeStrncpy():
        RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncpy"))

    elif dangerousFunc == "<strcat@plt>" and detectStrcatVuln() and analyzeStrcat(function):
        RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcat"))

    elif dangerousFunc == "<strncat@plt>" and detectStrncatVuln() and analyzeStrncat(function):
        RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncat"))

    elif dangerousFunc == "<fgets@plt>":

        if analyzeFgets(instruction, function):
            RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "fgets"))

    elif dangerousFunc == "<gets@plt>":
        RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "gets"))

    return RETOverflowVulnerability


#def detectINVALIDACCS(instruction, function):



def addRegisterToRegister(dest, val):
    register[dest] = register[dest] + register[val]


def addRegisterToPointer(dest, val, function):
    memory[function][val[11:-1]]['value'] = memory[function][val[11:-1]]['value'] + register[val]  


def addPointerToRegister(dest, val, function):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = register[dest] + memory[function][val[11:-1]]['value']


def addNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = memory[function][dest[11:-1]]['value'] + int(val,16)


def addNumToRegister(dest, val):   
    register[dest] = register[dest] + int(val,16)


def subNumToRegister(dest, val):   
    register[dest] = register[dest] - int(val,16)


def subRegisterToRegister(dest, val):
    register[dest] = register[dest] - register[val]


def subRegisterToPointer(dest, val, function):
    memory[function][val[11:-1]]['value'] = memory[function][val[11:-1]]['value'] - register[val]  


def subPointerToRegister(dest, val, function):
    if "rip" in val:
        register[dest] = "stdin"
        return
    
    register[dest] = register[dest] - memory[function][val[11:-1]]['value']


def subNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = memory[function][dest[11:-1]]['value'] - int(val,16)
    

def movRegisterToRegister(dest, val):
    register[dest] = register[val]


def movRegisterToPointer(dest, val, function):

    if dest[11:-1] not in memory[function]:
        
        var = { dest[11:-1]: { "value": register[val]["value"], "context": register[val]["context"] } }

        memory[function].update(var)
    else:
        memory[function][dest[11:-1]]['value'] = register[val]  


def movPointerToRegister(dest, val, function):
    if "rip" in val:
        register[dest] = "stdin"
        return

    var = { "context": memory[function][val[11:-1]]['context'], "value": memory[function][val[11:-1]]['value'] }
    
    register[dest] = var


def movNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = int(val,16)
 
   
def movNumToRegister(dest, val): 

    register[dest] = int(val,16)


def isNumber(val):
    return all(c in string.hexdigits for c in val[2:])


def initializeLocalVariables(program, function):

    aux = {}

    for var in program[function]["variables"]:

        aux[var["address"]] = {"name": var["name"], "value": 0, "type": var["type"], "bytes": var["bytes"] }

    memory[function] = aux


def mov(destination, value, function):
    
    if destination == "esi":
        destination = "rsi" #esi = rsi

    if destination == "eax":
        destination = "rax" #eax = rax
    
    if destination == "edx":
        destination = "rdx" #edx = rdx
        
    if (destination in register.keys()) and (value in register.keys()):
        movRegisterToRegister(destination, value)
                
    elif (destination in register.keys()) and (isNumber(value)):
        movNumToRegister(destination, value)
    
    elif (destination in register.keys()) and ("PTR" in value):
        movPointerToRegister(destination,value,function)
    
    elif ("PTR" in destination) and (value in register.keys()):
        movRegisterToPointer(destination,value,function)
        
    elif ("PTR" in destination) and (isNumber(value)):
        movNumToPointer(destination,value, function)


def add(destination, value, function):
    
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
        addPointerToRegister(destination,value,function)
    
    elif ("PTR" in destination) and (value in register.keys()):
        addRegisterToPointer(destination,value,function)
        
    elif ("PTR" in destination) and (isNumber(value)):
        addNumToPointer(destination,value,function)


def sub(destination, value, function):
    
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
        subPointerToRegister(destination,value,function)
    
    elif ("PTR" in destination) and (value in register.keys()):
        subRegisterToPointer(destination,value,function)
        
    elif ("PTR" in destination) and (isNumber(value)):
        subNumToPointer(destination,value,function)


def lea(destination, value, function):

    register[destination] = { "context": function, "value": value[1:-1] }


def push(value, function):

    subNumToRegister("rsp", "0x8")

    memory[function][register["rsp"]] = { "value": register[value] }


def leave(function):

    movRegisterToRegister("rsp", "rbp")
    
    register["rbp"] = memory[function][register["rsp"]]["value"]
    memory[function].pop(register["rsp"], None)


def runInstruction(instr, function):
        
    if instr['op'] == "mov":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        mov(destination, value, function)
    
    elif instr['op'] == "add":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        add(destination, value, function)

    elif instr['op'] == "sub":
        
        destination = instr['args']['dest']
        value = instr['args']['value']
        
        sub(destination, value, function)

    elif instr['op'] == "lea":
        
        destination = instr['args']['dest']
        value = instr['args']['value']

        lea(destination, value, function)

    elif instr['op'] == "push":
        
        value = instr["args"]["value"]
        push(value, function)

    elif instr["op"] == "leave":
        leave(function)


def analyzeCall(program, instruction, function):

    global vulnerabilities

    dangerousFunctions = ["<gets@plt>", "<strcpy@plt>", "<strcat@plt>", "<fgets@plt>", "<strncpy@plt>", "<strncat@plt>"]

    if instruction["args"]["fnname"] in dangerousFunctions:        

        destAddr = register["rdi"]["value"]
        context = register["rdi"]["context"]

        dest = memory[context][destAddr]["value"]

        vulnerabilities = vulnerabilities + detectRETOverflow(instruction, function)

        memory[context][destAddr]["value"] = dest
        
        vulnerabilities = vulnerabilities + detectRBPOverflow(instruction, function)

        memory[context][destAddr]["value"] = dest

        vulnerabilities = vulnerabilities + detectVariableOverflow(instruction, function)
        
        memory[context][destAddr]["value"]
        
        vulnerabilities = vulnerabilities + detectStackCorruption(instruction, function)

    else:
        runFunction(program, (instruction["args"]["fnname"])[1:-1])
    


def runFunction(program, function):
    
    initializeLocalVariables(program, function)
    
    for instr in program[function]["instructions"]:

        if instr['op'] == "nop":
            continue
        
        elif instr['op'] == "call":
            analyzeCall(program, instr, function)

        elif instr['op'] == "ret":
            return

        runInstruction(instr, function)


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
    #writeJson((sys.argv[1])[:-5], vulnerabilities)

    print(vulnerabilities)
