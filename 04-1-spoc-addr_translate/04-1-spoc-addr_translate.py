#input: mem.txt disk.txt

def get_mem_data(page_num, offset):
    global mem_data
    line = mem_data[page_num].split(' ')
    return int(line[offset + 2],16)
    
def get_disk_data(page_num, offset):
    global disk_data
    line = disk_data[page_num].split(' ')
    return int(line[offset + 2],16)
        
def translate_addr(va):
    global PDBR

    print 'Virtual Address: ', hex(va)
    pde_index = (va >> 10) & (0x1f)
    pde_content = get_mem_data(PDBR, pde_index)
    print '  --> pde index:' + hex(pde_index),
    
    if( not(pde_content & 0x80) ):
        print ' pde contents:(valid 0, pfn '+ hex(pde_content&0x7f) + ')'
        print '      --> Fault (PDE entry not valid)\n'
    
    print ' pde contents:(valid 1, pfn ' + hex(pde_content&0x7f) + ')'
    
    pde_content &= 0x7f 
    pte_index = (va >> 5) & (0x1f) # get middle 5 bits: get pte index
    
    pte_content = get_mem_data(pde_content, pte_index)
    print '    --> pte index:'+hex(pte_index),
    
    offset = (va) & (0x1f) # get last 5 bits: get offset
    if(not(pte_content & 0x80)):
        pte_content &= 0x7f
        print ' pte contents:(valid 0, pfn '+ hex(pte_content&0x7f) +')'
        
        if(pte_content == 0x7f):
            print '      --> Fault (Data is not in disk)\n'
        else:
            val = get_disk_data(pte_content, offset)
            print '      --> To Disk Vector Address ' + hex( (pte_content << 5) + offset) + ' --> value:', hex(val), '\n'
    else:
        pte_content &= 0x7f
        print ' pte contents:(valid 1, pfn '+ hex(pte_content&0x7f) +')'
        val = get_mem_data(pte_content, offset)
        print '      --> Physical Address ' + hex((pte_content << 5) + offset) + ' --> Value:', hex(val),'\n'    
    
    
if  __name__ == "__main__":
    #sys.stdout = open('answers.txt','w')
    
    #load data from file
    f_mem = open('mem.txt','r')
    f_disk = open('disk.txt', 'r')
    mem_data = f_mem.readlines()
    disk_data = f_disk.readlines()
    f_mem.close()
    f_disk.close()
    
    page_size = 32
    PDBR = 0xd80 / page_size #the page that the PDBR is in
    
    translate_addr(0x0330)
    translate_addr(0x6653)
    translate_addr(0x1c13)
    translate_addr(0x6890)
    translate_addr(0x0af6)
    translate_addr(0x1e6f)