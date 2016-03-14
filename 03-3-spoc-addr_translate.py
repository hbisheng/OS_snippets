def get_page_table_info(str):
    va = int(str[3:13], 16)
    pa = int(str[-10:], 16)
    pde_index = va >> 22
    if(pde_index >= 0x300 & pde_index <= 0x363):
        pde_content = (pde_index-0x300+1) << 12 + 3
    else:
        pde_content = 0
        
    pte_index = (va >> 12) & 0x3FF
    pte_content = (pa >> 12) << 12 + 3
    
    print '%s, pde_idx 0x%08x, pde_ctx 0x%08x, pte_idx 0x%08x, pte_ctx 0x%08x' % (str, pde_index, pde_content, pte_index, pte_content)
    
if  __name__ == "__main__":
    
    samples = [
        'va 0xc2265b1f, pa 0x0d8f1b1f',
        'va 0xcc386bbc, pa 0x0414cbbc',
        'va 0xc7ed4d57, pa 0x07311d57',
        'va 0xca6cecc0, pa 0x0c9e9cc0',
        'va 0xc18072e8, pa 0x007412e8',
        'va 0xcd5f4b3a, pa 0x06ec9b3a',
        'va 0xcc324c99, pa 0x0008ac99',
        'va 0xc7204e52, pa 0x0b8b6e52',
        'va 0xc3a90293, pa 0x0f1fd293',
        'va 0xce6c3f32, pa 0x007d4f32'];
    
    for i in range(0, len(samples)):
        get_page_table_info(samples[i])