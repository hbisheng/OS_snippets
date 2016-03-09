#input: internal.txt/data
#output: answer.txt

import sys
def getdata(page_num, offset):
    global data
    line = data[page_num].split(' ')
    return line[offset + 2]

if  __name__ == "__main__":
    #load data from file
    f = open('internal.txt','r')
    sys.stdout = open('answers.txt','w')
    data = f.readlines()
    f.close()

    page_size = 32
    PDBR = 544 / page_size #the page that the PDBR is in
    questions = ['6c74','6b22','03df','69dc','317a','4546','2c03','7fd7','390e','748b']

    for q in questions:
        print 'Virtual Address: ', q
        input = int(q,16)    
        pde_index = (input >> 10) & (0x1f) #get first 5 bits: pde index
        pde_content = int( getdata(PDBR , pde_index), 16)
        print '  --> pde index:' + hex(pde_index),
        if(not(pde_content & 0x80)):
            print ' pde contents:(valid 0, PT '+ hex(pde_content&0x7f) + ')'
            print '      --> Fault (page directory entry not valid)\n'
            continue
        print ' pde contents:(valid 1, PT ' + hex(pde_content&0x7f) + ')'

        pde_content &= 0x7f 
        pte_index = (input >> 5) & (0x1f)#get middle 5 bits: get pte index
        pte_content = int( getdata(pde_content, pte_index), 16)
        print '    --> pte index:'+hex(pte_index),
        if(not(pte_content & 0x80)):
            print 'pte contents:(valid 0, PFN '+ hex(pte_content&0x7f) +')'
            print '      --> Fault (page table entry not valid)\n'
            continue
        print ' pte contents:(valid 1, PFN '+ hex(pte_content&0x7f) +')'

        pte_content &= 0x7f
        offset = (input) & (0x1f) #get last 5 bits: get offset
        ans = int( getdata(pte_content, offset), 16)
        print '      --> Translates to Physical Address ' + hex((pte_content << 5) + offset) + ' --> Value:',ans,'\n'    
    sys.stdout.close()