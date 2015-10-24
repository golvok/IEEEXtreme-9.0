
import sys
from decimal import *
import math

def catalan(n):
	result = Decimal(1)
	for k in range(2,n+1):
		result = result * Decimal(n + k)/Decimal(k)
	return result


for line in sys.stdin:
	getcontext().prec=350
	# print(catalan(549)); # 329 digits
	target_num = Decimal(line);
	binary_searching = True;
	min_num = 0;
	max_num = 550;
	while 1:
		if (binary_searching):
			num = int((max_num - min_num)/2 + min_num)
		else:
			max_num = max_num*2;
			num = max_num;
		cat = catalan(num);
		print(num);
		# print(cat);
		# print();
		if (cat == target_num):
			print(num+2);
			break;
		if (binary_searching):
			if (cat < target_num):
				min_num = num+1;
			else:
				max_num = num-1;
		else:
			if (cat > target_num):
				binary_searching = True
			else:
				min_num = max_num;
