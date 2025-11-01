import sys
import re

total_points = 50
percent_per_error = 0.05
min_grade = 25
total_tests = 2**15

errors = 0
inputs = 0
filename = sys.argv[1]

with open(filename) as fp:
    lines = fp.readlines()

    p = re.compile('(\d*):\s*HW\s*=\s*(\d*),\s*SW\s*=\s*(\d*)')

    for line in lines:
        m = p.match(line)
        if m:
            i = int(m.group(1))
            hw = int(m.group(2))
            sw = int(m.group(3))
            
            inputs = inputs + 1
            
            if sw != hw:
                print(f'HW result incorrect for input {i}:', end = ' ')
                print(f'HW={hw}, SW={sw}')
                errors = errors + 1;

if inputs != total_tests:
    print(f'Error: detected {inputs} outputs of {total_tests} results.')

print(f'Total Errors: {errors}')
grade = total_points - total_points * percent_per_error * errors
if grade < min_grade:
    grade = min_grade

print(f'Corresponding grade: {grade}%')
