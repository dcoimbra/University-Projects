import sys

def readFile(file):
    print("Reading file %s" % file)
    f = open(file, 'r')
    file_contents = f.read()
    f.close()
    return file_contents

def usage(progName):
    print('Usage:')
    print(' python %s <program>.json' % progName)
    sys.exit()

if __name__ == '__main__':
    if len(sys.argv) < 2:
        usage(sys.argv[0])

    print(readFile(sys.argv[1]))