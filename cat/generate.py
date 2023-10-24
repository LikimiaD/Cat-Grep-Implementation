with open('tests/TEST-FILE_005', 'w') as file:
    for i in range(10, 127):
        file.write(chr(i))