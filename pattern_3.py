import sys

def longest_substr(lst):
    longest = -1
    end = len(lst)
    for i in range(end):
        test_s = 0
        test_e = i
        print("--" + lst[0:1])
        while (True):
            if test_e == end:
                longest = i
                break
            if test_e > end:
                if (lst[test_s:test_e] == lst[0:test_e - test_s]):
                    longest = i
                break
            if (lst[0:i] != lst[test_s:test_e]):
                break
            test_s = test_e
            test_e += i
            
    return longest

a = sys.stdin.readline()
for line in sys.stdin:
    line = line.replace('\n', '')
    print (line)
    print(">>" + line[0:longest_substr(line)])
