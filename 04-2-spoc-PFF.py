input = ['c', 'c', 'd', 'b', 'c', 'e', 'c', 'e', 'a', 'd']
T = 2

# initial state
pages_lastHit = {}
pages_lastHit['a'] = 0
pages_lastHit['d'] = 0
pages_lastHit['e'] = 0
lastPF = 0

print "Initial State:", pages_lastHit
for i in range(0, len(input)):
    visitPage = input[i]
    print "Visit page", visitPage,
    if pages_lastHit.has_key(visitPage):
        print "Hit " + visitPage,
        pages_lastHit[visitPage] = i+1
    else:
        print "(Interrupt)page Fault",
        if (i+1) - lastPF > T:
            # delete all unused pages since last Page Fault
            existingPages = pages_lastHit.keys()
            for page in existingPages:
                if(pages_lastHit[page] <= lastPF):
                    pages_lastHit.pop(page, None)
            pages_lastHit[visitPage] = i+1
        else:
            # just add the missing page
            pages_lastHit[visitPage] = i+1
          = i+1
    print ""
    print pages_lastHit