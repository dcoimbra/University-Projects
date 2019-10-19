#!/usr/bin/env python3

# Group 2
# 84708 - David Coimbra
# 84750 - Nuno Bombico
# 84842 - Jose Ferrao 

import sys
import json
import string

# Estrutura que representa os registos
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


# Structure that represents memory at a certain point in time
# Each variable has a context (a function) and the following fields 
# bytes: total allocated size
# value: bytes effectively written in memory for a buffer, or the actual value for a variable
# name: variable name
memory = {}  

vulnerabilities = []

# Determines if the value inserted in a buffer overflows its limit
# Implicitly updates the "value" field of the buffer with its new size
def isVulnerable(destBufferSize,valueToInsert):
    
    destAddr = register["rdi"]["value"]
    context = register["rdi"]["context"]
    
    dest = memory[context][destAddr]["value"]
    
    memory[context][destAddr]["value"] = dest + valueToInsert
    
    return valueToInsert > destBufferSize

# Determines if a call to fgets results in a potential vulnerability
# Implicitly updates structures as if fgets were executed
def detectFgetsVuln():

    bufferAddress = register["rdi"]["value"]
    context = register["rdi"]["context"]

    bytesToGet = register["rsi"]
    
    bufferSize = memory[context][bufferAddress]["bytes"]
        
    memory[context][bufferAddress]["value"] = bytesToGet
        
    return bytesToGet > bufferSize

# Determines if a call to strcpy results in a potential vulnerability
# Implicitly updates structures as if strcpy were executed
def detectStrcpyVuln():

    dest = register["rdi"]["value"]
    destContext = register["rdi"]["context"]
    
    src = register["rsi"]["value"]
    srcContext = register["rsi"]["context"]

    destSize = memory[destContext][dest]["bytes"]
    srcSize = memory[srcContext][src]["value"]
    
    memory[destContext][dest]["value"] = srcSize
        
    return destSize < srcSize

# Determines if a call to strncpy results in a potential vulnerability
# Implicitly updates structures as if strncpy were executed
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

# Determines if a call to strcat results in a potential vulnerability
# Implicitly updates structures as if strcat were executed
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

# Determines if a call to strncat results in a potential vulnerability
# Implicitly updates structures as if strcat were executed
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
    
# Outputs a variable overflow vulnerability to the overall vulnerabilities list
def addVarOver(instruction, function, offset, buffer, fnname):
        
    return [{
            "fnname": fnname,
            "vuln_function": function,
            "overflown_var": memory[function][offset]["name"],
            "vulnerability": "VAROVERFLOW",
            "overflow_var": memory[function][buffer]["name"],
            "address": instruction["address"]
            }]

# Outputs a RBP overflow vulnerability to the overall vulnerabilities list
def addRBPOverflow(instruction, function, buffer, context, fnname):
    
    return [{
            "vulnerability": "RBPOVERFLOW",
            "overflow_var": memory[context][buffer]["name"],
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": fnname
            }]

# Outputs a RET overflow vulnerability to the overall vulnerabilities list
def addRETOverflowOutput(function, instruction, dangerousFunc):

    return {
            "vulnerability": "RETOVERFLOW",
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": dangerousFunc,
            "overflow_var": memory[register["rdi"]["context"]][str(register["rdi"]["value"])]["name"]
    }

# Outputs a SCORRUPTION vulnerability to the overall vulnerabilities list
def addStackCorruption(instruction, function, dangerousFunc):
    
    return [{
            "overflow_address": "rbp+0x10",
            "vulnerability": "SCORRUPTION",
            "vuln_function": function,
            "address": instruction["address"],
            "fnname": dangerousFunc,
            "overflow_var": memory[register["rdi"]["context"]][str(register["rdi"]["value"])]["name"]      
            }]

# determines which local variables are affected by a buffer overflow    
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

# a call to "gets" affects all variables, so this function rounds them all up
def identifyGetsWrittenVariables(instruction, function):

    found = []
    
    buffer = str(register["rdi"]["value"])
    address = buffer[4:]
    intOffset = int(address,16)
    
    for off in memory[function]:
        offset = str(off)
        if "0x" in offset:
            # negative offsets, so ignore sign
            auxIntOffset = int(offset[4:],16) 

            if(auxIntOffset < intOffset):
                found = found + addVarOver(instruction, function, offset, buffer, "gets")
        
        #if 0x in offset -> take and see if offset is lower than this one
    
    return found

# for each call to a dangerous function, determine if there is a variable overflow
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

# determines if a buffer overflow reaches offset rbp+0x10, corrupting another stack frame
def isStackCorrupted(instruction, function, comparator, fnname):
    
    #The offset that begins to be considered is rbp+0x10, if it's main or f1 
    #which is 16, converted to int
    outOfBounds = 16
    
    aux = []
    
    if (comparator > outOfBounds):
        aux = aux + addStackCorruption(instruction, function, fnname) 
    
    return aux

# for each call to a dangerous function, determine if the stack is corrupted
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


# determines if the value inserted to a buffer overwrites the saved RBP in the same frame
def isRBPOverflow(bufferOffsetToRBP, bytesToInsert):
    return (bufferOffsetToRBP + bytesToInsert >= 0)


# for each call to a dangerous function, determine if there is a RBP overflow
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

# determines if the value inserted to a buffer overwrites the saved return address in the same frame
def overflowsRET(destBufferOffset, sizeToCpy):
    
    #RBP+0x8
    retAddress = 8

    return destBufferOffset + sizeToCpy > retAddress

# determines if a call to strcpy results in a potential overflow of the saved return address
def strcpyOverflowsRET():

    dstAddress = str(register["rdi"]["value"])
    srcAddress = str(register["rsi"]["value"])

    srcContext = register["rsi"]["context"]
    srcSize = memory[srcContext][srcAddress]["value"]

    return overflowsRET(int(dstAddress[3:], 16), srcSize)

# determines if a call to strncpy results in a potential overflow of the saved return address
def strncpyOverflowsRET():

    dstAddress = str(register["rdi"]["value"])
    sizeToCpy = register["rdx"]

    return overflowsRET(int(dstAddress[3:], 16), sizeToCpy)

# determines if a call to strcat results in a potential overflow of the saved return address
def strcatOverflowsRET():

    dstAddress = str(register["rdi"]["value"])
    dstContext = register["rdi"]["context"]

    srcAddress = str(register["rsi"]["value"])
    srcContext = register["rsi"]["context"]

    dstSize = memory[dstContext][dstAddress]["value"] - 1
    srcSize = memory[srcContext][srcAddress]["value"]

    sizeToCpy = dstSize + srcSize

    return overflowsRET(int(dstAddress[3:], 16), sizeToCpy)

# determines if a call to strncat results in a potential overflow of the saved return address
def strncatOverflowsRET():

    dstAddress = str(register["rdi"]["value"])
    dstContext = register["rdi"]["context"]

    dstSize = memory[dstContext][dstAddress]["value"] - 1
    sizeToCpy = str(register["rdx"])

    totalSizeToCpy = dstSize + sizeToCpy + 1

    return overflowsRET(int(dstAddress[3:], 16), totalSizeToCpy)


# determines if a call to fgets results in a potential overflow of the saved return address
def fgetsOverflowsRET():

    bufferAddress = str(register["rdi"]["value"])

    srcSize = register["rsi"]

    return overflowsRET(int(bufferAddress[3:], 16), srcSize)

# for each call to a dangerous function, determine if there is a RET oveflow
def detectRETOverflow(instruction, function):

    RETOverflowVulnerability = []

    dangerousFunc = instruction["args"]["fnname"]

    if dangerousFunc == "<strcpy@plt>" and detectStrcpyVuln() and strcpyOverflowsRET():
    		RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcpy"))

    elif dangerousFunc == "<strncpy@plt>" and detectStrncpyVuln() and strncpyOverflowsRET():
    		RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncpy"))

    elif dangerousFunc == "<strcat@plt>" and detectStrcatVuln() and strcatOverflowsRET():
    		RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strcat"))

    elif dangerousFunc == "<strncat@plt>" and detectStrncatVuln() and strncatOverflowsRET():
    		RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "strncat"))

    elif dangerousFunc == "<fgets@plt>" and detectFgetsVuln() and fgetsOverflowsRET():
    		RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "fgets"))

    elif dangerousFunc == "<gets@plt>":
        RETOverflowVulnerability.append(addRETOverflowOutput(function, instruction, "gets"))

    return RETOverflowVulnerability

# executes the assembly "add" instruction from a register to a register
def addRegisterToRegister(dest, val):
    register[dest] = register[dest] + register[val]

# executes the assembly "add" instruction from a register to a memory address
def addRegisterToPointer(dest, val, function):
    memory[function][val[11:-1]]['value'] = memory[function][val[11:-1]]['value'] + register[val]  


# executes the assembly "add" instruction from a memory address to a pointer
def addPointerToRegister(dest, val, function):
    if "rip" in val:  
        register[dest] = "stdin" #in the context of this project, when the memory address is a fixed value (not relative to rbp), it's always reading from standard input.
        return
    
    register[dest] = register[dest] + memory[function][val[11:-1]]['value']

# executes the assembly "add" instruction from an integer value to a memory address
def addNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = memory[function][dest[11:-1]]['value'] + int(val,16)

# executes the assembly "add" instruction from an integer value to a register
def addNumToRegister(dest, val):   
    register[dest] = register[dest] + int(val,16)

# executes the assembly "add" instruction from an integer value to a register
def subNumToRegister(dest, val):   
    register[dest] = register[dest] - int(val,16)

# executes the assembly "sub" instruction from a register to a register
def subRegisterToRegister(dest, val):
    register[dest] = register[dest] - register[val]

# executes the assembly "sub" instruction from a register to a memory address
def subRegisterToPointer(dest, val, function):
    memory[function][val[11:-1]]['value'] = memory[function][val[11:-1]]['value'] - register[val]  

# executes the assembly "sub" instruction from a memory address to a register
def subPointerToRegister(dest, val, function):
    if "rip" in val:
        register[dest] = "stdin" #in the context of this project, when the memory address is a fixed value (not relative to rbp), it's always reading from standard input.
        return
    
    register[dest] = register[dest] - memory[function][val[11:-1]]['value']

# executes the assembly "sub" instruction from an integer value to a memory address
def subNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = memory[function][dest[11:-1]]['value'] - int(val,16)
    
# executes the assembly "mov" instruction from a register to a register
def movRegisterToRegister(dest, val):
    register[dest] = register[val]

# executes the assembly "mov" instruction from a register to a memory address
def movRegisterToPointer(dest, val, function):

    if dest[11:-1] not in memory[function]: # if the variable doesn't exist, create it
        
        var = { dest[11:-1]: { "value": register[val]["value"], "context": register[val]["context"] } }

        memory[function].update(var)
    else:
        memory[function][dest[11:-1]]['value'] = register[val]  

# executes the assembly "mov" instruction from a memory address to a register
def movPointerToRegister(dest, val, function):
    if "rip" in val:
        register[dest] = "stdin"
        return

    # register variable value and function context
    var = { "context": memory[function][val[11:-1]]['context'], "value": memory[function][val[11:-1]]['value'] }
    
    register[dest] = var

# executes the assembly "mov" instruction from an integer value to a memory address
def movNumToPointer(dest, val, function):
    memory[function][dest[11:-1]]['value'] = int(val,16)
 
# executes the assembly "mov" instruction from an integer value to a register
def movNumToRegister(dest, val): 

    register[dest] = int(val,16)

# determines if a string corresponds to a hexadecimal integer
def isNumber(val):
    return all(c in string.hexdigits for c in val[2:])

# adds to memory the local variables initialized at the start of a function
def initializeLocalVariables(program, function):

    aux = {}

    for var in program[function]["variables"]:

        aux[var["address"]] = {"name": var["name"], "value": 0, "type": var["type"], "bytes": var["bytes"] }

    memory[function] = aux

# executes the "mov" instruction
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

# executes the "add" instruction
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

# executes the "sub" instruction
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


# executes the "lea" instruction
def lea(destination, value, function):

    register[destination] = { "context": function, "value": value[1:-1] }


# executes the "push" instruction
def push(value, function):

    subNumToRegister("rsp", "0x8") #allocate space in the stack

    memory[function][register["rsp"]] = { "value": register[value] } #push to stack

# executes the "leave" instruction
def leave(function):

    movRegisterToRegister("rsp", "rbp") #restore stack pointer
    
    #pop the stack
    register["rbp"] = memory[function][register["rsp"]]["value"]
    memory[function].pop(register["rsp"], None)


# executes a certain assembly instruction that is not "nop", "call" or "ret"
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

# determines if a call is dangerous
def analyzeCall(program, instruction, function):

    global vulnerabilities

    dangerousFunctions = ["<gets@plt>", "<strcpy@plt>", "<strcat@plt>", "<fgets@plt>", "<strncpy@plt>", "<strncat@plt>"]

    #if a call is dangerous, analyze for vulnerabilities, then continue execution
    if instruction["args"]["fnname"] in dangerousFunctions:        

        destAddr = register["rdi"]["value"]
        context = register["rdi"]["context"]

        dest = memory[context][destAddr]["value"]

        vulnerabilities = vulnerabilities + detectRETOverflow(instruction, function)

        memory[context][destAddr]["value"] = dest
        
        vulnerabilities = vulnerabilities + detectRBPOverflow(instruction, function)

        memory[context][destAddr]["value"] = dest

        vulnerabilities = vulnerabilities + detectVariableOverflow(instruction, function)
        
        memory[context][destAddr]["value"] = dest
        
        vulnerabilities = vulnerabilities + detectStackCorruption(instruction, function)

    # else, call the function
    else:
        runFunction(program, (instruction["args"]["fnname"])[1:-1])
    

# runs all of the instructions in a function
def runFunction(program, function):
    
    initializeLocalVariables(program, function)
    
    for instr in program[function]["instructions"]:

        if instr['op'] == "nop": # nop has no effect
            continue
        
        elif instr['op'] == "call":
            analyzeCall(program, instr, function)

        elif instr['op'] == "ret": # exit function, return to calling context
            return

        runInstruction(instr, function)

# writes analyzed vulnerabilities to a file in JSON format
def writeJson(progName, vulnerabilities):
    f = open("%s.output.json" % progName, 'w+')
    f.write(json.dumps(vulnerabilities, indent=4, separators=(',', ': ')))
    f.close()

# reads a file in JSON format and converts it into a dictionary for easy traversal
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
    writeJson((sys.argv[1])[:-5], vulnerabilities)
