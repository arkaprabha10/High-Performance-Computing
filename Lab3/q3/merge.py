import config
import os

def extactRawData(filename):
    data = []
    with open(filename, 'r') as f:
        for line in f.readlines():
            n, p, rid, alg, e2e = line.strip().split(',')
            data.append((n, p, alg, e2e))

    # remove incomplete runs
    while len(data) > 0 and (int(data[-1][0]) != config.prob[-1] or int(data[-1][1]) != config.procs[-1]):
        data.pop()

    return data

def checkConsistency(data):
    procsLen = len(config.procs)
    probLen = len(config.prob)
    singleRunLen = procsLen * probLen

    probIndx = 0
    procsIndx = 0
    for i in range(len(data)):
        if len(data[i]) != 4:
            return False
        if int(data[i][0]) != config.prob[probIndx]:
            return False
        if int(data[i][1]) != config.procs[procsIndx]:
            return False

        probIndx += 1

        if probIndx == probLen:
            probIndx = 0
            procsIndx += 1

        if procsIndx == procsLen:
            procsIndx = 0
    
    if procsIndx != 0 or probIndx != 0:
        return False
    return True



if __name__ == "__main__":
    if 'fragments' not in os.listdir(os.curdir):
        print("Folder 'fragments' is missing: exiting")
        exit()
    if 'data-merged.txt' in os.listdir(os.curdir):
        print('Warning: data-merged.txt will be overwritten')
        proceed = None
        while proceed != 'n' and proceed != 'y':
            proceed = raw_input('Proceed? (y/n):')
        
        if proceed == 'n':
            exit()
    else:
        print('File data-merged.txt will be created')

    procsLen = len(config.procs)
    probLen = len(config.prob)
    toBeWritten = []
    runId = 0
    
    for frag in os.listdir(os.curdir + '/fragments'):
        try:
            data = extactRawData(os.curdir+'/fragments/'+frag)
            if checkConsistency(data):
                print "File", frag, ":", len(data) / (procsLen * probLen), "runs"
                for point in data:
                    n, p, alg, e2e = point
                    toBeWritten.append([n,p,str(runId),alg,e2e])
                    if len(toBeWritten) % (procsLen * probLen) == 0:
                        runId += 1
            else:
                print "File", frag, "is not consistent: ignoring"
        except:
            print "Entity", frag, "is not file/supported: ignoring"


    with open('data-merged.txt', 'w') as f:
        for point in toBeWritten:
            f.write(','.join(point)+'\n')
    
    print "Total Runs Collected:", runId