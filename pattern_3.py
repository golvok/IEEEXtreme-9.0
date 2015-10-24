import sys

def longest_substr(lst):
    longest = len
    end = len(lst)
    for i in range(1,end):
        test_s = 0
        test_e = i
        # print("--" + lst[0:1])
        while (True):
            # print("hi");
            # print(test_e)
            # print(test_s)
            if test_e == end:
                return i
                break
            if test_e > end:
                # print(lst[test_s:test_e])
                # print(lst[0: test_e - end]) 
                if (lst[0:i - test_e - end] == lst[test_s: test_e - end]):
                    # print ("this")
                    return i
                else:
                    return end
            if (lst[0:i] != lst[test_s:test_e]):
                break
            test_s = test_e
            test_e += i
            
    return longest

a = sys.stdin.readline()
for line in sys.stdin:
    line = line.replace('\n', '')
    # print (line)
    print(line[0:longest_substr(line)])
